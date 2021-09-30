#include"GeometryGenerator.hpp"

namespace LemonCube{

GeometryGenerator::MeshData
GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n){
    MeshData meshData;

	UINT32 vertexCount = m*n;
	UINT32 faceCount   = (m-1)*(n-1)*2;

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n-1);
	float dz = depth / (m-1);

	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	meshData.vertices.resize(vertexCount);
	for(UINT32 i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for(UINT32 j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			meshData.vertices[i*n+j].position = XMFLOAT3(x, 0.0f, z);
			meshData.vertices[i*n+j].normal   = XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.vertices[i*n+j].tangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

			meshData.vertices[i*n+j].uv.x = j*du;
			meshData.vertices[i*n+j].uv.y = i*dv;
		}
	}


	meshData.indices.resize(faceCount*3); // 3 indices per face

	UINT32 k = 0;
	for(UINT32 i = 0; i < m-1; ++i)
	{
		for(UINT32 j = 0; j < n-1; ++j)
		{
			meshData.indices[k]   = i*n+j;
			meshData.indices[k+1] = i*n+j+1;
			meshData.indices[k+2] = (i+1)*n+j;

			meshData.indices[k+3] = (i+1)*n+j;
			meshData.indices[k+4] = i*n+j+1;
			meshData.indices[k+5] = (i+1)*n+j+1;

			k += 6;
		}
	}
    return meshData;
}
GeometryGenerator::MeshData
GeometryGenerator::CreateBox(float width, float height, float depth, UINT32 numSubdivisions){
    MeshData meshData;

	Vertex v[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;
    
	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	v[8]  = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.vertices.assign(&v[0], &v[24]);

	UINT16 i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.indices.assign(&i[0], &i[36]);

    // Put a cap on the number of subdivisions.
    numSubdivisions = std::min<UINT16>(numSubdivisions, 6u);

    for(UINT32 i = 0; i < numSubdivisions; ++i)
        Subdivide(meshData);

    return meshData;
}
GeometryGenerator::MeshData
GeometryGenerator::CreateSphere(float radius, UINT32 sliceCount, UINT32 stackCount){
	MeshData meshData;

	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.vertices.push_back( topVertex );

	float phiStep   = XM_PI/stackCount;
	float thetaStep = 2.0f*XM_PI/sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(UINT32 i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
        for(UINT32 j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;

			// spherical to cartesian
			v.position.x = radius*sinf(phi)*cosf(theta);
			v.position.y = radius*cosf(phi);
			v.position.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.tangentU.x = -radius*sinf(phi)*sinf(theta);
			v.tangentU.y = 0.0f;
			v.tangentU.z = +radius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.tangentU);
			XMStoreFloat3(&v.tangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.position);
			XMStoreFloat3(&v.normal, XMVector3Normalize(p));

			v.uv.x = theta / XM_2PI;
			v.uv.y = phi / XM_PI;

			meshData.vertices.push_back( v );
		}
	}

	meshData.vertices.push_back( bottomVertex );

    for(UINT32 i = 1; i <= sliceCount; ++i)
	{
		meshData.indices.push_back(0);
		meshData.indices.push_back(i+1);
		meshData.indices.push_back(i);
	}
	
    UINT32 baseIndex = 1;
    UINT32 ringVertexCount = sliceCount + 1;
	for(UINT32 i = 0; i < stackCount-2; ++i)
	{
		for(UINT32 j = 0; j < sliceCount; ++j)
		{
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.indices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			meshData.indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	UINT32 southPoleIndex = meshData.vertices.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(UINT32 i = 0; i < sliceCount; ++i)
	{
		meshData.indices.push_back(southPoleIndex);
		meshData.indices.push_back(baseIndex+i);
		meshData.indices.push_back(baseIndex+i+1);
	}

    return meshData;

}
GeometryGenerator::MeshData
GeometryGenerator::CreateGeosphere(float radius, UINT32 numSubdivisions){
	MeshData meshData;

    numSubdivisions = std::min<UINT32>(numSubdivisions, 6u);

	const float X = 0.525731f; 
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] = 
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),  
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),    
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X), 
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),    
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f), 
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

    UINT32 k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

    meshData.vertices.resize(12);
    meshData.indices.assign(&k[0], &k[60]);

	for(UINT32 i = 0; i < 12; ++i)
		meshData.vertices[i].position = pos[i];

	for(UINT32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	// Project vertices onto sphere and scale.
	for(UINT16 i = 0; i < meshData.vertices.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.vertices[i].position));

		// Project onto sphere.
		XMVECTOR p = radius*n;

		XMStoreFloat3(&meshData.vertices[i].position, p);
		XMStoreFloat3(&meshData.vertices[i].normal, n);

		// Derive texture coordinates from spherical coordinates.
        float theta = atan2f(meshData.vertices[i].position.z, meshData.vertices[i].position.x);

        // Put in [0, 2pi].
        if(theta < 0.0f)
            theta += XM_2PI;

		float phi = acosf(meshData.vertices[i].position.y / radius);

		meshData.vertices[i].uv.x = theta/XM_2PI;
		meshData.vertices[i].uv.y = phi/XM_PI;

		// Partial derivative of P with respect to theta
		meshData.vertices[i].tangentU.x = -radius*sinf(phi)*sinf(theta);
		meshData.vertices[i].tangentU.y = 0.0f;
		meshData.vertices[i].tangentU.z = +radius*sinf(phi)*cosf(theta);

		XMVECTOR T = XMLoadFloat3(&meshData.vertices[i].tangentU);
		XMStoreFloat3(&meshData.vertices[i].tangentU, XMVector3Normalize(T));
	}

    return meshData;
}

GeometryGenerator::MeshData
GeometryGenerator::CreateCylinder(
    const float top_radius, const float bottom_radius, const float height,
    const UINT32 slice_count, const UINT32 stack_count
){
    MeshData mesh_data;
    float height_bias = -0.5f * height;
    float stack_height = height / stack_count;
    float radius_step = (top_radius - bottom_radius) / stack_count;

    for(UINT32 i = 0; i <= stack_count; i++){
        float y = height_bias + i * stack_height;
        float r = bottom_radius + i * radius_step;
        float delta_theta = 2.0f * XM_PI / slice_count;

        for(UINT32 j = 0; j <= slice_count; j++){
            float cos = cosf(j * delta_theta);
            float sin = sinf(j * delta_theta);
            Vertex vertex;
            vertex.position = XMFLOAT3(r * cos, y, r * sin);
            vertex.tangentU = XMFLOAT3(-sin, 0, cos);

            float dr = bottom_radius - top_radius;
            XMFLOAT3 bitangent(dr*cos, -height, dr*sin);

            XMVECTOR T = XMLoadFloat3(&vertex.tangentU);
            XMVECTOR B = XMLoadFloat3(&bitangent);
            XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
            XMStoreFloat3(&vertex.normal, N);

            vertex.uv = XMFLOAT2(
                static_cast<float>(j) / slice_count,
                1.0f - static_cast<float>(i) / stack_count
            );
            mesh_data.vertices.push_back(vertex);
        }
    }
    float vertex_count = slice_count + 1;
    for(UINT32 i = 0; i < stack_count; i++){
        for(UINT32 j = 0; j < slice_count; j++){
                mesh_data.indices.push_back(i * vertex_count + j);
                mesh_data.indices.push_back((i + 1) * vertex_count + j);
                mesh_data.indices.push_back((i + 1) * vertex_count + j + 1);

                mesh_data.indices.push_back(i * vertex_count + j);
                mesh_data.indices.push_back((i + 1) * vertex_count + j + 1);
                mesh_data.indices.push_back(i * vertex_count + j + 1);
        }
    }

    BuildCylinderCap(
        top_radius, bottom_radius, height, 
        slice_count, stack_count, mesh_data
    );

    return mesh_data;
}

GeometryGenerator::MeshData 
GeometryGenerator::CreateQuad(float x, float y, float w, float h, float depth){
    MeshData meshData;

	meshData.vertices.resize(4);
	meshData.indices.resize(6);

	// Position coordinates specified in NDC space.
	meshData.vertices[0] = Vertex(
        x, y - h, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);

	meshData.vertices[1] = Vertex(
		x, y, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);

	meshData.vertices[2] = Vertex(
		x+w, y, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);

	meshData.vertices[3] = Vertex(
		x+w, y-h, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);

	meshData.indices[0] = 0;
	meshData.indices[1] = 1;
	meshData.indices[2] = 2;

	meshData.indices[3] = 0;
	meshData.indices[4] = 2;
	meshData.indices[5] = 3;

    return meshData;
}

GeometryGenerator::Vertex 
GeometryGenerator::MidPoint(const Vertex &v0, const Vertex &v1){
XMVECTOR p0 = XMLoadFloat3(&v0.position);
    XMVECTOR p1 = XMLoadFloat3(&v1.position);

    XMVECTOR n0 = XMLoadFloat3(&v0.normal);
    XMVECTOR n1 = XMLoadFloat3(&v1.normal);

    XMVECTOR tan0 = XMLoadFloat3(&v0.tangentU);
    XMVECTOR tan1 = XMLoadFloat3(&v1.tangentU);

    XMVECTOR tex0 = XMLoadFloat2(&v0.uv);
    XMVECTOR tex1 = XMLoadFloat2(&v1.uv);
 
    XMVECTOR pos = 0.5f*(p0 + p1);
    XMVECTOR normal = XMVector3Normalize(0.5f*(n0 + n1));
    XMVECTOR tangent = XMVector3Normalize(0.5f*(tan0+tan1));
    XMVECTOR tex = 0.5f*(tex0 + tex1);

    Vertex v;
    XMStoreFloat3(&v.position, pos);
    XMStoreFloat3(&v.normal, normal);
    XMStoreFloat3(&v.tangentU, tangent);
    XMStoreFloat2(&v.uv, tex);

    return v;
}

void GeometryGenerator::Subdivide(MeshData &meshData){
	MeshData inputCopy = meshData;

	meshData.vertices.resize(0);
	meshData.indices.resize(0);

	UINT32 numTris = (UINT32)inputCopy.indices.size()/3;
	for(UINT32 i = 0; i < numTris; ++i)
	{
		Vertex v0 = inputCopy.vertices[inputCopy.indices[i * 3 + 0]];
		Vertex v1 = inputCopy.vertices[inputCopy.indices[i * 3 + 1]];
		Vertex v2 = inputCopy.vertices[inputCopy.indices[i * 3 + 2]];

        Vertex m0 = MidPoint(v0, v1);
        Vertex m1 = MidPoint(v1, v2);
        Vertex m2 = MidPoint(v0, v2);

		meshData.vertices.push_back(v0); // 0
		meshData.vertices.push_back(v1); // 1
		meshData.vertices.push_back(v2); // 2
		meshData.vertices.push_back(m0); // 3
		meshData.vertices.push_back(m1); // 4
		meshData.vertices.push_back(m2); // 5
 
		meshData.indices.push_back(i*6+0);
		meshData.indices.push_back(i*6+3);
		meshData.indices.push_back(i*6+5);

		meshData.indices.push_back(i*6+3);
		meshData.indices.push_back(i*6+4);
		meshData.indices.push_back(i*6+5);

		meshData.indices.push_back(i*6+5);
		meshData.indices.push_back(i*6+4);
		meshData.indices.push_back(i*6+2);

		meshData.indices.push_back(i*6+3);
		meshData.indices.push_back(i*6+1);
		meshData.indices.push_back(i*6+4);
	}
}
void GeometryGenerator::BuildCylinderCap(
    const float top_radius, const float bottom_radius, const float height,
    const UINT32 slice_count, const UINT32 stack_count, MeshData &mesh_data
){
    const float half_height = 0.5f * height;
    const float delta_theta = 2.0f * XM_PI / slice_count;
    UINT32 base_count = mesh_data.vertices.size();

    for(int i = 0; i <= slice_count; i++){
        float x = cosf(i * delta_theta) * top_radius;
        float z = sinf(i * delta_theta) * top_radius;
        float u = x / height + 0.5f;
        float v = z / height + 0.5f;

        mesh_data.vertices.push_back(
            Vertex{x, half_height, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v}
        );
    }
    mesh_data.vertices.push_back(
        Vertex(0.0f, half_height, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f)
    );

    UINT32 center_index = mesh_data.vertices.size() - 1;
    for(int i = 0; i < slice_count; i++){
        mesh_data.indices.push_back(base_count+i);
        mesh_data.indices.push_back(center_index);
		mesh_data.indices.push_back(base_count+i+1);
    }

    base_count = mesh_data.vertices.size();

    for(int i = 0; i <= slice_count; i++){
        float x = cosf(i * delta_theta) * bottom_radius;
        float z = sinf(i * delta_theta) * bottom_radius;
        float u = x / height + 0.5f;
        float v = z / height + 0.5f;

        mesh_data.vertices.push_back(
            Vertex{x, -half_height, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v}
        );
    }
    mesh_data.vertices.push_back(
        Vertex(0.0f, -half_height, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f)
    );

    center_index = mesh_data.vertices.size() - 1;
    for(int i = 0; i < slice_count; i++){
        mesh_data.indices.push_back(base_count + i + 1);
        mesh_data.indices.push_back(center_index);
        mesh_data.indices.push_back(base_count + i);
    }
}


}