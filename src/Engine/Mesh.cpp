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
	vertices.push_back(Vertex{ { 0.0f,  0.5f, 0.0f }, { 0.5f, 0.5f } });
	vertices.push_back(Vertex{ { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } });
	vertices.push_back(Vertex{ { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f } });

	std::vector<uint32_t> indices = { 0,1,2 };

	return Initialize(uploader, std::move(vertices), std::move(indices));
}

bool Mesh::CreateQuad(UploadContext& uploader)
{
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } });
	vertices.push_back(Vertex{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } });
	vertices.push_back(Vertex{ {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } });
	vertices.push_back(Vertex{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } });

	std::vector<uint32_t> indices =
	{
		0, 1, 2,
		0, 2, 3
	};

	return Initialize(uploader, std::move(vertices), std::move(indices));
}

bool Mesh::CreateCube(UploadContext& uploader)
{
	const float s = 0.5f;

	std::vector<Vertex> vertices;
	vertices.reserve(24);

	std::vector<uint32_t> indices;
	indices.reserve(36);

	auto addFace = [&](XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, XMFLOAT3 d)
		{
			// 4 vertices par face
			uint32_t base = (uint32_t)vertices.size();

			// UV cohérents (V inversé souvent pratique selon chargement)
			vertices.push_back(Vertex{ a, {0.0f, 1.0f} }); // bas-gauche
			vertices.push_back(Vertex{ b, {0.0f, 0.0f} }); // haut-gauche
			vertices.push_back(Vertex{ c, {1.0f, 0.0f} }); // haut-droit
			vertices.push_back(Vertex{ d, {1.0f, 1.0f} }); // bas-droit

			// 2 triangles
			indices.push_back(base + 0); indices.push_back(base + 1); indices.push_back(base + 2);
			indices.push_back(base + 0); indices.push_back(base + 2); indices.push_back(base + 3);
		};

	// BACK  (-Z)
	addFace({ -s, -s, -s }, { -s, +s, -s }, { +s, +s, -s }, { +s, -s, -s });

	// RIGHT (+X)
	addFace({ +s, -s, -s }, { +s, +s, -s }, { +s, +s, +s }, { +s, -s, +s });

	// FRONT (+Z)
	addFace({ +s, -s, +s }, { +s, +s, +s }, { -s, +s, +s }, { -s, -s, +s });

	// LEFT  (-X)
	addFace({ -s, -s, +s }, { -s, +s, +s }, { -s, +s, -s }, { -s, -s, -s });

	// TOP   (+Y)
	addFace({ -s, +s, -s }, { -s, +s, +s }, { +s, +s, +s }, { +s, +s, -s });

	// BOTTOM(-Y)
	addFace({ -s, -s, +s }, { -s, -s, -s }, { +s, -s, -s }, { +s, -s, +s });

	return Initialize(uploader, std::move(vertices), std::move(indices));
}


bool Mesh::CreateCylinder(UploadContext& uploader)
{
	// Cylindre simple (16 slices)
	const uint32_t slices = 16;
	const float radius = 1.0f;
	const float y0 = 0.0f;
	const float y1 = 1.0f;

	std::vector<Vertex> vertices;
	vertices.reserve(slices * 2 + 2);

	for (uint32_t s = 0; s < slices; ++s)
	{
		float u = (float)s / (float)slices;
		float ang = u * XM_2PI;
		float x = cosf(ang) * radius;
		float z = sinf(ang) * radius;

		vertices.push_back(Vertex{ { x, y0, z }, { u, 1.0f } }); // bas
		vertices.push_back(Vertex{ { x, y1, z }, { u, 0.0f } }); // haut
	}

	uint32_t centerBottom = (uint32_t)vertices.size();
	vertices.push_back(Vertex{ { 0.0f, y0, 0.0f }, { 0.5f, 0.5f } });

	uint32_t centerTop = (uint32_t)vertices.size();
	vertices.push_back(Vertex{ { 0.0f, y1, 0.0f }, { 0.5f, 0.5f } });

	std::vector<uint32_t> indices;
	indices.reserve(slices * 12);

	for (uint32_t s = 0; s < slices; ++s)
	{
		uint32_t sn = (s + 1) % slices;

		uint32_t b0 = s * 2;
		uint32_t t0 = s * 2 + 1;
		uint32_t b1 = sn * 2;
		uint32_t t1 = sn * 2 + 1;

		// sides (CCW LH)
		indices.push_back(b0); indices.push_back(t0); indices.push_back(t1);
		indices.push_back(b0); indices.push_back(t1); indices.push_back(b1);

		// cap bottom
		indices.push_back(centerBottom); indices.push_back(b1); indices.push_back(b0);

		// cap top
		indices.push_back(centerTop); indices.push_back(t0); indices.push_back(t1);
	}

	return Initialize(uploader, std::move(vertices), std::move(indices));
}


bool Mesh::CreateSphere(UploadContext& uploader)
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

			vertices.push_back(Vertex{ pos, { u, 1.0f - v } });
		}
	}

	// Indices
	const uint32_t ring = slices + 1;
	for (uint32_t stack = 0; stack < stacks; ++stack)
	{
		for (uint32_t slice = 0; slice < slices; ++slice)
		{
			uint32_t i0 = (uint16_t)(stack * ring + slice);
			uint32_t i1 = (uint16_t)((stack + 1) * ring + slice);
			uint32_t i2 = (uint16_t)((stack + 1) * ring + (slice + 1));
			uint32_t i3 = (uint16_t)(stack * ring + (slice + 1));

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
