//���͗p
struct vertexIn
{
	float4 pos : POSITION0;
	float4 nrm : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4 BoneIndices : BONEINDICES0;
	float4 weight : WEIGHT0;
};

//�o�͗p
struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float2 tex2 : TEX2;
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
	float4x4 Bones[100];
	float4x4 LclBone;
	float4 Light;
	//float4 Color;
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	float4x4 pos0 = 0;

	float4 pos = mul(IN.pos, LclBone);

	pos0 += Bones[int(IN.BoneIndices.x)] * IN.weight.x;
	pos0 += Bones[int(IN.BoneIndices.y)] * IN.weight.y;
	pos0 += Bones[int(IN.BoneIndices.z)] * IN.weight.z;
	pos0 += Bones[int(IN.BoneIndices.w)] * IN.weight.w;

	OUT.pos = mul(mul(mul(mul(pos0, pos), World), View), Projection);

	OUT.Spos = OUT.pos;

	float4 posL = mul(mul(pos0, pos), LProjection);
	posL = posL / posL.w;
	OUT.Lpos.x = (posL.x + 1.0) / 2.0;
	OUT.Lpos.y = (-posL.y + 1.0) / 2.0;
	OUT.Lpos.z = posL.z;

	//��������
	float3 L = normalize(Light.xyz);

	//���[���h�ϊ�
	float4 nrm = mul(IN.nrm, World * pos0);
	OUT.nrm = nrm;

	//���C�g�Ɩ@���̓���
	float lgtdot = dot(L, OUT.nrm);

	float3 eye = normalize(mul(View, OUT.pos));

	//�@���ƃJ�����̓���
	float eyedot = dot(eye, OUT.nrm);

	//OUT.nrm = mul(IN.nrm, World);

	OUT.col = float4(1,1,1,1);
	OUT.tex = IN.tex;
	OUT.tex2.x = lgtdot;
	OUT.tex2.y = eyedot;

	//�����Ɩ@���̓��ς����߂�
	OUT.col2 = dot(OUT.nrm, L);

	OUT.col2.a = 1.0f;

	return OUT;
}