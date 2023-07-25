#include "BasicShaderHeader.hlsli"

Output BasicVS(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint2 boneno : BONE_NO, min16uint weight : WEIGHT)
{
    Output output; //�s�N�Z���V�F�[�_�֓n���l
    pos = mul(world, pos);
    output.svpos = mul(mul(proj, view), pos); //�V�F�[�_�ł͗�D��Ȃ̂Œ���
    output.pos = mul(view, pos);
    normal.w = 0; //�����d�v(���s�ړ������𖳌��ɂ���)
    output.normal = mul(world, normal); //�@���ɂ����[���h�ϊ����s��
    output.vnormal = mul(view, output.normal);
    output.uv = uv;
    output.ray = normalize(pos.xyz - mul(view, eye)); //�����x�N�g��
    return output;
}