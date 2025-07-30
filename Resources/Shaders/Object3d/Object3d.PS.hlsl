#include "Object3d.hlsli"


// �}�e���A��
struct Material
{
    float4 color;
    float4x4 uvTransform;
    uint textureIndex;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);

// �J����
struct ViewProjectionMatrix
{
    float4x4 view;
    float4x4 projection;
    float4x4 orthoGraphic;
    float3 cameraPosition;
};
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMat : register(b1);

// ���C�g
struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
    float shininess;
    int eneble;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);

// ���}�b�v
struct Environment
{
    float scale;
    int enable;
};
ConstantBuffer<Environment> gEnvironment : register(b3);
TextureCube<float4> gEnvironmentTexture : register(t1);

// �F���Z
struct ColorAddition
{
    float4 addColor;
    float intensity;
    int enable;
};
ConstantBuffer<ColorAddition> gColorAddition : register(b4);

// NormalMap
struct NormalMapSettings
{
    int enable;
};
ConstantBuffer<NormalMapSettings> gNormalMapSettings : register(b5);
Texture2D<float4> gNormalMap : register(t2);

// �f�J�[��
struct DecalSettings
{
    float4x4 decalMatrix; // �f�J�[���̈ʒu�A��]�A�X�P�[�������肷��s��
    float4 decalColor; // �f�J�[���̐F
    float decalStrength; // �f�J�[���̓K�p���x
    int enable;
};
ConstantBuffer<DecalSettings> gDecalSettings : register(b6);
Texture2D<float4> gDecalTexture : register(t3);


SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    // UV�ϊ���K�p
    float4 transformedUV = mul(float4(input.texCoord, 0.0f, 1.0f), gMaterial.uvTransform);

    // �e�N�X�`������F���T���v�����O
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    // �A���t�@�l��0.9f�����Ȃ�`����X�L�b�v
    if (textureColor.a == 0.0f)
    {
        discard;
    }
    
    float3 normal = normalize(input.normal);

    // --- Normal Map ---
    if (gNormalMapSettings.enable != 0)
    {
        float3 normalSample = gNormalMap.Sample(gSampler, transformedUV.xy).rgb;
        normalSample = normalSample * 2.0f - 1.0f; // [0,1] �� [-1,1]
        normal = normalize(normalSample);
    }
    
    // --- Lighting ---
    if (gDirectionalLight.eneble)
    {
        // ���C�g�̕����x�N�g���𐳋K��
        float3 lightDir = normalize(gDirectionalLight.direction);
        
        // �����x�N�g���̌v�Z
        float3 toEye = normalize(gViewProjectionMat.cameraPosition - input.worldPos);
        
        // ���t���N�g�x�N�g���̌v�Z
        float3 reflectLight = reflect(lightDir, normal);
        
        // �n�[�t�x�N�g���̌v�Z
        float3 halfVector = normalize(lightDir + toEye);
        
        // �g�U���˂̌v�Z
        float NdotL = dot(normal, lightDir);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        // ���ʔ��˂̌v�Z
        float NdotH = dot(normal, halfVector);
        float3 specularPow = pow(saturate(NdotH), gDirectionalLight.shininess);
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow;
       
        // �g�U���˂Ƌ��ʔ��˂̍���
        float3 finalColor = diffuse;
        
        // �e�N�X�`���̃A���t�@��0�̏ꍇ�̓s�N�Z����j��
        if (textureColor.a <= 0.5f)
        {
            discard;
        }
        
        // �F�̐ݒ�
        output.color.rgb = finalColor;
    }
    else
    {
        // ���C�e�B���O�������ȏꍇ�́A�e�N�X�`���̐F�Ƀ}�e���A���̐F���|�����킹��
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
    }
    
    // --- Decal ---
    if (gDecalSettings.enable != 0)
    {
        // ���[���h���W����f�J�[����Ԃւ̕ϊ�
        float4 decalPos = mul(float4(input.worldPos, 1.0f), gDecalSettings.decalMatrix);

        // �f�J�[��UV���W�̌v�Z (0.0�`1.0�͈̔͂ɐ��K��)
        float2 decalUV = decalPos.xy * 0.5f + 0.5f;

        // �f�J�[�����͈͊O�ł���΃X�L�b�v (�f�J�[���e�N�X�`���͈̔͊O�̃s�N�Z����`�悵�Ȃ�)
        if (decalUV.x < 0.0f || decalUV.x > 1.0f || decalUV.y < 0.0f || decalUV.y > 1.0f)
        {
            // �f�J�[�����K�p�����ׂ��̈�O�̃s�N�Z���͌��̐F��ێ�
        }
        else
        {
            // �f�J�[���e�N�X�`�����T���v�����O
            float4 decalSample = gDecalTexture.Sample(gSampler, decalUV);

            // �f�J�[���̃A���t�@�l�Ɋ�Â��Č��̐F�ƃf�J�[���̐F���u�����h
            // �Ђъ���\���̏ꍇ�A�f�J�[���e�N�X�`���̂Ђъ��ꕔ�����s���� (�A���t�@�l������) �ɂȂ�悤�ɂ���
            // �f�J�[���̐F (gDecalSettings.decalColor) �ƌ��̃I�u�W�F�N�g�̐F���A�f�J�[���e�N�X�`���̃A���t�@�l�ŕ�Ԃ���
            output.color.rgb = lerp(output.color.rgb, gDecalSettings.decalColor.rgb, decalSample.a * gDecalSettings.decalStrength);
        }
    }
    
    // --- Environment ---
    if (gEnvironment.enable)
    {
        float3 cameraToPos = normalize(input.worldPos - gViewProjectionMat.cameraPosition);
        float3 reflectedVector = reflect(cameraToPos, normalize(input.normal));
        float4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
        environmentColor *= gEnvironment.scale;
        output.color.rgb += environmentColor.rgb;
    }
    
    // --- ColorAddition ---
    if (gColorAddition.enable)
    {
        output.color.rgb += gColorAddition.addColor.rgb * gColorAddition.intensity;
    }

    // �A���t�@�l�̐ݒ�
    output.color.a = gMaterial.color.a * textureColor.a;

    return output;
}