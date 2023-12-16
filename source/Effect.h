#pragma once
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

private:
	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
};

