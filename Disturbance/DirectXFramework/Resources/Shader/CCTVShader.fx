float4x4 gWorldViewProj : WorldViewProjection;
texture gRoundTexture : DIFFUSE;

sampler2D RoundSampler = sampler_state {
    Texture = <gRoundTexture>;
};

struct VS_INPUT 
{
   float4 mPosition : POSITION;
   float2 mUV: TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 mPosition : POSITION;
   float2 mUV: TEXCOORD0;
};

struct PS_INPUT 
{
   float2 mUV: TEXCOORD0;
   float2 PosOut : TEXCOORD1;
};

VS_OUTPUT mainVS1(VS_INPUT input)
{
	VS_OUTPUT Output;
	Output.mPosition = mul(input.mPosition, gWorldViewProj);
	Output.mUV = input.mUV;
	return Output;
}

float4 mainPS1(PS_INPUT input) : COLOR 
{
	float alpha = lerp(0.f ,0.3f ,tex2D(RoundSampler, input.mUV).x);
	return float4(1,1,1,abs(alpha-0.3));
}

technique technique0 {
	pass p0 {
		//ZEnable = false;
		//ZWriteEnable = false;
		ColorWritEenable = 8;
		VertexShader = compile vs_3_0 mainVS1();
		PixelShader = compile ps_3_0 mainPS1();
	}
	
	pass p1{
		ColorWritEenable = 7;
		alphablendenable = true;
		srcBlend = srcAlpha;
		destBlend = ONE;
	}
}
