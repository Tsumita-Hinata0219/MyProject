struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOSITION;
    float2 decalUV : TEXCOORD;
};

// �J������� (�ʏ�̃I�u�W�F�N�g�Ɠ������̂��g�p)
struct ViewProjectionMatrix
{
    float4x4 view;
    float4x4 projection;
    float4x4 orthoGraphic;
    float3 cameraPosition;
};

// �f�J�[���ݒ�
struct DecalSettings
{
    float4x4 decalMatrix; // ���[���h��Ԃ���f�J�[����Ԃւ̕ϊ��s�� (�ʏ�̓f�J�[�����[���h�s��̋t�s��)
    float4 decalColor; // �f�J�[���̐F
    float decalStrength; // �f�J�[���̓K�p���x
    int enable;
};