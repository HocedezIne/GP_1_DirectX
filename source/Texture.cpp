#include "pch.h"
#include "Texture.h"

namespace dae
{
	Texture::~Texture()
	{
		if (m_pTexture != nullptr)
		{
			m_pShaderResourceView->Release();
			m_pTexture->Release();
		}
	}

	Texture* Texture::LoadFromFile(ID3D11Device* pDevice, const std::string& path)
	{
		SDL_Surface* surfacePtr{ IMG_Load(path.c_str()) };
		if (!surfacePtr) return nullptr;

		Texture* newTexture = new Texture();

		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = surfacePtr->w;
		desc.Height = surfacePtr->h;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initdata;
		initdata.pSysMem = surfacePtr->pixels;
		initdata.SysMemPitch = static_cast<UINT>(surfacePtr->pitch);
		initdata.SysMemSlicePitch = static_cast<UINT>(surfacePtr->h * surfacePtr->pitch);

		HRESULT hr = pDevice->CreateTexture2D(&desc, &initdata, &newTexture->m_pTexture);
		if (FAILED(hr))
		{
			delete newTexture;
			return nullptr;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SVRDesc{};
		SVRDesc.Format = format;
		SVRDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SVRDesc.Texture2D.MipLevels = 1;

		hr = pDevice->CreateShaderResourceView(newTexture->m_pTexture, &SVRDesc, &newTexture->m_pShaderResourceView);

		if (FAILED(hr))
		{
			delete newTexture;
			return nullptr;
		}

		SDL_FreeSurface(surfacePtr);
		return newTexture;
	}
}