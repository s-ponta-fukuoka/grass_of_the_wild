//入力用
struct vertexIn
{
	float4 pos : POSITION0;
	float4 nrm : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4x4 mat : MATRIX;
	uint InstanceId : SV_InstanceID;
};

//出力用
struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float4 Spos : TEXCOORD2;
	float4x4  World : WORLD;
	float4x4 View : VIEW;
	float4x4 Projection : PROJECTION;
	float time : TIME;
	float2 fogFactor : FOG;
	float3 posEye : POSITION_EYE;
	float3 posPlayer : TEXCOORD3;
};

//変換用行列
cbuffer ConstantBuffer : register(b0)
{
	float4x4 View;		//ビュー変換行列
	float4x4 Projection;		//プロジェクション変換行列
	float time;
	float3 posPlayer;
	float3 posEye;
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	float4 posWorld;
	posWorld = IN.pos;

	OUT.pos = posWorld;

	OUT.time = time;

	OUT.World = IN.mat;

	OUT.posPlayer = posPlayer;

	OUT.View = View;
	OUT.Projection = Projection;

	OUT.Spos = OUT.pos;

	//float4x4 LightWVP = mul(LView, LProjection);
	//
	//float4 posL = mul(IN.pos, LProjection);
	//posL = posL / posL.w;
	//OUT.Lpos.x = (posL.x + 1.0) / 2.0;
	//OUT.Lpos.y = (-posL.y + 1.0) / 2.0;
	//OUT.Lpos.z = posL.z;

	//光源処理
	//float3 L = normalize(Light.xyz);

	//ワールド変換

	OUT.nrm = normalize(mul(float4(IN.nrm.xyz, 0.0f), IN.mat));

	OUT.col = IN.col;
	OUT.tex = IN.tex;

	//光源と法線の内積を求める
	//OUT.col2 = dot(OUT.nrm, L);

	//OUT.col2.a = 1.0f;

	float4 cameraPosition;

	// Calculate the camera position.
	cameraPosition = mul(IN.pos, IN.mat);
	cameraPosition = mul(cameraPosition, View);

	// Calculate linear fog.    
	OUT.fogFactor.x = saturate(1.0f - (200 - cameraPosition.z) / (200 - 0));
	OUT.fogFactor.y = saturate((2000 - cameraPosition.z) / (2000 - 0));
	
	OUT.posEye = cameraPosition;

	return OUT;
}
