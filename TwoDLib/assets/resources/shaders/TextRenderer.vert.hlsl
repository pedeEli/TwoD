struct Instance
{
    float4 color;
    float2 texMin;
    float2 texMax;
    float2 quadMin;
    float2 quadMax;
};

StructuredBuffer<Instance> dataBuffer : register(t0, space0);

struct Output
{
    float4 position : SV_POSITION;
    float4 color : TEXCOORD0;
    float2 texCoord : TEXCOORD1;
};

static const uint indices[6] = { 0, 1, 2, 3, 2, 1 };

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 projection;
    float4x4 view;
    float4x4 model;
}


Output main(uint id : SV_VertexID)
{
    uint spriteIndex = id / 6;
    uint index = indices[id % 6];
    Instance data = dataBuffer[spriteIndex];
    
    const float2 texCoords[4] =
    {
        data.texMin,
        { data.texMax.x, data.texMin.y },
        { data.texMin.x, data.texMax.y },
        data.texMax
    };
    
    const float2 positions[4] =
    {
        data.quadMin,
        { data.quadMax.x, data.quadMin.y },
        { data.quadMin.x, data.quadMax.y },
        data.quadMax
    };
    
    Output output;
    output.position = mul(projection, mul(view, mul(model, float4(positions[index], 0.0f, 1.0f))));
    output.texCoord = texCoords[index];
    output.color = data.color;
    return output;
}