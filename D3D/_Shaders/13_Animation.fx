#include "00_Global.fx"
#include "00_Light.fx"

float3 LightDirection = float3(-1, -1, 1);

struct VertexModel
{
	float4 Position : Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
	float4 BlendIndices : BlendIndices;
	float4 BlendWeights : BlendWeights;
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
};

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
cbuffer CB_Bones
{
	Matrix Transforms[MAX_MODEL_TRANSFORMS];
	uint BoneIndex;
}
struct AnimationFrame
{
    int Clip;

    uint CurrFrame ;
    uint NextFrame;

    float Time;
    float RunningTime;

    float Speed;
};

cbuffer CB_Keyframes
{
    AnimationFrame AnimationFrames;
	
}

Texture2DArray TransformsMap;

void SetAnimationWorld(inout matrix world,VertexModel input)
{
    float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
    float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
	
    int clip;
    int currFrame , nextFrame;
    float time;
	
    clip = AnimationFrames.Clip;
    currFrame = AnimationFrames.CurrFrame;
    nextFrame = AnimationFrames.NextFrame;
    time = AnimationFrames.Time;
	
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix transform = 0;
    matrix curr = 0, next = 0;
    matrix currAnim = 0;
	
	[unroll(4)]
    for (int i = 0; i < 4; i ++)
    {
        c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame, clip, 0));
        c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame, clip, 0));
        c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame, clip, 0));
        c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame, clip, 0));
        curr = matrix(c0, c1, c2, c3);
        
        n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame, clip, 0));
        n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame, clip, 0));
        n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame, clip, 0));
        n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame, clip, 0));
        next = matrix(n0, n1, n2, n3);
        
        currAnim = lerp(curr, next, time);

        transform += mul(weights[i], currAnim);
    }
	
    world = mul(transform, world);
}

VertexOutput VS(VertexModel input)
{
	VertexOutput output;
	
	//World = mul(Transforms[BoneIndex], World);
    SetAnimationWorld(World, input);
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Normal = WorldNormal(input.Normal);
	output.Uv = input.Uv;
	
	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float lambert = dot(normal, -LightDirection);
	float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	
	return diffuse * lambert;
}

technique11 T0
{
	P_VP(P0, VS, PS)
	P_RS_VP(P1, FillMode_WireFrame, VS, PS)
}