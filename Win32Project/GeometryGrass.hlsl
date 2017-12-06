
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
	float4x4  World : WORLD;
	float4x4 View : VIEW;
	float4x4 Projection : PROJECTION;
};

struct geometryOut
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
};

[maxvertexcount(80)]
void main(
	point geometryInput input[1],
	inout TriangleStream< geometryOut > output)
{
	float _Length = 200;
	float _Width = 5;
	float _Gravity = 0.5;

	//入力データに直接アクセスするとコードが混乱する傾向があるため、
	//通常はすべてをクリーン変数に再パックします

	float4 P1 = input[0].pos;
	float4 P2 = input[0].pos;
	P2.x = input[0].pos.x - 1;

	float4 N1 = input[0].nrm;

	float4 P = P1;
	float4 N = N1;
	float4 T = float4(normalize(P2 - P1).xyz, 0.0f);

	geometryOut element = (geometryOut)0;

	int steps = 2;

	for (uint j = 1; j < 2; j++)
	{
		for (uint i = 1; i < steps; i++)
		{
			float t0 = (float)i / steps;
			float t1 = (float)(i + 1) / steps;

			float4 p0 = normalize(N - (float4(0, _Length * t0, 0, 0) * _Gravity * t0)) * (_Length * t0);
			float4 p1 = normalize(N - (float4(0, _Length * t1, 0, 0) * _Gravity * t1)) * (_Length * t1);

			float4 w0 = T * lerp(_Width, 0, t0);
			float4 w1 = T * lerp(_Width, 0, t1);

			float4 pos;

			pos = (p0 - w0);
			pos.y *= -1;
			element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			element.col = float4(0, 0.5, 0, 1);
			output.Append(element);

			pos = (p0 + w0);
			pos.y *= -1;
			element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			element.col = float4(0, 0.5, 0, 1);
			output.Append(element);

			pos = (p1 - w1);
			pos.y *= -1;
			element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			element.col = float4(1, 1, 0, 1);
			output.Append(element);

			pos = (p0 - w0);
			pos.y *= -1;
			element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			element.col = float4(0, 0.5, 0, 1);
			output.Append(element);

			pos = (p1 - w1);
			pos.y *= -1;
			element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			element.col = float4(1, 1, 0, 1);
			output.Append(element);

			pos = (p0 + w0);
			pos.y *= -1;
			element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			element.col = float4(0, 0.5, 0, 1);
			output.Append(element);

		}

		output.RestartStrip();
	}
}