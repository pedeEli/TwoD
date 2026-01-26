struct Instance
{
    float4 color;
    float2 model1;
    float2 model2;
    float2 model3;
    bool inWorld;
    float padding;
};

StructuredBuffer<Instance> dataBuffer : register(t0, space0);

struct Output
{
    float4 position : SV_POSITION;
    float4 color : TEXCOORD0;
};

static const uint indices[6] = { 0, 1, 2, 3, 2, 1 };
static const float3 positions[4] =
{
    { -0.5f, -0.5f, 1.0f },
    { 0.5f, -0.5f, 1.0f },
    { -0.5f, 0.5f, 1.0f },
    { 0.5f, 0.5f, 1.0f }
};

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 projection;
    float4x4 projectionFixedZoom;
    float4x4 view;
}


Output main(uint id : SV_VertexID)
{
    const uint spriteIndex = id / 6;
    const uint index = indices[id % 6];
    const Instance data = dataBuffer[spriteIndex];
    
    const float3x3 model =
    {
        data.model1.x, data.model2.x, data.model3.x,
        data.model1.y, data.model2.y, data.model3.y,
        0.0f, 0.0f, 1.0f
    };
    const float4 position = float4(mul(model, positions[index]).xy, 0.0f, 1.0f);
    
    Output output;
    if (data.inWorld)
    {
        output.position = mul(projection, mul(view, position));
    }
    else
    {
        output.position = mul(projectionFixedZoom, position);
    }
    output.color = data.color;
    return output;
}