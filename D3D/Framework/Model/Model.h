#pragma once

#define MAX_MODEL_TRANSFORMS 250

class ModelBone;
class ModelMesh;
class ModelMeshPart;

class Model
{
public:
	struct VertexModel;
	friend class ModelRenderer;

private:
	Model();
	~Model();

private:
	void ReadMesh(wstring file);

private:
	void BindBone();
	void BindMesh();

public:
	UINT BoneCount() { return bones.size(); }
	vector<ModelBone*>& Bones() { return bones; }
	ModelBone* BoneByIndex(UINT index) { return bones[index]; }
	ModelBone* BoneByName(wstring name);

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh*>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }

public:
	struct VertexModel
	{
		Vector3 Position;
		Vector2 Uv;
		Vector3 Normal;
		Vector3 Tangent;
		Vector4 BlendIndices;
		Vector4 BlendWeights;

		VertexModel()
		{
			Position = Vector3(0, 0, 0);
			Uv = Vector2(0, 0);
			Normal = Vector3(0, 0, 0);
			BlendIndices = Vector4(0, 0, 0, 0);
			BlendWeights = Vector4(0, 0, 0, 0);
		}
	};


private:
	ModelBone* root;

	vector<ModelBone*> bones;
	vector<ModelMesh*> meshes;
};
