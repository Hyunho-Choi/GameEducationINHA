/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

float4x4 gWorld : World;
float4x4 gViewProj : ViewProjection;
float2 gCharactor2DPos = {0,-0.1};
float4 gLightPos;
float  gMaxLightDist = 10;
float  gBrightness = 0.5;


sampler2D gTexture;
texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\InhaTeamProj\\DirectXFramework\\DirectXFramework\\Resources\\XFile\\PolygonOffice_Texture_01_A.png";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

struct VS_INPUT 
{
   float4 mPosition : POSITION;
   float3 mNormal: NORMAL;   
   float2 mUV: TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 mPosition : POSITION;
   float4 mColor : COLOR;   
   float2 mUV: TEXCOORD0;
   float2 PosOut : TEXCOORD1;
};

struct PS_INPUT 
{
   float2 PosOut : TEXCOORD1;
   float4 mColor : COLOR;   
   float2 mUV: TEXCOORD0;
};

VS_OUTPUT mainVS1(VS_INPUT input)
{
	VS_OUTPUT Output;
	Output.mPosition = mul(input.mPosition , gWorld);
	Output.mColor = 1 - input.mPosition.y * 0.1;
	//Output.mColor = dot(float4(input.mNormal,0.f) , gLightPos - Output.mPosition);
	//Output.mColor *= saturate(1.f / length(gLightPos - Output.mPosition) * 3.0);
	Output.mPosition = mul(Output.mPosition, gViewProj);
	Output.PosOut =  Output.mPosition.xy;
	Output.mUV = input.mUV;
	return Output;
}

float4 mainPS1(PS_INPUT input) : COLOR 
{
	
	float4 albedo = tex2D(DiffuseSampler, input.mUV);
	albedo *= clamp(input.mColor.y, 0.5f, 2.f);

	return float4(albedo.xyz * gBrightness, 
	4*sqrt(pow(input.PosOut.x - gCharactor2DPos.x, 2) + pow(input.PosOut.y - gCharactor2DPos.y, 2)));
}

/*
VS_OUTPUT OutlineVertexShader(VS_INPUT input)
{
	VS_OUTPUT Output;
	Output.mColor = float4(1.0,1.0,1.0,0.3);
	Output.mPosition = mul(float4(input.mPosition.xyz + input.mNormal.xyz * 0.03f , 1.0), gWorldViewProj);
	return Output;
}

float4 OutlinePixelShader(VS_OUTPUT input) : COLOR 
{
	return input.mColor;
}
*/

technique technique0 {
	pass p1 {
		ColorWritEenable = 0;
		VertexShader = compile vs_3_0 mainVS1();
		PixelShader = compile ps_3_0 mainPS1();
	}
	
	pass p2{
		ColorWritEenable = 7;
		alphablendenable = true;
		srcBlend = srcAlpha;
		destBlend = invSrcAlpha;
	}
	/*
	pass p0 {
		ZWriteEnable = false;
		VertexShader = compile vs_3_0 OutlineVertexShader();
		PixelShader = compile ps_3_0 OutlinePixelShader();
		ZWriteEnable = true;
	}
	*/
}
