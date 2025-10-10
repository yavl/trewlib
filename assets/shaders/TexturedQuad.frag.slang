struct UniformData {
    float4 multiplyColor;
};

[[vk::binding(0, 3)]]
ConstantBuffer<UniformData> ubo;

[[vk::binding(0, 2)]]
Texture2D texture;

SamplerState sampler;

[shader("fragment")]
float4 main(float2 texCoord : TEXCOORD0) : SV_Target0
{
    return ubo.multiplyColor * texture.Sample(sampler, texCoord);
}
