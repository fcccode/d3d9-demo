uniform extern float4x4 g_view_proj;
uniform extern float3   g_light_dir;
uniform extern texture  g_texture;

static float g_tex_scale = 24.0f;

sampler TexSampler = sampler_state {
    Texture   = <g_texture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

struct OutputVS {
    float4 pos   : POSITION0;
    float2 tex   : TEXCOORD0;
    float  shade : TEXCOORD1;
};

OutputVS TerrainVS(float3 pos    : POSITION0,
                   float3 normal : NORMAL0,
                   float2 tex    : TEXCOORD0) {
    OutputVS output;

    output.pos   = mul(float4(pos, 1.0f), g_view_proj);
    output.tex   = tex * g_tex_scale;
    output.shade = saturate(max(0.0f, dot(normal, g_light_dir)) + 0.3f);

    return output;
}

float4 TerrainPS(float2 tex : TEXCOORD0, float shade : TEXCOORD1) : COLOR {
    return float4(tex2D(TexSampler, tex).rgb * shade, 1.0f);
}

technique TerrainTech {
    pass Pass0 {
        VertexShader = compile vs_2_0 TerrainVS();
        PixelShader  = compile ps_2_0 TerrainPS();
    }
}
