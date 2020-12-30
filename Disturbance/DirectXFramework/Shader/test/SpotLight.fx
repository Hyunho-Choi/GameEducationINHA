/*
% A basic spot light shader.
author: Alex Lemke
date: 4/20/12
*/

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float4x4 World : WORLD;
float4x4 View : VIEW;
float4x4 WorldInverseTranspose : WORLDINVERSETRANSPOSE;

//Object Color
float4 ObjectColor : COLOR;

//Ambient Color
float3 AmbientLightColor : COLOR;

//Spot Light
struct SpotLight
{
	float3 color	: COLOR;
	float3 position	: POSITION;
	float range;
	float3 direction : DIRECTION;
	
	//Cone Angle
	float innerConeAngle;
	float outerConeAngle;
};
SpotLight mySpotLight;

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
	float3 LightDirection 	: TEXCOORD4;
};

VertexShaderOutput mainVS(VertexShaderInput input){
	VertexShaderOutput output;
	
	output.Position = mul(float4(input.Position.xyz, 1.0), WorldViewProj);
	output.Normal = mul(float4(input.Normal.xyz, 1.0), WorldInverseTranspose);
	
	float3 worldPosition = mul(float4(input.Position.xyz, 1.0), World);
	output.LightDirection = mySpotLight.position - worldPosition;
	
	output.View = normalize(View[3].xyz - worldPosition);
	
	return output;
}

float4 mainPS(VertexShaderOutput input) : COLOR {
	
	float3 finalColor = float3(0, 0, 0);
	
	//Normalizing Inputs
	float3 normal = normalize(input.Normal);
	float3 view = normalize(input.View);
	
	//Spot Light Direction
	mySpotLight.direction = -normalize(mySpotLight.direction);

	//Attenuation
	float3 directionToLight = input.LightDirection;
	float attenuation = saturate(1 - dot(mySpotLight.direction / mySpotLight.range, directionToLight / mySpotLight.range));
	normalize(directionToLight);
	 
	//Spot Light Angle
	float angleDifference = dot(mySpotLight.direction, directionToLight);
	float spotFactor = lerp(mySpotLight.outerConeAngle, mySpotLight.innerConeAngle, angleDifference); 
	
	//Diffuse
	float3 diffuse = 0;
	float NdotL = saturate(dot(normal, mySpotLight.direction));
	diffuse += (NdotL * attenuation  * spotFactor * mySpotLight.color * ObjectColor);
	
	//Computing Final Color
	finalColor += diffuse + AmbientLightColor;

	return float4(finalColor, 1.0);
}

technique technique0 {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
