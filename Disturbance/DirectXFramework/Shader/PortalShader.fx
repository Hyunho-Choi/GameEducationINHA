////////////////////////////////////////////////////////////////////////////////
// Filename: water.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
float4x4 gMVP : WorldViewProjection;
float gTime : Time;
float3 gColor = float3(0.5,0.5,1);
float radius = 0.8f;
float angle = 1.5f;
float2 center = float2(0.5,0.5);
texture gRefractionTexture;
texture gNormalTexture;
sampler2D refractionTexture = sampler_state
{
   Texture = (gRefractionTexture);
};
sampler2D normalTexture = sampler_state
{
   Texture = (gNormalTexture);
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};
struct PixelInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType WaterVertexShader(VertexInputType input)
{
    PixelInputType output;
	
    input.position.w = 1.0f;
    output.position = mul(input.position, gMVP);
   
    output.tex = input.tex;
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

float4 WaterPixelShader(PixelInputType input) : SV_Target
{

  	float2 tc = input.tex;
  	tc -= center;
  	float dist = length(tc);
  	if (dist < radius) 
  	{
    	float percent = (radius - dist) / radius;
    	float theta = percent * percent * angle * 8.0;
    	float s = sin(theta + gTime);
    	float c = cos(theta + gTime);
    	tc = float2(dot(tc, float2(c, -s)), dot(tc, float2(s, c)));
  	}
 	tc += center;
  	
  	return float4(gColor, tex2D(refractionTexture, input.tex).x - tex2D(normalTexture, tc).x + 0.4);
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique technique0
{
    pass pass0
    {
		ColorWritEenable = 0;
		VertexShader = compile vs_3_0 WaterVertexShader();
		PixelShader = compile ps_3_0 WaterPixelShader();
    }
	
	pass p2{
		ColorWritEenable = 7;
		alphablendenable = true;
		srcBlend = srcAlpha;
		destBlend = invSrcAlpha;
	}
}