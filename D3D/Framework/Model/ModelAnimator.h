#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	void Update();
	void Render();
private:
	void UpdateAnimationFrame();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file);

	void PlayTweenMode(UINT clip,float speed = 1.f, float takeTime = 1.f);

public:
	void SetShader(Shader* shader, bool bFirst = false);
	void Pass(UINT pass);
	Transform* GetTransform() { return transform; }
	Model* GetModel() { return model; }

private:
	void CreateTexture();
	void CreateClipTransform(UINT index);

private:
	struct ClipTransform
	{
		Matrix** Transforms;

		ClipTransform()
		{
			Transforms = new Matrix*[MAX_MODEL_KEYFRAMES];

			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
				Transforms[i] = new Matrix[MAX_MODEL_TRANSFORMS];
		}

		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
				SafeDeleteArray(Transforms[i]);

			SafeDeleteArray(Transforms);
		}
	};
	ClipTransform* clipTransforms;

private:
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* transformsSRV;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV;

private:
	struct KeyFrameDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;
		UINT NextFrame = 0;

		float Time = 0.f;
		float RunningTime = 0.f;

		float Speed = 1.f;

		Vector2 Padding;
	};

private:
	struct TweenDesc
	{
		float TakeTime = 1.f;
		float TweenTime = 0.f; 
		float ChangetTime = 0.f;
		float Padding;

		KeyFrameDesc Curr;
		KeyFrameDesc Next;
		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;
		}
	}tweenDesc;


	
	ConstantBuffer* frameBuffer;
	ID3DX11EffectConstantBuffer* sFrameBuffer;

private:
	Shader* shader;
	Model* model;

	Transform* transform;
};
