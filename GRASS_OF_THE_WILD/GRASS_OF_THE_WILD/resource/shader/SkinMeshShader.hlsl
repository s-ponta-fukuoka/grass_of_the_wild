//���͗p
struct vertexIn
{
	float4 pos : POSITION0;
	float3 nrm : NORMAL0;
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
};

//�ϊ��p�s��
cbuffer ConstantBuffer : register(b0)
{
	matrix  World;		//���[���h�ϊ��s��
	matrix View;		//�r���[�ϊ��s��
	matrix Projection;		//�v���W�F�N�V�����ϊ��s��
	matrix Bones[100];
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;
	
	matrix pos0 = 0;

	float4 pos = mul(IN.pos, World);

	pos0 += Bones[int(IN.BoneIndices.x)] * IN.weight.x;
	pos0 += Bones[int(IN.BoneIndices.y)] * IN.weight.y;
	pos0 += Bones[int(IN.BoneIndices.z)] * IN.weight.z;
	pos0 += Bones[int(IN.BoneIndices.w)] * IN.weight.w;
	
	OUT.pos = mul(mul(mul(pos0, pos), View), Projection);

	//���[���h�ϊ�
	OUT.nrm = mul(IN.nrm, World);

	OUT.col = IN.col;
	OUT.tex = IN.tex;

	return OUT;
}