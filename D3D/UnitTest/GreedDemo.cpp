#include "stdafx.h"
#include "GreedDemo.h"

void GreedDemo::Initialize()
{
	shader = new Shader(L"05_World.fxo");

	vertexCount = (width +1) * (height +1);
	vertices = new Vertex[vertexCount];

	for (UINT y = 0; y <= height; y++)
	{
		for (UINT x = 0; x <= height; x++)
		{
			UINT index = (width + 1) * y + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = (float)y;
			vertices[index].Position.z = 0.f;
		}
	}

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	indexCount = width * height * 6;
	indices = new UINT [indexCount];

	UINT index = 0;
	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < height; x++)
		{
			indices[index + 0] = (width + 1) * y + x;
			indices[index + 1] = (width + 1) * (y+1) + x;
			indices[index + 2] = (width + 1) * y + (x + 1);
			indices[index + 3] = (width + 1) * y + (x + 1);
			indices[index + 4] = (width + 1) * (y + 1) + x;
			indices[index + 4] = (width + 1) * (y + 1) + x;


			index += 6;
		}
	}

	//create index buffe
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}
}

void GreedDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);		//delete x
	SafeRelease(indexBuffer);		//delete x

}

void GreedDemo::Update()
{
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void GreedDemo::Render()
{	//파이프라인 동작코드
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3D::GetDC()->IASetVertexBuffers(0,1, &vertexBuffer,&stride,&offset);

	shader->Draw(0,0,2);
}
