#pragma once
#include "Texture.h"

namespace dae
{
	enum class SamplerState
	{
		Point,
		Linear,
		Anisotropic
	};

	class Effect final
	{
	public:
		Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
		~Effect();

		Effect(const Effect& other) = delete;
		Effect& operator=(const Effect& other) = delete;

		Effect(Effect&& other) = delete;
		Effect& operator=(Effect&& other) = delete;

		ID3DX11Effect* GetEffect() const { return m_pEffect; };
		ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; };
		ID3DX11EffectMatrixVariable* GetMatWorldViewProjVariable() const { return m_pMatWorldViewProjVariable; };

		void SetDiffuseMap(const Texture* pDiffuseTexture);
		void CycleSampelState();

	private:
		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
		bool InitializeSamplers(ID3D11Device* pDevice);

		ID3DX11Effect* m_pEffect;
		ID3DX11EffectTechnique* m_pTechnique;
		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;


		// Sampler
		SamplerState m_SamplerState{ SamplerState::Point };
		ID3DX11EffectSamplerVariable* m_pSamplerStateVariable;
		ID3D11SamplerState* m_pSamplerPoint;
		ID3D11SamplerState* m_pSamplerLinear;
		ID3D11SamplerState* m_pSamplerAnisotropic;
	};
}