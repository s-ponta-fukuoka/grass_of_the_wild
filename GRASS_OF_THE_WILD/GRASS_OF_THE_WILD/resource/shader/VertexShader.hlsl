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
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float4 Lpos : POSITION_SM;
	float4 Spos : TEXCOORD2;
};

//�ϊ��p�s��
cbuffer ConstantBuffer : register(b0)
{
	float4x4  World;		//���[���h�ϊ��s��
	float4x4 View;		//�r���[�ϊ��s��
	float4x4 Projection;		//�v���W�F�N�V�����ϊ��s��
	float4x4 LView;		//�r���[�ϊ��s��
	float4x4 LProjection;		//�v���W�F�N�V�����ϊ��s��
	float4 Light;		//���C�g
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	float4 posWorld;
	posWorld = mul(IN.pos, World);

	OUT.pos = mul(mul(posWorld, View), Projection);

	OUT.Spos = OUT.pos;

	float4x4 LightWVP = mul(LView, LProjection);

	float4 posL = mul(IN.pos, LProjection);
	posL = posL / posL.w;
	OUT.Lpos.x = (posL.x + 1.0) / 2.0;
	OUT.Lpos.y = (-posL.y + 1.0) / 2.0;
	OUT.Lpos.z = posL.z;

	//matrix LightWVP = mul(LView, LProjection);
	//float4 posL = mul(mul(IN.pos, World), LightWVP);
	//OUT.Lpos = posL;

	//��������
	float3 L = normalize(Light.xyz);

	//���[���h�ϊ�
	OUT.nrm = mul(IN.nrm, World);

	OUT.col = IN.col;
	OUT.tex = IN.tex;

	//�����Ɩ@���̓��ς����߂�
	OUT.col2 = dot(OUT.nrm, L);

	OUT.col2.a = 1.0f;

	return OUT;
}
