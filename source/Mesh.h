#pragma once
#include "Effect.h"

namespace dae
{
	struct Vertex
	{
		Vector3 position{};
		ColorRGB color{1.f, 1.f, 1.f};
		Vector2 uv{};
	};

	class Mesh final
	{
	public:
		Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		~Mesh();

		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;

		Mesh(Mesh&& other) = delete;
		Mesh& operator=(Mesh&& other) = delete;

		void Render(ID3D11DeviceContext* pDeviceContext, Matrix viewProjectionMatrix);

		void SetDiffuseMap(Texture* pTexture) { m_pEffect->SetDiffuseMap(pTexture); };
		void CycleSamplerStates() { m_pEffect->CycleSampelState(); };

	private:
		Effect* m_pEffect;
		ID3DX11EffectTechnique* m_pTechnique{};
		ID3D11InputLayout* m_pInputLayout{};
		ID3D11Buffer* m_pVertexBuffer{};
		ID3D11Buffer* m_pIndexBuffer{};

		uint32_t m_NumIndices{};
	};
}