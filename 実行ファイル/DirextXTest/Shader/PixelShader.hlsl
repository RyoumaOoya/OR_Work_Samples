#include "BasicShaderHeader.hlsli"
#include "Math.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> sph : register(t1);
Texture2D<float4> spa : register(t2);
Texture2D<float4> toon : register(t3);

SamplerState smp : register(s1);

cbuffer Material : register(b2)
{
	float4 diffuse; //ディフューズカラー
	float4 specular; //スペキュラーカラー
	float3 ambient; //アンビエントカラー
};

float4 BasicPS(Output input) : SV_TARGET
{
	//光の方向
    float3 light = normalize(float3(1, -1, 1));
	//ライトカラー
    float3 lightColor = float3(1, 1, 1);
	
    float diffuseB = saturate(dot(-light, input.normal));
    float4 toonDif = toon.Sample(smp, float2(0, 1.0 - diffuseB));
	//光の反射ベクトル
    float3 refLight = normalize(reflect(light, input.normal.xyz));
    float speculerB = pow(saturate(dot(refLight, -input.ray)), specular.a);
	
	//スフィアマップuv
    float2 sphereMatUV = (input.vnormal.xy + float2(1, -1) * float2(0.5, -0.5));
	
    float4 texColor = tex.Sample(smp, input.uv);
	
    return max(saturate(toonDif * diffuse * texColor * sph.Sample(smp, sphereMatUV))
	+ saturate(spa.Sample(smp, sphereMatUV)) * texColor + float4(speculerB * specular.rgb, 1), float4(texColor * ambient * 0.5, 1));
}