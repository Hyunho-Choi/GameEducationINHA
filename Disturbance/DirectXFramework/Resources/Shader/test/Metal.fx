//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Metal Effects Group
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// MetalEffect
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Skull Pass
//--------------------------------------------------------------//
string Metal_Effects_Group_MetalEffect_Skull_Pass_Metal_Effect_Skull_Pass_Skull : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Skull.3ds";

//--------------------------------------------------------------//
// vs1
//--------------------------------------------------------------//
VertexShader Metal_Effects_Group_MetalEffect_Skull_Pass_vs1_ = asm
{
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   dcl_tangent    v3
   dcl_binormal   v4
   
   // output vertex position
   m4x4 oPos, v0, c0
   
   // output texture coordinates
   mov oT0, v2
   
   // output normal vector
   mov oT2, v1
   
   // find the light position in object space
   mov  r0, c9
   m4x4 r1, r0, c5
   
   // find light vector
   sub  r1, r1, v0
   
   // normalize light vector
   dp3  r2.x, r1, r1
   rsq  r2.x, r2.x
   mul  r1.xyz, r1.xyz, r2.xxx
   
   // output light vector
   mov oT1, r1
   
   // find the eye position in object space
   mov  r0, c4
   m4x4 r2, r0, c5
   
   // find eye vector
   sub  r2, r2, v0
   
   // normalize eye vector
   dp3  r3.x, r2, r2
   rsq  r3.x, r3.x
   mul  r2.xyz, r2.xyz, r3.xxx
   
   // output eye vector
   mov oT4, r2
   
   // find half angle vector
   add r3, r1, r2
   mul r3, r3, c10.yyy
   
   // normalize half angle vector
   dp3  r4.x, r3, r3
   rsq  r4.x, r4.x
   mul  r3.xyz, r3, r4.xxx
   
   mov r3.xy, v1.xy
   
   // normalize half angle vector
   dp3  r4.x, r3, r3
   rsq  r4.x, r4.x
   mul  r3.xyz, r3, r4.xxx
   
   // output half angle vector vector
   mov oT3, r3
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
     
   
   

};

//--------------------------------------------------------------//
// ps1
//--------------------------------------------------------------//
PixelShader Metal_Effects_Group_MetalEffect_Skull_Pass_ps1_ = asm
{
   ps.1.4
   
   def c0, 0.0f, 0.0f, 0.0f, 1.0f
   
   texcrd r0.rgb, t0  // env map
   texcrd r1.rgb, t1  // L
   texcrd r2.rgb, t2  // N
   texcrd r3.rgb, t3  // H
   texcrd r4.rgb, t4  // V
   
   // N.L
   dp3 r5, r2, r1
   
   // offset basemap
   mov r0.a, c0.a
   add r0, r0, r5
   
   // N.H
   dp3 r4.r, r2, r3
   
   // H.H (aka ||H||^2)
   dp3_sat r4.g, r3, r3
   mov r4.g, 1-r4.g
   
   phase
   
   texld  r1, r4       // N.H^k
   texld  r0, r0       // base
   
   mul r1.rgb, r1, r5
   mul r1.rgb, r1, c1
   
   mul r5.rgb, r5, c2
   mul r0.rgb, r5, r0
   
   add_sat r0, r0, r1
   
   
   

};

//--------------------------------------------------------------//
// Disc Pass
//--------------------------------------------------------------//
string Metal_Effects_Group_MetalEffect_Disc_Pass_Metal_Effect_Disc_Pass_Disc : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Disc.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader Metal_Effects_Group_MetalEffect_Disc_Pass_Vertex_Shader_ = asm
{
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   dcl_tangent    v3
   dcl_binormal   v4
   
   // Swizzle pos to orient disc
   mov  r0.x, v0.x
   mov  r0.y, v0.z
   mov  r0.z, v0.y
   mov  r0.w, v0.w
   
   // Offset disc
   add  r0, r0, c10
   
   // Transform to viewproj space
   m4x4 r1, r0, c0
   mov  oPos, r1
   
   // output texture coordinates
   mov oT0, v2.yx
   
   // find the light position in object space
   mov  r0, c9
   m4x4 r1, r0, c5
   
   // find light vector
   sub  r1, r1, v0
   
   // normalize light vector
   dp3  r2.x, r1, r1
   rsq  r2.x, r2.x
   mul  r1.xyz, r1, r2.xxx
   
   // rotate light vector into tangent space
   dp3 r2.x, r1, c9
   dp3 r2.y, r1, v4
   
   // orient normal
   mov r3.x, v1.x
   mov r3.y, v1.z
   mov r3.z, v1.y
   
   dp3 r2.z, r1, r3
   mov r2.w, v1.w
   
   // output light vector
   mov oT1, r2
   
   // find the eye position in object space
   mov  r0, c4
   m4x4 r1, r0, c5
   
   // find eye vector
   sub  r1, r1, v0
   
   // normalize eye vector
   dp3  r3.x, r1, r1
   rsq  r3.x, r3.x
   mul  r1.xyz, r1, r3.xxx
   
   // rotate eye vector into tangent space
   dp3 r3.x, r1, c9
   dp3 r3.y, r1, v4
   dp3 r3.z, r1, v1
   mov r3.w, v1.w
   
   // output eye vector
   mov oT4, r3
   
   // find half angle vector
   add r1, r2, r3
   
   // normalize half angle vector
   dp3  r3.x, r1, r1
   rsq  r3.x, r3.x
   mul  r1, r1, r3.xxx
   
   // output half angle vector
   mov oT3, r1
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
     

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader Metal_Effects_Group_MetalEffect_Disc_Pass_Pixel_Shader_ = asm
{
   ps.1.4
   
   def c0, 0.0f, 0.0f, 0.0f, 1.0f
   
   texld  r0, t0      // base map
   texld  r1, t0      // N
   texcrd r2.rgb, t3  // H
   texld  r3, t0      // k
   texcrd r4.rgb, t1  // L
   
   // N.L
   dp3_sat r4, r1_bx2, r4
   
   // N.H
   dp3 r1.r, r1_bx2, r2
   
   // (N.H)^2
   mul r1.r, r1.r, r1.r
   
   // ||H||^2
   dp3_sat r1.b, r2, r2 
   
   // ||H||^2  * k * 0.5
   mul_d2_sat r1.g, r1.b, r3.r
   
   mov r1.g, 1-r1.g
   
   phase
   
   texld r2, r1_dz
   
   mov r0.a, c0.a
   
   mov r4.a, c0.a
   mul r2, r2, r4
   mul r2, r2, r0
   
   
   mul r0, r0, r4 
   add_sat r0, r0, r2
   
   

};

//--------------------------------------------------------------//
// Car Paint Effects
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// CheapShiny
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 1
//--------------------------------------------------------------//
string Car_Paint_Effects_CheapShiny_Pass_1_Teapot : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Teapot.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader Car_Paint_Effects_CheapShiny_Pass_1_Vertex_Shader_ = asm
{
    
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   dcl_tangent    v3
   dcl_binormal   v4
   
   m4x4 oPos, v0, c0
   
   
   // get the y component of the normal and transform it into viewspace
   mov  r0, v1
   m3x3 r1, r0, c4
   
   // scale and bias to get it to range [0,1]
   mul r1.y, r1.y, c8.y
   
   // output as tex coord to index horizon gradient
   add oT0, r1.y, c8.y
   
   
   
   
   
     

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader Car_Paint_Effects_CheapShiny_Pass_1_Pixel_Shader_ = asm
{
    
   ps.1.4
   
   
   // use the normalized horizon angle to index the horizon gradient
   texld r0, t0
   
   mov  r1.rgb, c0
   +mov_sat r1.a, c0.a
   
   // scale reflection by inverse base alpha
   mul r0, r0, 1-r1.a
   
   // scale base by base alpha then add base and reflection colors
   mad_sat r0, r1, r1.a, r0
   
   
   
   
   
   
     
};

//--------------------------------------------------------------//
// CarPaint2Tone
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 1
//--------------------------------------------------------------//
string Car_Paint_Effects_CarPaint2Tone_Pass_1_Teapot : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Teapot.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader Car_Paint_Effects_CarPaint2Tone_Pass_1_Vertex_Shader_ = asm
{
    
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   dcl_tangent    v3
   dcl_binormal   v4
   
   m4x4  oPos, v0, c4              //OutPos = ObjSpacePos * WVP Matrix
   
   mov  r0, c2                     // find the eye position in object space
   m4x4 r1, r0, c8
   
   sub   r8, r1, v0                //viewer direction
   
   dp3   r10.x, r8, r8             //magnitude
   rsq   r10.y, r10.x              //normailize
   mul   r8, r8, r10.y             //V normalized view vector in r2
   add   r1, r8, r8                //2V
   dp3   r10.x, r1, v3             //2V.N
   mad   r7, v1, r10.x, -r8        // W=(2V.N)N-V reflevted view vector in r1
   mov   r5, v1                    //normal (tZ)
   mov   r4, v3                    //tangent  (tY)
   mul   r3, r4.yzxw, r5.zxyw 
   mad   r3, r4.zxyw, -r5.yzxw, r3 //setup binormal (tX) vector perp to normal in r3
   
   m3x3  oT1, v1, c12              //xform view vec into tangent space
   mul   oT0, v2, c1.x             //output bumpmap texcoords
   
   m3x3  oT2, v1, c12
   
   
   
   
   
   
   
     

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader Car_Paint_Effects_CarPaint2Tone_Pass_1_Pixel_Shader_ = asm
{
    
   ps.1.4
   texld r0, t0                          //bump map 
   
   texld r1, t1                          //view vector lookup (norm cube)
   texld r2, t2                          //env map reflection vector lookup
   
   lrp      r3.rgb, c7.a, r0_bx2, c7
   +mov_sat r3.a, r1_bx2.b 
   mul_x8   r5.rgb, r2, r2.a
   +mad_sat r5.a, 1-r3.a, 1-r3.a, c3.a   // (1-N.V)^2 + fresnel boost 
   
   mul      r5.rgb, r5, r5.a              // mult HDR by fresnel term
   dp3      r3.a, r3, r1_bx2              //sparkle N.V
   mul      r3.a, r3.a, r3.a              // (N.V)^2
   
   mad_sat  r3.rgb, c1, r3_x2.a, r5          
   +mad_sat r3.a, r3.a, r3.a, c0.a          // ~(N.V)^8
   
   
   mad_sat  r0.rgb, c2, r3_x2.a, r3        //add sparkles          
   
   
   +mul_sat  r0.a, r2.a, r2.a            //hdr glow in alpha + sparkles
   
   
   
   
   
     
};

//--------------------------------------------------------------//
// CarPaintMetalic
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string Car_Paint_Effects_CarPaintMetalic_Single_Pass_Teapot : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Teapot.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader Car_Paint_Effects_CarPaintMetalic_Single_Pass_Vertex_Shader_ = asm
{
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   dcl_tangent    v3
   dcl_binormal   v4
   
   m4x4  oPos, v0, c4              // OutPos = ObjSpacePos * WVP Matrix
   
   mov  r0, c2                     // Find the eye position in object space
   m4x4 r1, r0, c8
   
   sub   r8, r1, v0                // Compute view direction
   
   dp3   r10.x, r8, r8             // Normalize the view vector
   rsq   r10.y, r10.x              // 
   mul   r8, r8, r10.y             // 
   
   add   r1, r8, r8                // Compute 2V
   dp3   r10.x, r1, v1             // Compute 2V.N
   mad   r7, v1, r10.x, -r8        // Compute reflected view vector 
   				     //     W = (2V.N)N - V
   
   mov   r5, v1                    // The normal vector
   mov   r4, v3                    // The tangent vector
   mul   r3, r4.yzxw, r5.zxyw      // Setup binormal (tX) vector perp to normal in r3
   mad   r3, r4.zxyw, -r5.yzxw, r3 // 
   m3x3  r6, r8, r3                // Transform view vector into tangent space
   mul   oT0, v2, c1.x             // Output bump map texture coordinates
   
   m3x3  oT1, r7, c12              // Output reflection vector (in object space)
   mov   oT2, r3                   // Output inverse tangent space basis vector 0 (X) 
   mov   oT3, r4                   // Output inverse tangent space basis vector 1 (Y) 
   mov   oT4, r5                   // Output inverse tangent space basis vector 2 (Z) 
   mov   oT5, r8                   // Output view vector in object space
   
   mov   oD0.xyz, r6               // Output view vector in tangent space
   
   
   
   
   
   
   
     

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader Car_Paint_Effects_CarPaintMetalic_Single_Pass_Pixel_Shader_ = asm
{
   ps.1.4
   
   texld    r0,     t0                  // Read from microflake noise map to get perturbed normals
   texld    r1,     t1                  // Environment map reflection vector lookup ( for the gloss coat)
   texcrd   r3.rgb, t2                  // Inverse tangent space base vector 0 ( tan X )
   texcrd   r4.rgb, t3                  // Inverse tangent space base vector 1 ( tan Y )
   texcrd   r5.rgb, t4                  // Inverse tangent space base vector 2 ( ten Z )
   texcrd   r2.rgb, t5                  // The view vector 
   
   lrp      r0.rgb, c5.a, r0_bx2, c5    // Increase the microflake perturbed normal a bit
   
   mul      r3.rgb, r0.r, r3         
   mad      r3.rgb, r0.g, r4, r3
   mad      r3.rgb, r0.b, r5, r3        // Convert the perturbed normal from 
   dp3_sat  r4, r2, r3                  // Nbw.Vw
   
   mad      r3.rgb, r3_x2, r4, -r2      // Compute Rw = 2N(V.N)-V
   
   mul_x8   r1.rgb, r1, r1.a            // Brighten  up the luminosity by 8
   mov      r5, r1
   
   phase
   texld    r0, t0                      // Load the second microflake normal from the microflake                       
   texld    r1, r3                      // noise map to simulate second layer of metallic flakes
   
   mul_sat   r1.rgb, r1, r1.a           // Increase the 2nd microflake layer normal 
   +mad_sat  r0.a, 1-v0.b, 1-v0.b, c1.a // Compute (1-N.V)^2
   
   lrp       r4.rgb, c3, r4, v0.b       // Simulate color scattering
   mul       r4.rgb, r4_x2, c4          // Combine resulting color with base color
   mul       r1.rgb, r1, r0             // Add microflakes color 
   mad       r1.rgb, r1, r5, r4         // Combine sparkles with base color gradient
   lrp       r0.rgb, r0.a, r5, r1       // Add reflections from the environment map
   
   mul       r0.a, r1.a, c2.a          // Final color composite
   

};

//--------------------------------------------------------------//
// Technique Section for Metal Effects Group
//--------------------------------------------------------------//
technique MetalEffect
{
   pass Skull_Pass
   {
      CULLMODE = CCW;
      ZFUNC = LESS;
      ZENABLE = TRUE;

      VertexShader = (Metal_Effects_Group_MetalEffect_Skull_Pass_vs1_);
      PixelShader = (Metal_Effects_Group_MetalEffect_Skull_Pass_ps1_);
   }

   pass Disc_Pass
   {
      ZFUNC = LESS;
      CULLMODE = NONE;

      VertexShader = (Metal_Effects_Group_MetalEffect_Disc_Pass_Vertex_Shader_);
      PixelShader = (Metal_Effects_Group_MetalEffect_Disc_Pass_Pixel_Shader_);
   }

}

//--------------------------------------------------------------//
// Technique Section for Car Paint Effects
//--------------------------------------------------------------//
technique CheapShiny
{
   pass Pass_1
   {

      VertexShader = (Car_Paint_Effects_CheapShiny_Pass_1_Vertex_Shader_);
      PixelShader = (Car_Paint_Effects_CheapShiny_Pass_1_Pixel_Shader_);
   }

}

technique CarPaint2Tone
{
   pass Pass_1
   {

      VertexShader = (Car_Paint_Effects_CarPaint2Tone_Pass_1_Vertex_Shader_);
      PixelShader = (Car_Paint_Effects_CarPaint2Tone_Pass_1_Pixel_Shader_);
   }

}

technique CarPaintMetalic
{
   pass Single_Pass
   {

      VertexShader = (Car_Paint_Effects_CarPaintMetalic_Single_Pass_Vertex_Shader_);
      PixelShader = (Car_Paint_Effects_CarPaintMetalic_Single_Pass_Pixel_Shader_);
   }

}

