struct Input
{
    float4 Position : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
    float4 Color : TEXCOORD2;
};

struct Output
{
    float2 TexCoord : TEXCOORD0;
    float4 Color : TEXCOORD1;
    float4 Position : SV_Position;
};

struct UniformData {
    float4x4 modelViewProjection;
};

[[vk::binding(0, 1)]]
ConstantBuffer<UniformData> ubo;

[shader("vertex")]
Output main(Input input)
{
    Output output;
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    output.Position = mul(ubo.modelViewProjection, input.Position);
    return output;
}