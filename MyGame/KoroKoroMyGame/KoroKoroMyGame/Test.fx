float4x4	WorldMat;
float4x4	ViewMat;
float4x4	ProjMat;

float4		LightDir;
float4		MaterialDiffuse;
float4 		ViewPos;

struct VS_Out
{
	float4 Pos		: POSITION;
	float4 Tex 	    : TEXCOORD0;
	float3 ViewVec	: TEXCOORD1;
	float4 Normal	: TEXCOORD2;
};

VS_Out VertexShader_Main(float4 inPos : POSITION,float4 inTex : TEXCOORD0, float3  inNormal : NORMAL0)
{
	VS_Out Out;
	
	float4 WorldPos = mul(inPos, WorldMat);
	Out.Pos = mul(WorldPos, ViewMat);
	Out.Pos = mul(Out.Pos, ProjMat);
	Out.Normal = mul(inNormal, WorldMat);
	
    Out.Tex = inTex;
    Out.Normal = mul(inNormal, WorldMat);
    Out.ViewVec = normalize(ViewPos - WorldPos);
	return Out;
}

float4 PixelShader_Main(VS_Out In) : COLOR0
{
	/*
	float4 Color;
	float3 HarfVec = normalize(normalize(-LightDir.xyz) + normalize(In.ViewVec));
	
//	float Diffuse = dot(normalize(In.Normal.xyz), normalize(-LightDir));
	
	float Diffuse = max(dot(normalize(In.Normal.xyz), normalize(-LightDir,xyz),0.2f);
	float Specular = dot(normalize(In.Normal.xyz), HarfVec);
	
	Specular = pow(Specular,15.0f);
	
	
	Color.rgb = MaterialDiffuse.rgb * Diffuse;
	Color.a = 1.0f;
	return Color;
	*/
/*
	// ランバート、 * 0.5 + 0.5fでハーフランバート
	float4 Color;
	Color.rgb = dot(normalize(In.Normal.xyz), normalize(-LightDir)) * 0.5f + 0.5f;
	Color.a = 1.0f;
	
	return Color;
*/



	float4 Color;
	
	
	float Diffuse = dot(normalize(In.Normal.xyz), normalize(-LightDir));
	
	float3 HarfVec = (normalize(-LightDir.xyz) + normalize(In.ViewVec.xyz));
	float  Speculer = dot(normalize(HarfVec), normalize(In.Normal.xyz));
	
	float SpeculerPower = 18.0f;
	Speculer = pow(Speculer, SpeculerPower);
	
	Color.rbg = MaterialDiffuse.rbg * max(Diffuse,0.2f) + Speculer ;
	Color.a = MaterialDiffuse.a;
	
	return Color;


}

technique main
{
	pass P0
	{
 		VertexShader = compile vs_2_0  VertexShader_Main();
		PixelShader = compile ps_2_0 PixelShader_Main();
	}
}