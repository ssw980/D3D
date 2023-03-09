#include "Framework.h"
#include "Model.h"
#include "Utilities/BinaryFile.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::ReadMesh(wstring file)
{
    file =  L"../../_Models/" + file + L".mesh";

    BinaryReader* r = new BinaryReader(file);

    //Load Bone
    UINT count = 0;
    count = r->UInt();
    for (UINT i = 0; i < count; i++)
    {
       ModelBone* bone = new ModelBone();

       bone->index = r->Int();
       bone->name = String::ToWString(r->String());
       bone->parentIndex = r->Int();
       bone->transform = r->Matrix();

        bones.push_back(bone);
    }

    //Load Mesh
    count = r->UInt();
    for (UINT i = 0; i < count; i++)
    {
        ModelMesh* mesh = new ModelMesh();
        
        mesh->boneIndex = r->Int();

        //Vertex Data
        {
            UINT count = r->UInt();

            vector<Model::VertexModel> vertices;
            vertices.assign(count, Model::VertexModel());

            void* ptr = (void*)&vertices[0];
            r->Byte(&ptr, sizeof(Model::VertexModel) * count);

            mesh->vertices = new Model::VertexModel[count];
            mesh->vertexCount = count;
            copy
            (
                vertices.begin(), vertices.end(),
                stdext::checked_array_iterator<Model::VertexModel*>(mesh->vertices, count)
            );
        }

        //Index Data
        {
            UINT count = r->UInt();

            vector<UINT> indices;
            indices.assign(count, UINT());

            void* ptr = (void*)&indices[0];
            r->Byte(&ptr, sizeof(UINT) * count);

            mesh->indices = new UINT[count];
            mesh->indexCount = count;
            copy
            (
                indices.begin(), indices.end(),
                stdext::checked_array_iterator<UINT*>(mesh->indices, count)
            );
        }

        UINT partCount = r->UInt();
        for (UINT k = 0; k < partCount; k++)
        {
            ModelMeshPart* meshPart = new ModelMeshPart();
            meshPart->materialName = String::ToWString(r->String());

            meshPart->startVertex = r->UInt();
            meshPart->vertexCount = r->UInt();

            meshPart->startIndex = r->UInt();
            meshPart->indexCount = r->UInt();

            mesh->meshParts.push_back(meshPart);
        }

        meshes.push_back(mesh);
    }

    SafeDelete(r);

    BindBone();
    BindMesh();
}

void Model::BindBone()
{
    root = bones[0];

    for (ModelBone* bone : bones)
    {
        if (bone->parentIndex > -1)
        {
            bone->parent = bones[bone->parentIndex];
            bone->parent->children.push_back(bone);
        }
        else
            bone->parent = nullptr;
    }
}

void Model::BindMesh()
{
    for (ModelMesh* mesh : meshes)
    {
        mesh->bone = bones[mesh->boneIndex];
        mesh->Binding(this);
    }
}

ModelBone* Model::BoneByName(wstring name)
{
    for (ModelBone* bone : bones)
    {
        if (name == bone->Name())
            return bone;
    }

    return nullptr;
}
