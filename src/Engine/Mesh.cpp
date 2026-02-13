#include "Mesh.h"
#include "UploadContext.h"

bool Mesh::Initialize(UploadContext& uploader, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	if (vertices.empty() || indices.empty())
		return false;

	Release();

	// Stockage CPU
	m_vertices = std::move(vertices);
	m_indices = std::move(indices);

	m_indexCount = (uint32_t)m_indices.size();

	const uint64_t vbBytes = uint64_t(m_vertices.size()) * sizeof(Vertex);
	const uint64_t ibBytes = uint64_t(m_indices.size()) * sizeof(uint32_t);

	// Upload GPU
	if (uploader.UploadBuffer(m_vertices.data(), vbBytes, m_pVb, m_pVbUpload,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER) == false)
		return false;

	m_vbView.BufferLocation = m_pVb->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)vbBytes;
	m_vbView.StrideInBytes = sizeof(Vertex);

	if (uploader.UploadBuffer(m_indices.data(), ibBytes, m_pIb, m_pIbUpload,
		D3D12_RESOURCE_STATE_INDEX_BUFFER) == false)
		return false;

	m_ibView.BufferLocation = m_pIb->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)ibBytes;
	m_ibView.Format = DXGI_FORMAT_R32_UINT;

	return true;
}

bool Mesh::CreateTriangle(UploadContext& uploader)
{
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex{ { 0.0f,  0.5f, 0.0f } });
	vertices.push_back(Vertex{ { 0.5f, -0.5f, 0.0f } });
	vertices.push_back(Vertex{ { -0.5f,-0.5f, 0.0f } });

	std::vector<uint32_t> indices = { 0,1,2 };

	return Initialize(uploader, std::move(vertices), std::move(indices));
}

bool Mesh::CreateQuad(UploadContext& uploader)
{
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex{ { -0.5f,  0.5f, 0.0f } });
	vertices.push_back(Vertex{ {  0.5f,  0.5f, 0.0f } });
	vertices.push_back(Vertex{ {  0.5f, -0.5f, 0.0f } });
	vertices.push_back(Vertex{ { -0.5f, -0.5f, 0.0f } });

	std::vector<uint32_t> indices =
	{
		0, 1, 2,
		0, 2, 3
	};

	return Initialize(uploader, std::move(vertices), std::move(indices));
}

bool Mesh::CreateCube(UploadContext& uploader)
{
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex{ { -1.0f, -1.0f, -1.0f } });
	vertices.push_back(Vertex{ { -1.0f, +1.0f, -1.0f } });
	vertices.push_back(Vertex{ { +1.0f, +1.0f, -1.0f } });
	vertices.push_back(Vertex{ { +1.0f, -1.0f, -1.0f } });
	vertices.push_back(Vertex{ { -1.0f, -1.0f, +1.0f } });
	vertices.push_back(Vertex{ { -1.0f, +1.0f, +1.0f } });
	vertices.push_back(Vertex{ { +1.0f, +1.0f, +1.0f } });
	vertices.push_back(Vertex{ { +1.0f, -1.0f, +1.0f } });

	std::vector<uint32_t> indices=
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
	return Initialize(uploader, std::move(vertices), std::move(indices));
}

bool Mesh::CreateCylinder(UploadContext& uploader)
{
	std::vector<Vertex> vertices;
	vertices.reserve(34); // 16 pour la base, 16 pour le haut, 2 pour les centres

	vertices.push_back(Vertex{ { 1.0000f, 0.0f,  0.0000f } });
	vertices.push_back(Vertex{ { 0.9239f, 0.0f,  0.3827f } });
	vertices.push_back(Vertex{ { 0.7071f, 0.0f,  0.7071f } });
	vertices.push_back(Vertex{ { 0.3827f, 0.0f,  0.9239f } });
	vertices.push_back(Vertex{ { 0.0000f, 0.0f,  1.0000f } });
	vertices.push_back(Vertex{ { -0.3827f, 0.0f,  0.9239f } });
	vertices.push_back(Vertex{ { -0.7071f, 0.0f,  0.7071f } });
	vertices.push_back(Vertex{ { -0.9239f, 0.0f,  0.3827f } });
	vertices.push_back(Vertex{ { -1.0000f, 0.0f,  0.0000f } });
	vertices.push_back(Vertex{ { -0.9239f, 0.0f, -0.3827f } });
	vertices.push_back(Vertex{ { -0.7071f, 0.0f, -0.7071f } });
	vertices.push_back(Vertex{ { -0.3827f, 0.0f, -0.9239f } });
	vertices.push_back(Vertex{ { 0.0000f, 0.0f, -1.0000f } });
	vertices.push_back(Vertex{ { 0.3827f, 0.0f, -0.9239f } });
	vertices.push_back(Vertex{ { 0.7071f, 0.0f, -0.7071f } });
	vertices.push_back(Vertex{ { 0.9239f, 0.0f, -0.3827f } });

	vertices.push_back(Vertex{ { 1.0000f, 1.0f,  0.0000f } });
	vertices.push_back(Vertex{ { 0.9239f, 1.0f,  0.3827f } });
	vertices.push_back(Vertex{ { 0.7071f, 1.0f,  0.7071f } });
	vertices.push_back(Vertex{ { 0.3827f, 1.0f,  0.9239f } });
	vertices.push_back(Vertex{ { 0.0000f, 1.0f,  1.0000f } });
	vertices.push_back(Vertex{ { -0.3827f, 1.0f,  0.9239f } });
	vertices.push_back(Vertex{ { -0.7071f, 1.0f,  0.7071f } });
	vertices.push_back(Vertex{ { -0.9239f, 1.0f,  0.3827f } });
	vertices.push_back(Vertex{ { -1.0000f, 1.0f,  0.0000f } });
	vertices.push_back(Vertex{ { -0.9239f, 1.0f, -0.3827f } });
	vertices.push_back(Vertex{ { -0.7071f, 1.0f, -0.7071f } });
	vertices.push_back(Vertex{ { -0.3827f, 1.0f, -0.9239f } });
	vertices.push_back(Vertex{ { 0.0000f, 1.0f, -1.0000f } });
	vertices.push_back(Vertex{ { 0.3827f, 1.0f, -0.9239f } });
	vertices.push_back(Vertex{ { 0.7071f, 1.0f, -0.7071f } });
	vertices.push_back(Vertex{ { 0.9239f, 1.0f, -0.3827f } });

	vertices.push_back(Vertex{ { 0.0f, 0.0f, 0.0f } }); // centre bas
	vertices.push_back(Vertex{ { 0.0f, 1.0f, 0.0f } }); // centre haut

	std::vector<uint32_t> indices =
	{
		// DRAW TRIANGLES HERE
		0, 16, 1,
		1, 16, 17,

		1, 17, 2,
		2, 17, 18,

		2, 18, 3,
		3, 18, 19,

		3, 19, 4,
		4, 19, 20,

		4, 20, 5,
		5, 20, 21,

		5, 21, 6,
		6, 21, 22,

		6, 22, 7,
		7, 22, 23,

		7, 23, 8,
		8, 23, 24,

		8, 24, 9,
		9, 24, 25,

		9, 25, 10,
		10, 25, 26,

		10, 26, 11,
		11, 26, 27,

		11, 27, 12,
		12, 27, 28,

		12, 28, 13,
		13, 28, 29,

		13, 29, 14,
		14, 29, 30,

		14, 30, 15,
		15, 30, 31,

		// fermeture
		15, 31, 0,
		0, 31, 16,

		// Cap bas (y = 0)
		32, 0, 1,
		32,  1,2,
		32,  2,3,
		32,  3,4,
		32,  4,5,
		32, 5, 6,
		32, 6, 7,
		32, 7, 8,
		32,  8,9,
		32,  9,10,
		32,  10,11,
		32,  11,12,
		32,  12,13,
		32,  13,14,
		32, 14,15,
		32,15, 0,

		// Cap haut (y = 1)
		33, 17, 16,
		33, 18, 17,
		33, 19, 18,
		33, 20, 19,
		33, 21, 20,
		33, 22, 21,
		33, 23, 22,
		33, 24, 23,
		33, 25, 24,
		33, 26, 25,
		33, 27, 26,
		33, 28, 27,
		33, 29, 28,
		33, 30, 29,
		33, 31, 30,
		33, 16, 31,
	};

	return Initialize(uploader, std::move(vertices), std::move(indices));
}

bool Mesh::CreateIco(UploadContext& uploader)
{
	uint32_t slices = 10;
	uint32_t stacks = 9;

	uint32_t vertCount = (stacks + 1) * (slices + 1);

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(vertCount);
	indices.reserve(stacks * slices * 6);

	for (uint32_t stack = 0; stack <= stacks; ++stack)
	{
		float v = (float)stack / (float)stacks;      // 0..1
		float phi = v * XM_PI;                       // 0..PI

		float y = cosf(phi);
		float r = sinf(phi);

		for (uint32_t slice = 0; slice <= slices; ++slice)
		{
			float u = (float)slice / (float)slices;  // 0..1
			float theta = u * XM_2PI;                // 0..2PI

			float x = r * cosf(theta);
			float z = r * sinf(theta);

			XMFLOAT3 pos = { x , y , z };

			vertices.push_back(Vertex{ pos });
		}
	}

	// Indices
	const uint32_t ring = slices + 1;
	for (uint32_t stack = 0; stack < stacks; ++stack)
	{
		for (uint32_t slice = 0; slice < slices; ++slice)
		{
			uint16_t i0 = (uint16_t)(stack * ring + slice);
			uint16_t i1 = (uint16_t)((stack + 1) * ring + slice);
			uint16_t i2 = (uint16_t)((stack + 1) * ring + (slice + 1));
			uint16_t i3 = (uint16_t)(stack * ring + (slice + 1));

			indices.push_back(i0); indices.push_back(i1); indices.push_back(i2);
			indices.push_back(i0); indices.push_back(i2); indices.push_back(i3);
		}
	}

	return Initialize(uploader, std::move(vertices), std::move(indices));
}

void Mesh::Release()
{
	SafeRelease(m_pVb);
	SafeRelease(m_pVbUpload);
	SafeRelease(m_pIb);
	SafeRelease(m_pIbUpload);

	m_indexCount = 0;
	m_vertices.clear();
	m_indices.clear();
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



