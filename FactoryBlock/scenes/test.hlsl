Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

struct Input
{
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

float4 main(Input input) : SV_Target0
{
    const float distance = Texture.Sample(Sampler, input.texCoord).a - 0.5;
    const float w = fwidth(distance);
    const float alpha = smoothstep(-w, w, distance);
    return float4(input.color.rgb, input.color.a * alpha);
}