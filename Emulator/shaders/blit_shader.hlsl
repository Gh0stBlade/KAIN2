#ifdef D3D9
#define SV_TARGET COLOR0
#endif

struct VS_INPUT {
#ifdef D3D9
	float4 a_position : POSITION;
	float4 a_texcoord : TEXCOORD0;
#else
	int4 a_position  : POSITION;
	uint4 a_texcoord : TEXCOORD0;
#endif
};

struct VS_OUTPUT {
#ifdef D3D9
	float4 v_position  : POSITION;
#else
	float4 v_position  : SV_POSITION;
#endif
	float4 v_texcoord  : TEXCOORD0;
};

#ifdef VERTEX
	VS_OUTPUT main(VS_INPUT In) {
		VS_OUTPUT Out;
		Out.v_texcoord = In.a_texcoord * float4(8.0 / 1024.0, 8.0 / 512.0, 0.0, 0.0);
		Out.v_position = float4(In.a_position.xy, 0.0, 1.0);
		return Out;
	}
#else

#ifdef D3D9
	SamplerState s_texture : register(s0);
#else
	SamplerState samplerState : register(s0);
	Texture2D s_texture : register(t0);
#endif

	float4 main(VS_OUTPUT In) : SV_TARGET {
#ifdef D3D9
		float2 color_rg = tex2D(s_texture, In.v_texcoord.xy).ra * 255.0;
#else
		float2 color_rg = s_texture.Sample(samplerState, In.v_texcoord.xy).rg * 255.0;
#endif
		float color_16 = color_rg.y * 256.0 + color_rg.x;
		float4 color = frac(floor(color_16 / float4(1.0, 32.0, 1024.0, 32768.0)) / 32.0);
		color.a = 1.0;

		return color;
	}
#endif
