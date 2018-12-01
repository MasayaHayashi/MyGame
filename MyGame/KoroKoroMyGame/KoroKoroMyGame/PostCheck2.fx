float4 MaterialDiffuse;
float4 ScreenSize;

texture Texture;
sampler TexSampler = sampler_state
{
	Texture = <Texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_Out
{
	float4 Pos		: SV_POSITION;
	float4 Tex 	    : TEXCOORD0;
	float4 Color	: COLOR;
};

VS_Out VertexShader_Main(float4 inPos : POSITION, float4 Color : COLOR, float2 inTex : TEXCOORD0)
{
	VS_Out Out;
	
	Out.Pos = float4(inPos.x, inPos.y, 0.0f, 1.0f);
    Out.Tex = float4(inTex.xy, 0.0f, 0.0f);
    Out.Color = Color;
	return Out;
}


float4 PixelShader_Main(VS_Out In) : COLOR0
{
	float4 Color = tex2D(TexSampler, In.Tex);
	float3 LuminanceTable = float3(0.299f, 0.587f, 0.114f);
	
	float Luminance = dot(Color.rgb, LuminanceTable);
	
	float4 OutColor;
	OutColor.rgb = Luminance;
	OutColor.a = Color.a;
	return OutColor;
}

technique main
{
	pass P0
	{
 		VertexShader = compile vs_2_0  VertexShader_Main();
		PixelShader = compile ps_2_0 PixelShader_Main();
	}
}