//入力用
struct vertexIn
{
	float4 pos : POSITION0;
	float4 nrm : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

//出力用
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

//変換用行列
cbuffer ConstantBuffer : register(b0)
{
	float4x4  World;		//ワールド変換行列
	float4x4 View;		//ビュー変換行列
	float4x4 Projection;		//プロジェクション変換行列
	float4x4 LView;		//ビュー変換行列
	float4x4 LProjection;		//プロジェクション変換行列
	float4 Light;		//ライト
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

	//光源処理
	float3 L = normalize(Light.xyz);

	//ワールド変換
	OUT.nrm = mul(IN.nrm, World);

	OUT.col = IN.col;
	OUT.tex = IN.tex;

	//光源と法線の内積を求める
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
