
//この構造体は、ジオメトリシェーダからの入力データです。
//頂点シェーダからワールド位置にデータを変換するだけです
struct geometryInput
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
	float fogFactor : FOG;
	//float3 posEye : POSITION_EYE;
	float3 posPlayer : TEXCOORD3;
};

struct geometryOut
{
	float4 pos : SV_POSITION;
	float3 nrm : NORMAL0;
	float4 col : COLOR;
	//float3 posEye : POSITION_EYE;
	float3 posW : POSITION_W;
};

[maxvertexcount(3)]
void main(
	triangle geometryInput input[3],
	inout TriangleStream< geometryOut > output)
{
	float _Length = 100;
	float _Width = 10;
	float _Gravity = 1;

	//入力データに直接アクセスするとコードが混乱する傾向があるため、
	//通常はすべてをクリーン変数に再パックします

	float4 P1 = input[0].pos;
	float4 P2 = input[0].pos;
	P2.x = input[0].pos.x - 1;
	P2.z = input[0].pos.z - 1;

	float4 N1 = input[0].nrm;

	float4 P = P1;
	float4 N = N1;
	float4 T = float4(float3(1,0,0), 0.0f);

	geometryOut element = (geometryOut)0;

	int steps = 2;

	//for (uint j = 0; j < 1; j++)
	{

		//for (uint i = 1; i < steps; i++)
		{
			float t0 = (float)0 / 1;
			float t1 = (float)(0 + 1) / 1;

			float4 p0 = normalize(N - (float4(0, _Length * 0, 0, 0))) * (_Length);
			float4 p1 = normalize(N - (float4(0, _Length * 1, 0, 0))) * (_Length);

			float4 w0 = T * _Width;
			float4 w1 = T * _Width;

			float4 pos;

			pos = input[0].pos;
			//pos.x += pos1.x;
			//pos.z += pos1.z;
			element.nrm = float3(0, 0, 1);
			//element.posEye = input[0].posEye;
			element.posW = mul(pos, input[0].World);
			element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			element.col = float4(0, 0.7, 0, 1);
			element.col.a = input[0].fogFactor;
			output.Append(element);

			pos = input[1].pos;
			element.posW = mul(pos, input[0].World);
			//pos.x += pos1.x;
			//pos.z += pos1.z;
			element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			element.col = float4(0, 0.7, 0, 1);
			element.col.a = input[0].fogFactor;
			output.Append(element);

			element.nrm = float3(0, 1, 1);
			pos = input[2].pos;
			element.posW = mul(pos, input[0].World);
			//pos.x += pos1.x;
			//pos.z += pos1.z;
			pos = mul(pos, input[0].World);
			//pos.x = pos.x * dis.x;
			//pos.z = pos.z * dis.z;
			float3 dis = normalize(pos - input[0].posPlayer);
			float fDis = distance(pos , input[0].posPlayer);

			if (fDis <= 110)
			{
				pos.x = pos.x + (dis.x * 100);
				pos.y = pos.y - (50);
				pos.z = pos.z + (dis.z * 100);
			}
			else
			{
				pos.x = pos.x + (sin(input[0].time) * 20) * -1;
				pos.z = pos.z - (sin(input[0].time) * 20) * 0;
			}

			element.pos = mul(mul(pos, input[0].View), input[0].Projection);
			element.col = float4(1, 1, 0, 1);
			element.col.a = input[0].fogFactor;
			output.Append(element);

			//pos = (p0 - w0);
			//pos.y *= -1;
			//element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			//element.col = float4(0, 0.5, 0, 1);
			//output.Append(element);
			//
			//pos = (p1 - w1);
			//pos.y *= -1;
			//pos.x = pos.x + (sin(input[0].time) * 10);
			//pos.z = pos.z + (sin(input[0].time) * 10);
			//element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			//element.col = float4(1, 1, 0, 1);
			//element.col.a = input[0].fogFactor;
			//output.Append(element);
			//
			//pos = (p0 + w0);
			//pos.y *= -1;
			//element.pos = mul(mul(mul(pos, input[0].World), input[0].View), input[0].Projection);
			//element.col = float4(0, 0.5, 0, 1);
			//element.col.a = input[0].fogFactor;
			//output.Append(element);

		}

		output.RestartStrip();
	}
}

int DecimalConversion(int nNumber, int nDecimal1, int nDecimal2) {
	int ans = 0;

	for (int i = 0; nNumber >0; i++)
	{
		ans = ans + (nNumber % nDecimal1) * pow(nDecimal2, i);
		nNumber = nNumber / nDecimal1;
	}

	return ans;
};

int GetReverseNumber(int nNumber)
{
	int nReverse = 0;

	while (nNumber > 0)
	{
		nReverse++;
		nNumber /= 10;
	}

	return nReverse;
};