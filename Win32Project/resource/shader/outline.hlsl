struct GS_INPUT
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
struct PS_INPUT
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

// ジオメトリシェーダで輪郭線判定、ライン出力
[maxvertexcount(10)]
void main(lineadj GS_INPUT input[4],
	uint primID : SV_PrimitiveID,
	inout LineStream<PS_INPUT> Stream)
{
	float3 v0 = input[1].pos.xyz / input[1].pos.w;
	float3 v1 = input[2].pos.xyz / input[2].pos.w;
	float3 va = input[0].pos.xyz / input[0].pos.w;
	float3 vb = input[3].pos.xyz / input[3].pos.w;
	//ポリゴン v0-v1-vaとv1-v0-vbの方向を調べライン描画	
	float3 v0a = v0 - va;
	float3 v1a = v1 - va;
	float3 v0b = v0 - vb;
	float3 v1b = v1 - vb;

	if (0 > cross(v0a, v1a).z * cross(v1b, v0b).z)
	{
		PS_INPUT vo0 = (PS_INPUT)0;
		PS_INPUT vo1 = (PS_INPUT)0;
		float4 p0 = input[1].pos;
		float4 p1 = input[2].pos;
		vo0.pos = p0;
		vo1.pos = p1;
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();

		//一本だと細いので上下左右にずらして描画
		const float px = 0.8*p0.w / 640.0;
		const float py = 0.8*p1.w / 480.0;

		vo0.pos = p0 + float4(px, 0, 0, 0);
		vo1.pos = p1 + float4(px, 0, 0, 0);
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();

		vo0.pos = p0 + float4(-px, 0, 0, 0);
		vo1.pos = p1 + float4(-px, 0, 0, 0);
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();

		vo0.pos = p0 + float4(0, py, 0, 0);
		vo1.pos = p1 + float4(0, py, 0, 0);
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();

		vo0.pos = p0 + float4(0, -py, 0, 0);
		vo1.pos = p1 + float4(0, -py, 0, 0);
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();

	}

	Stream.RestartStrip();
}