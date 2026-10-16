#include"Object3d.hlsli"

struct TransformationMatrix{
    float32_t4x4 wvp;
    float32_t4x4 World;
};

struct Material{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
//ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix.wvp);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrix.World));
    
    return output;
}