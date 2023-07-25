struct Output
{
	float4 svpos : SV_POSITION;
    float4 pos : POSITION;
	float4 normal : NORMAL0;
    float4 vnormal : NORMAL1;
	float2 uv : TEXCOORD;
    float3 ray : VECTOR;
};


cbuffer cbuff0 : register(b0)
{
    matrix view;
    matrix proj;
    float3 eye;
};

cbuffer Transform : register(b1)
{
    matrix world; //ÉèÅ[ÉãÉhïœä∑çsóÒ
}

//struct MatricesData
//{
//    matrix world;
//    matrix viewproj;
//};

//ConstantBuffer<Matrixf> m : register(b0);