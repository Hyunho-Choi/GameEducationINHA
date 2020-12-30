/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/
float time = 0;

#define normalStrength 40.0
#define timeScale 			time * 1.0
#define fireMovement 		float2(-0.01, -0.5)
#define distortionMovement	float2(-0.01, -0.3)
#define distortionStrength	0.1

float4x4 WorldViewProj : WorldViewProjection;
float2 Res = {1080, 720};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

float rand(float2 co) {

	float a = 11.63;
	float b = 84.268;
	float c = 43758.5453;
	
	return frac(sin(dot(co.xy, float2(a, b))) * c);
}

float2 hash(float2 p) {

	p = float2(dot(p, float2(127.1, 311.7)),
				dot(p, float2(269.5, 183.3)));
				
	return -1.0 + 2.0 * frac(sin(p) * 43758.5453123);
}

float noise(float2 p ) {
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;

	float2 i = floor( p + (p.x+p.y) * K1 );

    float2 a = p - i + (i.x+i.y) * K2;
    float2 o = step(a.yx,a.xy);
    float2 b = a - o + K2;
	float2 c = a - 1.0 + 2.0*K2;

    float3 h = max( 0.5-float3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );

	float3 n = h*h*h*h*float3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));

    return dot( n, float3(70.0,70.0,70.0) );
}

float fbm (float2 p) {
    float f = 0.0;
    float2x2 m = float2x2( 1.6,  1.2, -1.2,  1.6 );
    f  = 0.5000 * noise(p); p = mul(m , p);
    f += 0.2500*noise(p); p = mul(m, p);
    f += 0.1250*noise(p); p = mul(m, p);
    f += 0.0625*noise(p); p = mul(m, p);
    f = 0.5 + 0.5 * f;
    return f;
}

float3 bumpMap(float2 uv) {
    float2 s = 1. / Res;
    float p =  fbm(uv);
    float h1 = fbm(uv + s * float2(1., 0));
    float v1 = fbm(uv + s * float2(0, 1.));

   	float2 xy = (p - float2(h1, v1)) * normalStrength;
    return float3(xy + .5, 1.);
}

VS_OUTPUT mainVS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.mPosition = mul(input.mPosition, WorldViewProj);
	output.mTexCoord = input.mTexCoord;
	return output;
}

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

float4 mainPS(PS_INPUT input) : COLOR
{
	float x = 0;
	float2 uv = input.mTexCoord;

	//x = fbm(uv * 6.);
	
	//x = x * 0.5 + 0.5;
	//return float4(x, x, x, 1.0);
	
	//float3 bump = bumpMap(uv * 5.0);
	//return float4(bump, 1.0);
	
    float3 normal = bumpMap(uv * float2(1.0, 0.3) + distortionMovement * timeScale);
    //return float4(normal, 1.0);
	
    float2 displacement = clamp((normal.xy - .5) * distortionStrength, -1., 1.);
    uv += displacement;

    float2 uvT = (uv * float2(1.0, 0.5)) + fireMovement * timeScale;
    float n = pow(fbm(6.0 * uvT), 1.0);

    float gradient = pow(1.0 - uv.y, 2.0) * 5.;
    float finalNoise = n * gradient;

    float3 color = finalNoise * float3(2.*n, 2.*n*n*n, n*n*n*n);
    return float4(color, 1.0);
	
    //return float4(float3(finalNoise, finalNoise,finalNoise), 1.);
}

technique technique0 {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
