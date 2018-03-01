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
	float fogFactor : FOG;
	float3 posEye : TEXCOORD3;
	float3 posW : TEXCOORD4;
	float Depth : TEXCOORD5;
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

	//��������
	float3 L = normalize(Light.xyz);

	//���[���h�ϊ�
	OUT.nrm = mul(IN.nrm, World);

	OUT.col = IN.col;
	OUT.tex = IN.tex;

	//�����Ɩ@���̓��ς����߂�
	OUT.col2 = dot(OUT.nrm, L);

	OUT.col2.a = 1.0f;
	
	float4 cameraPosition;

	// Calculate the camera position.
	cameraPosition = mul(IN.pos, World);
	cameraPosition = mul(cameraPosition, View);

	// Calculate linear fog.    
	OUT.fogFactor = saturate((5000 - cameraPosition.z) / (5000 - 0));

	OUT.posEye = cameraPosition;
	OUT.posW = posWorld;


	OUT.Depth = (length(posL.xyz - posWorld.xyz) / 100);

	return OUT;
}
