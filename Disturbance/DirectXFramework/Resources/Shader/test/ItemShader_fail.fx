float2 wndSize;

texture clr;
texture nrm;

sampler2D smp = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

static const float offX = 1.0 / wndSize.x;
static const float offY = 1.0 / wndSize.y;



struct VOutput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

struct VInput
{
	float2 Position : POSITION;
	float2 UV : TEXCOORD;
};

VOutput VSmain(VInput vin)
{
    VOutput output;

	output.UV = vin.UV;
	output.Position = float4(vin.Position, 0.0f, 1.0f);
	
    return output;
}

float sobelColor(float2 uv)
{
	float s00 = clr.Sample(smp, uv + float2(-offX, -offY)).a;
	float s01 = clr.Sample(smp, uv + float2( 0,   -offY)).a;
	float s02 = clr.Sample(smp, uv + float2( offX, -offY)).a;

	float s10 = clr.Sample(smp, uv + float2(-offX,  0)).a;
	float s12 = clr.Sample(smp, uv + float2( offX,  0)).a;
	
	float s20 = clr.Sample(smp, uv + float2(-offX,  offY)).a;
	float s21 = clr.Sample(smp, uv + float2( 0,    offY)).a;
	float s22 = clr.Sample(smp, uv + float2( offX,  offY)).a;
	
	float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
	float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

	float edgeSqr = (sobelX * sobelX + sobelY * sobelY);
	
	return 1-(edgeSqr > 0.07 * 0.07);
}

static const float threadHold = 0.10;

float sobelNormal(float2 uv)
{
	float3 c11 = nrm.Sample(smp, uv).xyz;
   
	float  s00 = max(0,dot(c11,nrm.Sample(smp, uv + float2(-offX,-offY)).xyz)-threadHold);   
	float  s01 = max(0,dot(c11,nrm.Sample(smp, uv + float2(   0,-offY)).xyz)-threadHold);
	float  s02 = max(0,dot(c11,nrm.Sample(smp, uv + float2( offX,-offY)).xyz)-threadHold);

	float  s10 = max(0,dot(c11,nrm.Sample(smp, uv + float2(-offX,   0)).xyz)-threadHold);
	float  s12 = max(0,dot(c11,nrm.Sample(smp, uv + float2( offX,   0)).xyz)-threadHold);
   
	float  s20 = max(0,dot(c11,nrm.Sample(smp, uv + float2(-offX, offY)).xyz)-threadHold);
	float  s21 = max(0,dot(c11,nrm.Sample(smp, uv + float2(   0, offY)).xyz)-threadHold);
	float  s22 = max(0,dot(c11,nrm.Sample(smp, uv + float2( offX, offY)).xyz)-threadHold);
   
	float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
	float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

	float edgeSqr = (sobelX * sobelX + sobelY * sobelY);

	return 1-(edgeSqr > 0.07 * 0.07);
}
struct PSInput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};


struct PSInput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};


float4 PSmain(PSInput pin) : SV_TARGET {
	float sbl = sobelNormal(pin.UV)*sobelColor(pin.UV);
	clip((sbl != 1) - 1);
	
	return float4(1.0f-sbl);
}


//--------------------------------------------------------------//
// Technique Section for ToonShader
//--------------------------------------------------------------//
technique ItemShader
{
   pass Pass_0
   {
	VertexShader = VSmain();
     	PixelShader = PSmain();
   }
}

