struct Input
{
    float4 color : COLOR0;
};

float4 main(Input input) : SV_Target0
{
    return input.color;
}