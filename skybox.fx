uniform extern float4x4 g_wvp;
uniform extern texture  g_envmap;

sampler EnvMapSampler = sampler_state {
    Texture   = <g_envmap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

struct OutputVS {
    float4 pos : POSITION0;
    float3 tex : TEXCOORD0;
};

OutputVS SkyVS(float3 pos : POSITION0) {
    OutputVS output;

    output.pos = mul(float4(pos, 1.0f), g_wvp).xyww;
    output.tex = pos;

    return output;
}

float4 SkyPS(float3 tex : TEXCOORD0) : COLOR {
    return texCUBE(EnvMapSampler, tex);
}

technique SkyTech {
    pass Pass0 {
        VertexShader  = compile vs_2_0 SkyVS();
        PixelShader   = compile ps_2_0 SkyPS();
        CullMode      = None;
        ZFunc         = Always;
        StencilEnable = true;
        StencilFunc   = Always;
        StencilPass   = Replace;
        StencilRef    = 0;
    }
}
