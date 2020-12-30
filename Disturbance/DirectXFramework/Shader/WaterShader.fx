////////////////////////////////////////////////////////////////////////////////
// Filename: water.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
float4x4 worldMatrix : World;
float4x4 viewMatrix : View;
float4x4 projectionMatrix : Projection;

texture gReflectionTexture;
texture gRefractionTexture;
texture gNormalTexture;

sampler2D reflectionTexture = sampler_state
{
   Texture = (gReflectionTexture);
};

sampler2D refractionTexture = sampler_state
{
   Texture = (gRefractionTexture);
};

sampler2D normalTexture = sampler_state
{
   Texture = (gNormalTexture);
};


float waterTranslation;

float reflectRefractScale;


///////////////////
// SAMPLE STATES //
///////////////////
/*
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};
*/

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
    float4 refractionPosition : TEXCOORD2;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType WaterVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4x4 reflectProjectWorld;
    float4x4 viewProjectWorld;
	
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Create the reflection projection world matrix.
    reflectProjectWorld = mul(projectionMatrix, worldMatrix);

    // Calculate the input position against the reflectProjectWorld matrix.
    output.reflectionPosition = mul(input.position, reflectProjectWorld);

    // Create the view projection world matrix for refraction.
    viewProjectWorld = mul(viewMatrix, projectionMatrix);
    viewProjectWorld = mul(worldMatrix, viewProjectWorld);
   
    // Calculate the input position against the viewProjectWorld matrix.
    output.refractionPosition = mul(input.position, viewProjectWorld);

    return output;
}



////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 WaterPixelShader(PixelInputType input) : SV_Target
{
    float2 reflectTexCoord;
    float2 refractTexCoord;
    float4 normalMap;
    float3 normal;
    float4 reflectionColor;
    float4 refractionColor;
    float4 color;

    // Move the position the water normal is sampled from to simulate moving water.	
    input.tex.y += waterTranslation;
   // Calculate the projected reflection texture coordinates.
    reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
    reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;
	
    // Calculate the projected refraction texture coordinates.
    refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
    refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;

    // Sample the normal from the normal map texture.
    normalMap = tex2D(normalTexture, input.tex);
	

    // Expand the range of the normal from (0,1) to (-1,+1).
    normal = (normalMap.xyz * 2.0f) - 1.0f;

    // Re-position the texture coordinate sampling position by the normal map value to simulate the rippling wave effect.
    reflectTexCoord = reflectTexCoord + (normal.xy * reflectRefractScale);
    refractTexCoord = refractTexCoord + (normal.xy * reflectRefractScale);

	// Sample the texture pixels from the textures using the updated texture coordinates.
    reflectionColor = float4(0,0,0.5,1);
	//tex2D(reflectionTexture, reflectTexCoord);
    refractionColor = tex2D(refractionTexture, refractTexCoord);

    // Combine the reflection and refraction results for the final color.
    color = lerp(reflectionColor, refractionColor, 0.5f);

    return color;
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
	
	pass p1{
		ColorWritEenable = 7;
		alphablendenable = true;
		srcBlend = srcAlpha;
		destBlend = invSrcAlpha;
	}
}