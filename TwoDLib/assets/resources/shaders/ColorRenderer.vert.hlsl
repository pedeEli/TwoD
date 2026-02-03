struct Vertex
{
    float2 pos : POSITION0;
    float2 tex : TEXCOORD0;
    float4 color : COLOR0;
};

struct Output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 projectionView;
}


Output main(Vertex vertex)
{
    Output output;
    output.pos = mul(projectionView, float4(vertex.pos, 0.0f, 1.0f));
    output.color = vertex.color;
    return output;
}