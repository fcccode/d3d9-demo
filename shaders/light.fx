uniform extern float3   g_eye_pos;
uniform extern float4x4 g_world;
uniform extern float4x4 g_world_it;
uniform extern float4x4 g_wvp;

uniform extern float4 g_ambient_light;
uniform extern float4 g_diffuse_light;
uniform extern float4 g_specular_light;
uniform extern float3 g_light_direction;

uniform extern float4 g_ambient_mtrl;
uniform extern float4 g_diffuse_mtrl;
uniform extern float4 g_specular_mtrl;
uniform extern float  g_specular_power;

uniform extern texture g_texture;

sampler TexSampler = sampler_state {
    Texture   = <g_texture>;
    MinFilter = Anisotropic;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    MaxAnisotropy = 8;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

struct OutputVS {
    float4 pos      : POSITION0;
    float4 diffuse  : COLOR0;
    float4 specular : COLOR1;
    float2 tex      : TEXCOORD0;
};

OutputVS LightVS(float3 pos : POSITION0,
                 float3 normal : NORMAL0,
                 float2 tex: TEXCOORD0) {
    OutputVS output;
    output.pos = mul(float4(pos, 1.0f), g_wvp);
    output.tex = tex;

    float3 normal_world = mul(float4(normal, 0.0f), g_world_it).xyz;
    normal_world = normalize(normal_world);
    float3 pos_world  = mul(float4(pos, 1.0f), g_world).xyz;

    float3 to_eye = normalize(g_eye_pos - pos_world);
    float3 reflection = reflect(-g_light_direction, normal_world);

    float t  = pow(max(dot(reflection, to_eye), 0.0f), g_specular_power);
    float s = max(dot(g_light_direction, normal_world), 0.0f);

    float3 specular = t * (g_specular_mtrl * g_specular_light).rgb;
    float3 diffuse  = s * (g_diffuse_mtrl * g_diffuse_light).rgb;
    float3 ambient  = g_ambient_mtrl * g_ambient_light;

    output.diffuse.rgb = ambient + diffuse;
    output.diffuse.a   = g_diffuse_mtrl.a;
    output.specular = float4(specular, 0.0f);

    return output;
}

float4 LightPS(float4 diffuse  : COLOR0,
               float4 specular : COLOR1,
               float2 tex      : TEXCOORD0) : COLOR {
    float3 tex_color = tex2D(TexSampler, tex).rgb;
    return float4(diffuse.rgb * tex_color + specular.rgb, diffuse.a);
}

technique LightTech {
    pass Pass0 {
        VertexShader = compile vs_2_0 LightVS();
        PixelShader  = compile ps_2_0 LightPS();
    }
}
