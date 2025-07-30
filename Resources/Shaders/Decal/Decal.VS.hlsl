#include "Decal.hlsli"

// �J������� 
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMat : register(b0);

// �f�J�[���ݒ�
ConstantBuffer<DecalSettings> gDecalSettings : register(b1);

struct DecalVertexShaderInput
{
    float3 position : POSITION; 
};

VertexShaderOutput main(DecalVertexShaderInput input)
{
    VertexShaderOutput output;
    
    float4x4 decalWorldMatrix = gDecalSettings.decalMatrix; // <-- �������C��
    
    // �f�J�[���{�b�N�X�̒��_���W�����[���h��Ԃ֕ϊ�
    output.worldPos = mul(float4(input.position, 1.0f), decalWorldMatrix).xyz;

    // ���[���h���W���r���[�v���W�F�N�V������Ԃ֕ϊ�
    output.position = mul(float4(output.worldPos, 1.0f), gViewProjectionMat.view);
    output.position = mul(output.position, gViewProjectionMat.projection);

    // �f�J�[���e�N�X�`����UV���W���v�Z
    // �f�J�[���{�b�N�X�̃��[�J�����W (-1.0f ���� 1.0f) �� 0.0f ���� 1.0f ��UV�͈͂ɕϊ�
    output.decalUV = input.position.xy * 0.5f + 0.5f;

    return output;
}