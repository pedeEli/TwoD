Texture2D<float4> texture: register(t0, space2);
SamplerState samplr : register(s0, space2);


struct Input
{
    float2 tex : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(Input input) : SV_Target0
{
    return input.color * texture.Sample(samplr, input.tex);
}