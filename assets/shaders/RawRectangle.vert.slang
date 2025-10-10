struct Input
{
    uint VertexIndex : SV_VertexID;
};

struct Output
{
    float4 Color : TEXCOORD0;
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
    float2 pos;
    
    // Create a rectangle using 6 vertices (2 triangles)
    if (input.VertexIndex == 0)
    {
        pos = float2(-1.0f, -1.0f); // bottom-left
        output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f); // red
    }
    else if (input.VertexIndex == 1)
    {
        pos = float2(1.0f, -1.0f); // bottom-right
        output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f); // green
    }
    else if (input.VertexIndex == 2)
    {
        pos = float2(-1.0f, 1.0f); // top-left
        output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f); // blue
    }
    else if (input.VertexIndex == 3)
    {
        pos = float2(-1.0f, 1.0f); // top-left
        output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f); // blue
    }
    else if (input.VertexIndex == 4)
    {
        pos = float2(1.0f, -1.0f); // bottom-right
        output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f); // green
    }
    else // input.VertexIndex == 5
    {
        pos = float2(1.0f, 1.0f); // top-right
        output.Color = float4(1.0f, 1.0f, 0.0f, 1.0f); // yellow
    }
    
    output.Position = mul(ubo.modelViewProjection, float4(pos, 0.0f, 1.0f));
    return output;
}