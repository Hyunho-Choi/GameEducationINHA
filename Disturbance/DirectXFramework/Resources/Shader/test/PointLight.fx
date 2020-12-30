/*
% A basic point light shader.
author: Alex Lemke
date: 4/20/12
*/

float4x4 worldViewProj : WorldViewProjection;
float4x4 world : World;
float4x4 view : View;
float4x4 worldInverseTranspose : WorldInverseTranspose;

//Camera Position
float3 cameraPosition : CameraPosition;

//Object Color
float4 objectColor : Color;

//Ambient Color
float3 ambientLightColor : Color;

//Point Light
struct PointLight
{
	float3 color	: Color;
	float3 position	: Position;
	float range;
};
PointLight myPointLight;

struct VertexShaderInput
{
	float4 Position : POSITION0;
	float3 Normal 	: NORMAL;
};

struct VertexShaderOutput
{
	float4 Position 		: POSITION0;
	float3 Normal 			: TEXCOORD2;
	float3 View 			: TEXCOORD3;
	float3 WorldPosition 	: TEXCOORD4;
};

VertexShaderOutput mainVS(VertexShaderInput input){
	VertexShaderOutput output;
	
	output.Position = mul(float4(input.Position.xyz, 1.0), worldViewProj);
	output.Normal = mul(float4(input.Normal.xyz, 1.0), world);
	
	output.WorldPosition = mul(float4(input.Position.xyz, 1.0), worldInverseTranspose);
	
	output.View = cameraPosition;
	
	return output;
}

float4 mainPS(VertexShaderOutput input) : COLOR {
	
	float3 finalColor = float3(0, 0, 0);
	
	//Normalizing Inputs
	float3 normal = normalize(input.Normal);
	float3 view = normalize(input.View);
	
	//Point Light
	float3 lightDirection = myPointLight.position - input.WorldPosition;
	float attenuation = saturate(1 - dot(lightDirection / myPointLight.range, lightDirection / myPointLight.range));
	lightDirection = normalize(lightDirection);	
	
	//Diffuse
	float3 diffuse = ambientLightColor;
	float NdotL = saturate(dot(normal, lightDirection));
	diffuse += (NdotL  * attenuation * myPointLight.color * objectColor);
	
	//Computing Final Color
	finalColor += diffuse;

	return float4(finalColor, 1.0);
}

technique technique0 {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}