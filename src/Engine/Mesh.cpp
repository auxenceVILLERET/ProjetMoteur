#include "Mesh.h"
#include "UploadContext.h"

bool Mesh::Initialize(UploadContext& uploader, const Vertex* vertices, uint32_t vertexCount, const uint16_t* indices, uint32_t indexCount)
{
	if (vertices == NULL || vertexCount == 0 || indices == NULL || indexCount == 0)
		return false;

	Release();
	m_indexCount = indexCount;

	const uint64_t vbBytes = uint64_t(vertexCount) * sizeof(Vertex);
	const uint64_t ibBytes = uint64_t(indexCount) * sizeof(uint16_t);

	if (uploader.UploadBuffer(vertices, vbBytes, m_pVb, m_pVbUpload, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER) == false)
		return false;

	m_vbView.BufferLocation = m_pVb->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)vbBytes;
	m_vbView.StrideInBytes = sizeof(Vertex);

	if (uploader.UploadBuffer(indices, ibBytes, m_pIb, m_pIbUpload, D3D12_RESOURCE_STATE_INDEX_BUFFER) == false)
		return false;

	m_ibView.BufferLocation = m_pIb->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)ibBytes;
	m_ibView.Format = DXGI_FORMAT_R16_UINT;

	return true;
}

bool Mesh::CreateTriangle(UploadContext& uploader)
{
	Vertex vertices[] =
	{
		{ { 0.0f,  0.5f, 0.0f }, { 1,0,0,1 } },
		{ { 0.5f, -0.5f, 0.0f }, { 0,1,0,1 } },
		{ { -0.5f,-0.5f, 0.0f }, { 0,0,1,1 } },
	};
	uint16_t indices[] = { 0,1,2 };

	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateQuad(UploadContext& uploader)
{
	Vertex vertices[] =
	{
		{ { -0.5f,  0.5f, 0.0f }, { 1,1,0,1 } },
		{ {  0.5f,  0.5f, 0.0f }, { 0,1,1,1 } },
		{ {  0.5f, -0.5f, 0.0f }, { 1,0,1,1 } },
		{ { -0.5f, -0.5f, 0.0f }, { 1,1,1,1 } },
	};
	uint16_t indices[] = { 0,1,2, 0,2,3 };
	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateCube(UploadContext& uploader)
{
	Vertex vertices[] =
	{
		{ { -1.0f, -1.0f, -1.0f }, { 1,0,0,1 } },
		{ { -1.0f, +1.0f, -1.0f }, { 0,1,0,1 } },
		{ { +1.0f, +1.0f, -1.0f }, { 0,0,1,1 } },
		{ { +1.0f, -1.0f, -1.0f }, { 1,1,0,1 } },
		{ { -1.0f, -1.0f, +1.0f }, { 1,0,1,1 } },
		{ { -1.0f, +1.0f, +1.0f }, { 0,1,1,1 } },
		{ { +1.0f, +1.0f, +1.0f }, { 1,1,1,1 } },
		{ { +1.0f, -1.0f, +1.0f }, { 0,0,0,1 } }
	};
	uint16_t indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4,6,5,
		4,7,6,
		// left face
		4,5,1,
		4,1,0,
		// right face
		3,2,6,
		3,6,7,
		// top face
		1,5,6,
		1,6,2,
		// bottom face
		4,0,3,
		4,3,7
	};
	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateCylinder(UploadContext& uploader)
{
	Vertex vertices[] =
	{
		// Base (y = 0)
		{ { 1.0000f, 0.0f,  0.0000f }, {1,0,0,1} },
		{ { 0.9239f, 0.0f,  0.3827f }, {1,0.5f,0,1} },
		{ { 0.7071f, 0.0f,  0.7071f }, {1,1,0,1} },
		{ { 0.3827f, 0.0f,  0.9239f }, {0.5f,1,0,1} },
		{ { 0.0000f, 0.0f,  1.0000f }, {0,1,0,1} },
		{ { -0.3827f, 0.0f,  0.9239f }, {0,1,0.5f,1} },
		{ { -0.7071f, 0.0f,  0.7071f }, {0,1,1,1} },
		{ { -0.9239f, 0.0f,  0.3827f }, {0,0.5f,1,1} },
		{ { -1.0000f, 0.0f,  0.0000f }, {0,0,1,1} },
		{ { -0.9239f, 0.0f, -0.3827f }, {0.5f,0,1,1} },
		{ { -0.7071f, 0.0f, -0.7071f }, {1,0,1,1} },
		{ { -0.3827f, 0.0f, -0.9239f }, {1,0,0.5f,1} },
		{ { 0.0000f, 0.0f, -1.0000f }, {1,0,0,1} },
		{ { 0.3827f, 0.0f, -0.9239f }, {1,0.5f,0,1} },
		{ { 0.7071f, 0.0f, -0.7071f }, {1,1,0,1} },
		{ { 0.9239f, 0.0f, -0.3827f }, {0.5f,1,0,1} },

		// Haut (y = 1)
		{ { 1.0000f, 1.0f,  0.0000f }, {1,0,0,1} },
		{ { 0.9239f, 1.0f,  0.3827f }, {1,0.5f,0,1} },
		{ { 0.7071f, 1.0f,  0.7071f }, {1,1,0,1} },
		{ { 0.3827f, 1.0f,  0.9239f }, {0.5f,1,0,1} },
		{ { 0.0000f, 1.0f,  1.0000f }, {0,1,0,1} },
		{ { -0.3827f, 1.0f,  0.9239f }, {0,1,0.5f,1} },
		{ { -0.7071f, 1.0f,  0.7071f }, {0,1,1,1} },
		{ { -0.9239f, 1.0f,  0.3827f }, {0,0.5f,1,1} },
		{ { -1.0000f, 1.0f,  0.0000f }, {0,0,1,1} },
		{ { -0.9239f, 1.0f, -0.3827f }, {0.5f,0,1,1} },
		{ { -0.7071f, 1.0f, -0.7071f }, {1,0,1,1} },
		{ { -0.3827f, 1.0f, -0.9239f }, {1,0,0.5f,1} },
		{ { 0.0000f, 1.0f, -1.0000f }, {1,0,0,1} },
		{ { 0.3827f, 1.0f, -0.9239f }, {1,0.5f,0,1} },
		{ { 0.7071f, 1.0f, -0.7071f }, {1,1,0,1} },
		{ { 0.9239f, 1.0f, -0.3827f }, {0.5f,1,0,1} },

		// centres
		{ { 0.0f, 0.0f, 0.0f }, {1,1,1,1} },
		{ { 0.0f, 1.0f, 0.0f }, {1,1,1,1} },
	};

	uint16_t indices[] =
	{
		// DRAW TRIANGLES HERE
		0, 16, 1,	 1, 17, 2,	  2, 18, 3,
		1, 16, 17,	 2, 17, 18,	  3, 18, 19,
		3, 19, 4,	 4, 20, 5,	  5, 21, 6,
		4, 19, 20,	 5, 20, 21,	  6, 21, 22,
		6, 22, 7,	 7, 23, 8,	  8, 24, 9,
		7, 22, 23,	 8, 23, 24,	  9, 24, 25,
		9, 25, 10,	 10, 26, 11,  11, 27, 12,
		10, 25, 26,	 11, 26, 27,  12, 27, 28,
		12, 28, 13,	 13, 29, 14,  14, 30, 15,
		13, 28, 29,	 14, 29, 30,  15, 30, 31,

		// fermeture
		15, 31, 0,
		0, 31, 16,

		// Cap bas (y = 0)
		32, 0, 1, 32,  1,2,
		
		32,  2,3, 32,  3,4,
		32,  4,5, 32, 5, 6,
		32, 6, 7, 32, 7, 8,
		32,  8,9, 32,  9,10,
		32,  10,11, 32,  11,12,
		32,  12,13, 32,  13,14,
		32, 14,15, 32,15, 0,
		
		// Cap haut (y = 1)
		33, 17, 16, 33, 18, 17,		
		33, 19, 18, 33, 20, 19,		
		33, 21, 20, 33, 22, 21,	
		33, 23, 22, 33, 24, 23,
		33, 25, 24, 33, 26, 25,
		33, 27, 26, 33, 28, 27,
		33, 29, 28, 33, 30, 29,
		33, 31, 30, 33, 16, 31,
		
	};

	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateIco(UploadContext& uploader)
{
	static const float a = 0.525731f;
	static const float b = 0.850651f;

	Vertex vertices[] =
	{
		{ { -a, 0.0f, +b }, {1,0,0,1} },
		{ { +a, 0.0f, +b }, {0,1,0,1} },
		{ { -a, 0.0f, -b }, {0,0,1,1} },
		{ { +a, 0.0f, -b }, {1,1,0,1} },

		{ { 0.0f, +b, +a }, {1,0,1,1} },
		{ { 0.0f, +b, -a }, {0,1,1,1} },
		{ { 0.0f, -b, +a }, {1,0.5f,0,1} },
		{ { 0.0f, -b, -a }, {0.5f,0,1,1} },

		{ { +b, +a, 0.0f }, {1,1,1,1} },
		{ { -b, +a, 0.0f }, {0.3f,1,0.3f,1} },
		{ { +b, -a, 0.0f }, {1,0.3f,0.3f,1} },
		{ { -b, -a, 0.0f }, {0.3f,0.3f,1,1} },
	};

	uint16_t indices[] =
	{
		// DRAW TRIANGLES HERE

		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateIco2(UploadContext& uploader)
{
	static const float t = 0.525731112119133606f;
	static const float z = 0.850650808352039932f;

	Vertex vertices[] =
	{
	{{-t,0, z},{1,1,1,1}},{{ t,0, z},{1,1,1,1}},{{-t,0,-z},{1,1,1,1}},{{ t,0,-z},{1,1,1,1}},
	{{0, z, t},{1,1,1,1}},{{0, z,-t},{1,1,1,1}},{{0,-z, t},{1,1,1,1}},{{0,-z,-t},{1,1,1,1}},
	{{ z, t,0},{1,1,1,1}},{{-z, t,0},{1,1,1,1}},{{ z,-t,0},{1,1,1,1}},{{-z,-t,0},{1,1,1,1}},

	{{-0.809017f, 0.5f, 0.309017f},{1,0,0,1}},
	{{-0.309017f, 0.809017f, 0.5f},{0,1,0,1}},
	{{ 0.309017f, 0.809017f, 0.5f},{0,0,1,1}},
	{{ 0.809017f, 0.5f, 0.309017f},{1,0,0,1}},
	{{ 0.809017f,-0.5f, 0.309017f},{0,1,0,1}},
	{{ 0.309017f,-0.809017f, 0.5f},{0,0,1,1}},
	{{-0.309017f,-0.809017f, 0.5f},{1,0,0,1}},
	{{-0.809017f,-0.5f, 0.309017f},{0,1,0,1}},

	{{-0.809017f, 0.5f,-0.309017f},{1,0,0,1}},
	{{-0.309017f, 0.809017f,-0.5f},{0,1,0,1}},
	{{ 0.309017f, 0.809017f,-0.5f},{0,0,1,1}},
	{{ 0.809017f, 0.5f,-0.309017f},{1,0,0,1}},
	{{ 0.809017f,-0.5f,-0.309017f},{0,1,0,1}},
	{{ 0.309017f,-0.809017f,-0.5f},{0,0,1,1}},
	{{-0.309017f,-0.809017f,-0.5f},{1,0,0,1}},
	{{-0.809017f,-0.5f,-0.309017f},{0,1,0,1}},

	{{0,0,1},{1,1,1,1}},{{0,0,-1}, {1,0,0,1}},
	{{1,0,0},{1,1,1,1}},{{-1,0,0}, {0,1,0,1}},
	{{0,1,0},{1,1,1,1}},{{0,-1,0}, {0,0,1,1}},
	{{0.5f,0.309017f,0.809017f},   {1,0,0,1}},
	{{-0.5f,0.309017f,0.809017f},  {0,1,0,1}},
	{{0.5f,-0.309017f,0.809017f},  {0,0,1,1}},
	{{-0.5f,-0.309017f,0.809017f}, {1,0,0,1}},
	{{0.5f,0.309017f,-0.809017f},  {0,1,0,1}},
	{{-0.5f,0.309017f,-0.809017f}, {0,0,1,1}},
	{{0.5f,-0.309017f,-0.809017f}, {0,1,0,1}},
	{{-0.5f,-0.309017f,-0.809017f},{0,0,1,1}}
	};

	uint16_t indices[] =
	{
	0,13,12, 13,4,14, 12,14,1, 13,14,12,
	0,12,19, 12,1,15, 19,15,6, 12,15,19,
	0,19,18, 19,6,17, 18,17,11, 19,17,18,
	0,18,16, 18,11,10, 16,10,8, 18,10,16,
	0,16,13, 16,8,4, 13,4,12, 16,4,13,

	4,22,14, 22,5,23, 14,23,1, 22,23,14,
	8,15,22, 15,1,23, 22,23,5, 15,23,22,
	10,24,15, 24,3,25, 15,25,7, 24,25,15,
	11,17,26, 17,6,27, 26,27,7, 17,27,26,
	9,20,13, 20,2,21, 13,21,4, 20,21,13,

	2,30,21, 30,3,24, 21,24,5, 30,24,21,
	3,25,30, 25,7,31, 30,31,2, 25,31,30,
	6,28,17, 28,7,27, 17,27,11, 28,27,17,
	8,16,29, 16,10,24, 29,24,3, 16,24,29,
	9,32,20, 32,4,21, 20,21,2, 32,21,20
	};

	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

void Mesh::Release()
{
	SafeRelease(m_pVb);
	SafeRelease(m_pVbUpload);
	SafeRelease(m_pIb);
	SafeRelease(m_pIbUpload);
	m_indexCount = 0;
}

void Mesh::FinalizeUpload()
{
	SafeRelease(m_pVbUpload);
	SafeRelease(m_pIbUpload);
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList) const
{
	cmdList->IASetVertexBuffers(0, 1, &m_vbView);
	cmdList->IASetIndexBuffer(&m_ibView);
	cmdList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}



