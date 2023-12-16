#include "pch.h"
#include "Mesh.h"

dae::Mesh::Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_pEffect = new Effect(pDevice, std::wstring(L"Resources/PosCol3D.fx"));

	//Create vertex buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	const HRESULT resultVB = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(resultVB))
		return;

	//Create Vertex Layout
	static constexpr uint32_t numElements{ 2 };
	D3D11_INPUT_ELEMENT_DESC vertextDesc[numElements]{};

	vertextDesc[0].SemanticName = "POSITION";
	vertextDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	vertextDesc[0].AlignedByteOffset = 0;
	vertextDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertextDesc[1].SemanticName = "COLOR";
	vertextDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	vertextDesc[1].AlignedByteOffset = 12;
	vertextDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	m_pTechnique = m_pEffect->GetTechnique();

	//Create index buffer
	m_NumIndices = static_cast<uint32_t>(indices.size());
	//bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	//bd.MiscFlags = 0;
	initData.pSysMem = indices.data();
	const HRESULT resultIB = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(resultIB))
		return;

	//Create Input Layout
	D3DX11_PASS_DESC passDesc{};
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	const HRESULT resultIL = pDevice->CreateInputLayout(vertextDesc, numElements,
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout);

	if (FAILED(resultIL))
		return;
}

dae::Mesh::~Mesh()
{
	m_pIndexBuffer->Release();
	m_pVertexBuffer->Release();
	m_pInputLayout->Release();
	m_pTechnique->Release();

	delete m_pEffect;
	m_pEffect = nullptr;
}

void dae::Mesh::Render(ID3D11DeviceContext* pDeviceContext, Matrix viewProjectionMatrix)
{
	//1. Set Primitive Topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//2. Set Input Layout
	pDeviceContext->IASetInputLayout(m_pInputLayout);

	//3. Set VertexBuffer
	constexpr UINT stride = sizeof(Vertex);
	constexpr UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	m_pEffect->GetMatWorldViewProjVariable()->SetMatrix((float*)&viewProjectionMatrix); // transform input position w worldviewprojection

	//4. Set IndexBuffer
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//5. Draw
	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}