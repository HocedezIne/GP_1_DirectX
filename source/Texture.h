#pragma once
#include "d3dx11effect.h"
#include "ColorRGB.h"
#include <SDL_surface.h>
#include <string>
#include "Vector2.h"

namespace dae
{
	class Texture final
	{
	public:
		~Texture();

		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;

		Texture(Texture&& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		static Texture* LoadFromFile(ID3D11Device* pDevice, const std::string& path);

		ID3D11ShaderResourceView* GetShaderResourceView() const {};
		ID3D11ShaderResourceView* GetSRV() const { return m_pShaderResourceView; };

	private:
		Texture();

		ID3D11Texture2D* m_pTexture{};
		ID3D11ShaderResourceView* m_pShaderResourceView{};
	};

}