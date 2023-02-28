#include "stdafx.h"
#include "VertexLineDemo.h"

void VertexLineDemo::Initialize()
{
	shader = new Shader(L"01.Line.fx");


	vertices[0].Position = Vector3(0, 0, 0);
	vertices[1].Position = Vector3(1, 0, 0);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vector3) * 2;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA

	D3D::GetDevice()->CreateBuffer(&desc,,&vertexBuffer)
}

void VertexLineDemo::Destroy()
{

}

void VertexLineDemo::Update()
{

}

void VertexLineDemo::Render()
{

}
