#include "BasicShaderHeader.hlsli"
#include "Math.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> sph : register(t1);
Texture2D<float4> spa : register(t2);
Texture2D<float4> toon : register(t3);

SamplerState smp : register(s1);

cbuffer Material : register(b2)
{
	float4 diffuse; //�f�B�t���[�Y�J���[
	float4 specular; //�X�y�L�����[�J���[
	float3 ambient; //�A���r�G���g�J���[
};

float4 BasicPS(Output input) : SV_TARGET
{
	//���̕���
    float3 light = normalize(float3(1, -1, 1));
	//���C�g�J���[
    float3 lightColor = float3(1, 1, 1);
	
    float diffuseB = saturate(dot(-light, input.normal));
    float4 toonDif = toon.Sample(smp, float2(0, 1.0 - diffuseB));
	//���̔��˃x�N�g��
    float3 refLight = normalize(reflect(light, input.normal.xyz));
    float speculerB = pow(saturate(dot(refLight, -input.ray)), specular.a);
	
	//�X�t�B�A�}�b�vuv
    float2 sphereMatUV = (input.vnormal.xy + float2(1, -1) * float2(0.5, -0.5));
	
    float4 texColor = tex.Sample(smp, input.uv);
	
    return max(saturate(toonDif * diffuse * texColor * sph.Sample(smp, sphereMatUV))
	+ saturate(spa.Sample(smp, sphereMatUV)) * texColor + float4(speculerB * specular.rgb, 1), float4(texColor * ambient * 0.5, 1));
}