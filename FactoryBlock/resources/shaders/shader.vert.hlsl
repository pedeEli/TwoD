struct SpriteData
{
    float4 color;
    float texU, texV, texW, texH;
    float2 model1;
    float2 model2;
    float2 model3;
    float2 padding;
};

StructuredBuffer<SpriteData> dataBuffer : register(t0, space0);

struct Output
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

static const uint indices[6] = { 0, 1, 2, 3, 2, 1 };
static const float3 positions[4] =
{
    { -0.5f, -0.5f, 1.0f },
    {  0.5f, -0.5f, 1.0f },
    { -0.5f,  0.5f, 1.0f },
    {  0.5f,  0.5f, 1.0f }
};

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 projection;
    float4x4 view;
    float2 atlasSize;
}


Output main(uint id : SV_VertexID)
{
    uint spriteIndex = id / 6;
    uint index = indices[id % 6];
    SpriteData data = dataBuffer[spriteIndex];
    
    float texelOffsetU = 0.5f / atlasSize.x;
    float texelOffsetV = 0.5f / atlasSize.y;
    
    float2 texCoords[4] = {
        { data.texU + texelOffsetU,              data.texV + texelOffsetV },
        { data.texU + data.texW - texelOffsetU,  data.texV + texelOffsetV },
        { data.texU + texelOffsetU,              data.texV + data.texH - texelOffsetV },
        { data.texU + data.texW - texelOffsetU,  data.texV + data.texH - texelOffsetV }
    };
    
    float3x3 model =
    {
      data.model1.x, data.model2.x, data.model3.x,  
      data.model1.y, data.model2.y, data.model3.y,
      0.0f,          0.0f,          1.0f
    };
    float2 position = mul(model, positions[index]).xy;
    
    Output output;
    output.position = mul(projection, mul(view, float4(position, 0.0f, 1.0f)));
    output.texCoord = texCoords[index];
    output.color = data.color;
    return output;
}