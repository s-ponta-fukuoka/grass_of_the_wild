//���͗p
struct vertexIn
{
	float4 pos : POSITION0;
	float4 nrm : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

//�o�͗p
struct vertexOut
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

//�ϊ��p�s��
cbuffer ConstantBuffer : register(b0)
{
	float4x4  World;		//���[���h�ϊ��s��
	float4x4 View;		//�r���[�ϊ��s��
	float4x4 Projection;		//�v���W�F�N�V�����ϊ��s��
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	float4 posWorld;
	posWorld = mul(IN.pos, World);

	OUT.pos = mul(mul(posWorld, View), Projection);

	OUT.tex = IN.tex;

	return OUT;
}
