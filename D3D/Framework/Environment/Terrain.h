#pragma once

class Terrain
{
public:
	Terrain(Shader* shader, wstring heighMapFile);
	~Terrain();

	void Update();
	void Render();

	UINT& Pass() { return pass; }

	void BaseMap(wstring file);

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();
	void CreateBuffers();

private:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector3 Normal = Vector3(0, 1, 0);
		Vector2 Uv = Vector2(0, 0);
	};

private:
	Shader* shader;
	UINT pass = 0;

	UINT width, height;

	UINT vertexCount;
	VertexTerrain* vertices;
	ID3D11Buffer* vertexBuffer;

	UINT indexCount;
	UINT* indices;
	ID3D11Buffer* indexBuffer;

	Matrix world;

	Texture* heightMap;
	Texture* baseMap = nullptr;
};