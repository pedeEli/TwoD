Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

struct Input
{
    float4 color : TEXCOORD0;
    float2 texCoord : TEXCOORD1;
};

const static float pxRange = 4.0;

float2 sqr(float2 x)
{
    return x * x;
}

float screenPxRange(float2 texCoord)
{
    const float2 unitRange = float2(pxRange, pxRange) / float2(270.0f, 270.0f);
    // If inversesqrt is not available, use vec2(1.0)/sqrt
    const float2 screenTexSize = rsqrt(sqr(ddx(texCoord)) + sqr(ddy(texCoord)));
    // Can also be approximated as screenTexSize = vec2(1.0)/fwidth(texCoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float4 main(Input input) : SV_TARGET0
{
	//return float4(Texture.Sample(Sampler, input.texCoord).agb, 1.0f);
    const float3 msd = Texture.Sample(Sampler, input.texCoord).agb;
    const float sd = median(msd.r, msd.g, msd.b);
    const float screenPxDistance = screenPxRange(input.texCoord) * (sd - 0.5);
    const float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    return float4(input.color.rgb, input.color.a * opacity);
}