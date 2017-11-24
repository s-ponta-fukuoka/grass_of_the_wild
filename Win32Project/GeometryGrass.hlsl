
//この構造体は、ジオメトリシェーダからの入力データです。
//頂点シェーダからワールド位置にデータを変換するだけです
struct geometryInput
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float4 Lpos : POSITION_SM;
	float4 Spos : TEXCOORD2;
	float4x4 View;
	float4x4 Projection;
};

struct geometryOut
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float4 Lpos : POSITION_SM;
	float4 Spos : TEXCOORD2;
};

[maxvertexcount(39)]
void main(
	triangle geometryInput input[3] : SV_POSITION,
	inout TriangleStream< geometryOut > output
)
{
	float _Length = 100;
	float _Width = 30;
	float _Gravity = 1;

	//入力データに直接アクセスするとコードが混乱する傾向があるため、
	//通常はすべてをクリーン変数に再パックします

	float4 P1 = input[0].pos;
	float4 P2 = input[1].pos;
	float4 P3 = input[2].pos;

	float4 N1 = input[0].nrm;
	float4 N2 = input[1].nrm;
	float4 N3 = input[2].nrm;

	float4 P = (P1 + P2 + P3) / 3.0f;
	float4 N = (N1 + N2 + N3) / 3.0f;
	float4 T = float4(normalize(P2 - P1).xyz, 0.0f);

	geometryOut element = (geometryOut)0;

	int steps = 5;

	for (uint i = 0; i < steps; i++)
	{

		float t0 = (float)i / steps;
		float t1 = (float)(i + 1) / steps;
		
		float4 p0 = normalize(N - (float4(0, _Length * t0, 0, 0) * _Gravity * t0)) * (_Length * t0);
		float4 p1 = normalize(N - (float4(0, _Length * t1, 0, 0) * _Gravity * t1)) * (_Length * t1);
		
		float4 w0 = T * lerp(_Width, 0, t0);
		float4 w1 = T * lerp(_Width, 0, t1);

		element.pos = mul(mul((p0 - w0), View), Projection);
		output.Append(element);

		element.pos = mul(mul((p0 + w0), View), Projection);
		output.Append(element);

		element.pos = mul(mul((p1 - w1), View), Projection);
		output.Append(element);

		element.pos = mul(mul((p1 + w1), View), Projection);
		output.Append(element);
	}

	output.RestartStrip();
}