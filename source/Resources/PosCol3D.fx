float4x4 gWorldViewProj : WorldViewProjection;
Texture2D gDiffuseMap : DiffuseMap;
SamplerState gSampleState : Sampler;
//--------------------------------------------------------------------------
//	Input/Output Structs
//--------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR;
    float2 uv : TEXCOORD;
};


//--------------------------------------------------------------------------
//	Vertex Shader
//--------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//output.Position = float4(input.Position, 1.0f);
	output.Position = mul(float4(input.Position, 1.0f), gWorldViewProj);
	output.Color = input.Color;
	output.uv = input.uv;

	return output;
}


//--------------------------------------------------------------------------
//	Pixel Shader
//--------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 colorSample = gDiffuseMap.Sample(gSampleState, input.uv) * input.Color;
	return float4(colorSample, 1.0f);
}


//--------------------------------------------------------------------------
//	Technique
//--------------------------------------------------------------------------
technique11 DefaultTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}