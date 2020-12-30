/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

float4x4 gWorldViewProj : WorldViewProjection;
float gOutlineWidth = 0.8f;
float2 gCharactor2DPos = {0,0};
float gBrightness = 0.5f;

texture DiffuseMap_Tex;
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
   float2 mUV: TEXCOORD0;
   float2 PosOut : TEXCOORD1;
};

struct PS_INPUT 
{
   float2 PosOut : TEXCOORD1; 
   float2 mUV: TEXCOORD0;
};

struct Line_VS_INPUT 
{
   float4 mPosition : POSITION;
   float3 mNormal: NORMAL;
};

struct Line_VS_OUTPUT 
{
   float4 mPosition : POSITION;
   float4 mColor : COLOR;
};

struct Line_PS_INPUT 
{
   float2 PosOut : TEXCOORD1;
   float4 mColor : COLOR; 
};

VS_OUTPUT mainVS1(VS_INPUT input)
{
	VS_OUTPUT Output;
	Output.mPosition = mul(input.mPosition , gWorldViewProj);
	Output.PosOut =  Output.mPosition.xy;
	Output.mUV = input.mUV;
	return Output;
}

float4 mainPS1(PS_INPUT input) : COLOR 
{
	float4 albedo = tex2D(DiffuseSampler, input.mUV);
	return float4(albedo.xyz * gBrightness, 2*sqrt(pow(input.PosOut.x - gCharactor2DPos.x, 2) + pow(input.PosOut.y - gCharactor2DPos.y, 2)));
}


Line_VS_OUTPUT OutlineVertexShader(Line_VS_INPUT input)
{
	Line_VS_OUTPUT Output;
	Output.mColor = float4(1.0,1.0,1.0,0.3);
	Output.mPosition = mul(float4(input.mPosition.xyz + input.mNormal.xyz * 0.3f , 1.0), gWorldViewProj);
	return Output;
}

float4 OutlinePixelShader(Line_VS_OUTPUT input) : COLOR 
{
	return input.mColor;
}


technique technique0 
{
	pass p0 {
		//ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_3_0 OutlineVertexShader();
		PixelShader = compile ps_3_0 OutlinePixelShader();
	}
	
	pass p1 {
		ZEnable = true;
		ZWriteEnable = true;
		VertexShader = compile vs_3_0 mainVS1();
		PixelShader = compile ps_3_0 mainPS1();
	}
}
