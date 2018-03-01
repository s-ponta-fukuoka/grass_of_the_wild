//���͗p
struct vertexIn
{
	float4 pos : POSITION0;
	float4 nrm : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4x4 mat : MATRIX;
	uint InstanceId : SV_InstanceID;
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
	float2 fogFactor : FOG;
};

//�ϊ��p�s��
cbuffer ConstantBuffer : register(b0)
{
	float4x4 View;			//�r���[�ϊ��s��
	float4x4 Projection;		//�v���W�F�N�V�����ϊ��s��
	float time;
	float3 posPlayer;
	float3 posEye;
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	float4 posWorld;
	posWorld = IN.pos;

	OUT.pos = mul(mul(mul(IN.pos, IN.mat), View), Projection);

	OUT.Spos = OUT.pos;

	//���[���h�ϊ�
	OUT.nrm = normalize(mul(float4(IN.nrm.xyz, 0.0f), IN.mat));

	OUT.col = float4(1,1,1,1);
	OUT.tex = IN.tex;

	float4 cameraPosition;

	// Calculate the camera position.
	cameraPosition = mul(IN.pos, IN.mat);
	cameraPosition = mul(cameraPosition, View);

	// Calculate linear fog.    
	OUT.fogFactor.x = saturate(1.0 - (cameraPosition.z - 200) / (0 - 200));

	OUT.fogFactor.y = saturate((cameraPosition.z - 5000) / (0 - 5000));

	return OUT;
}
