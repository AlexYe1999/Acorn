#pragma once
#include<vector>
#include<utility>
#include<windows.h>
#include<DirectXMath.h>
#include<DirectXPackedVector.h>

namespace LemonCube{
using namespace DirectX;

class GeometryGenerator{
public:
    struct Vertex{
        Vertex() = default;
        Vertex(
            XMFLOAT3 &&p, XMFLOAT3 &&n,
            XMFLOAT3 &&t, XMFLOAT2 &&_uv
        ) : 
            position(std::forward<XMFLOAT3>(p)),
            normal(std::forward<XMFLOAT3>(n)),
            tangentU(std::forward<XMFLOAT3>(t)),
            uv(std::forward<XMFLOAT2>(_uv))
        {}
        Vertex(
            float x, float y, float z,
            float nx, float ny, float nz,
            float tx, float ty, float tz,
            float u, float v
        ) : 
            position(x, y, z), normal(nx, ny, nz),
            tangentU(tx, ty, tz), uv(u, v)
        {}         
        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT3 tangentU;
        XMFLOAT2 uv;
    };

    struct MeshData{
        std::vector<Vertex> vertices;
        std::vector<UINT16> indices;
    };
    
    MeshData CreateGrid(float width, float depth, UINT m, UINT n);
    MeshData CreateBox(float width, float height, float depth, UINT32 numSubdivisions);
    MeshData CreateSphere(float radius, UINT32 sliceCount, UINT32 stackCount);
    MeshData CreateGeosphere(float radius, UINT32 numSubdivisions);
    MeshData CreateCylinder(
        const float top_radius, const float bottom_radius, const float height,
        const UINT32 slice_count, const UINT32 stack_count
    );
    MeshData CreateQuad(float x, float y, float w, float h, float depth);


private:
    Vertex MidPoint(const Vertex& v0, const Vertex& v1);
	void Subdivide(MeshData& meshData);
    void BuildCylinderCap(
        const float top_radius, const float bottom_radius, const float height,
        const UINT32 slice_count, const UINT32 stack_count, MeshData &mesh_data
    );

};



}