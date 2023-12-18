#include "pch.h"
#include "Effect.h"

namespace dae
{
	Effect::Effect(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		m_pEffect = LoadEffect(pDevice, assetFile);

		m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
		if (!m_pTechnique->IsValid())
			std::wcout << L"Technique is not valid\n";

		m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
		if (!m_pMatWorldViewProjVariable->IsValid())
			std::wcout << L"m_pMatWorldViewProjVariable not valid!\n";

		m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseMapVariable->IsValid())
			std::wcout << L"m_pDiffuseMatVariable not valid!\n";

		m_pSamplerStateVariable = m_pEffect->GetVariableByName("gSampleState")->AsSampler();
		if (!m_pSamplerStateVariable->IsValid())
			std::wcout << L"m_pSamplerStateVariable not valid!\n";

		if (!InitializeSamplers(pDevice))
			std::wcout << L"Failed to initialize samplers\n";
	}

	Effect::~Effect()
	{
		if (m_pSamplerStateVariable != nullptr)
		{
			m_pSamplerAnisotropic->Release();
			m_pSamplerLinear->Release();
			m_pSamplerPoint->Release();
			m_pSamplerStateVariable->Release();
		}

		m_pDiffuseMapVariable->Release();
		m_pMatWorldViewProjVariable->Release();
		if (m_pEffect != nullptr)
		{
			m_pTechnique->Release();
			m_pEffect->Release();
		}
	}

	void Effect::SetDiffuseMap(const Texture* pDiffuseTexture)
	{
		if (m_pDiffuseMapVariable)
			m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetSRV());
	}

	void Effect::CycleSampelState()
	{
		m_SamplerState = static_cast<SamplerState>((int(m_SamplerState) + 1) % 3);
		//std::wcout << int(m_SamplerState) << std::endl;

		switch (m_SamplerState)
		{
		case dae::SamplerState::Point:
			m_pSamplerStateVariable->SetSampler(0,m_pSamplerPoint);
			break;
		case dae::SamplerState::Linear:
			m_pSamplerStateVariable->SetSampler(0, m_pSamplerLinear);
			break;
		case dae::SamplerState::Anisotropic:
			m_pSamplerStateVariable->SetSampler(0, m_pSamplerAnisotropic);
			break;
		}
	}

	bool Effect::InitializeSamplers(ID3D11Device* pDevice)
	{
		D3D11_SAMPLER_DESC desc{};
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MaxAnisotropy = 1;

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		HRESULT result = pDevice->CreateSamplerState(&desc, &m_pSamplerPoint);
		if (FAILED(result)) return false;

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		result = pDevice->CreateSamplerState(&desc, &m_pSamplerLinear);
		if (FAILED(result)) return false;

		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		result = pDevice->CreateSamplerState(&desc, &m_pSamplerAnisotropic);
		if (FAILED(result)) return false;

		return true;
	}

	ID3DX11Effect* Effect::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		HRESULT result;
		ID3D10Blob* pErrorBlob{ nullptr };
		ID3DX11Effect* pEffect;

		DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined ( _DEBUG )
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		result = D3DX11CompileEffectFromFile(assetFile.c_str(), nullptr, nullptr,
			shaderFlags, 0, pDevice, &pEffect, &pErrorBlob);

		if (FAILED(result))
		{
			if (pErrorBlob != nullptr)
			{
				const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

				std::wstringstream ss;
				for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
					ss << pErrors[i];

				OutputDebugStringW(ss.str().c_str());
				pErrorBlob->Release();
				pErrorBlob = nullptr;

				std::wcout << ss.str() << std::endl;
			}
			else
			{
				std::wstringstream ss;
				ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile;
				std::wcout << ss.str() << std::endl;
				return nullptr;
			}
		}

		return pEffect;
	}
}