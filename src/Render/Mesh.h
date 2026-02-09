#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Helpers/d3dUtil.h"

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class Mesh
{
public:
	void Cube() {

		// CUBE CENTERED IN 0,0,0

		std::array<Vertex, 8> verticesCube =
		{
			Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::DarkCyan) }),
			Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::DarkCyan) }),
			Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::DarkCyan) }),
			Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::DarkCyan) }),
			Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::DarkCyan) }),
			Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::DarkCyan) }),
			Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::DarkCyan) }),
			Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::DarkCyan) })
		};

		std::array<std::uint16_t, 36> indicesCube =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};
	}

	void Ico() 
	{

		// Spheroid
		//icosahedron (20 faces)

		// normalized
		float a = 0.525731f; // 2PI/12
		float b = 0.850651f;

		//numbers of faces
		static const int n = 12;

		std::array<Vertex, n > vertices =
		{
			Vertex({ XMFLOAT3(-a, 0.0f, +b) }),
			Vertex({ XMFLOAT3(+a, 0.0f, +b) }),
			Vertex({ XMFLOAT3(-a, 0.0f, -b) }),
			Vertex({ XMFLOAT3(+a, 0.0f, -b) }),

			Vertex({ XMFLOAT3(0.0f, +b, +a) }),
			Vertex({ XMFLOAT3(0.0f, +b, -a) }),
			Vertex({ XMFLOAT3(0.0f, -b, +a) }),
			Vertex({ XMFLOAT3(0.0f, -b, -a) }),

			Vertex({ XMFLOAT3(+b,  +a, 0.0f) }),
			Vertex({ XMFLOAT3(-b,  +a, 0.0f) }),
			Vertex({ XMFLOAT3(+b,  -a, 0.0f) }),
			Vertex({ XMFLOAT3(-b,  -a, 0.0f) }),
		};

		for (size_t i = 0; i < n; i++)
		{
			vertices[i].Color.x = ((i * (1.0f / n)));
		}

		std::array<std::uint16_t, 60> indices =
		{
			// DRAW TRIANGLES HERE

			1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
			1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
			3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
			10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
		};
	}

	void Cylindre() 
	{
		//CYLINDER
		std::array<Vertex, 32 + 2 > vertices =
		{
			// Base (y = 0)
		   Vertex({ XMFLOAT3(1.0000f, 0.0f,  0.0000f) }),
		   Vertex({ XMFLOAT3(0.9239f, 0.0f,  0.3827f) }),
		   Vertex({ XMFLOAT3(0.7071f, 0.0f,  0.7071f) }),
		   Vertex({ XMFLOAT3(0.3827f, 0.0f,  0.9239f) }),
		   Vertex({ XMFLOAT3(0.0000f, 0.0f,  1.0000f) }),
		   Vertex({ XMFLOAT3(-0.3827f, 0.0f,  0.9239f) }),
		   Vertex({ XMFLOAT3(-0.7071f, 0.0f,  0.7071f) }),
		   Vertex({ XMFLOAT3(-0.9239f, 0.0f,  0.3827f) }),
		   Vertex({ XMFLOAT3(-1.0000f, 0.0f,  0.0000f) }),
		   Vertex({ XMFLOAT3(-0.9239f, 0.0f, -0.3827f) }),
		   Vertex({ XMFLOAT3(-0.7071f, 0.0f, -0.7071f) }),
		   Vertex({ XMFLOAT3(-0.3827f, 0.0f, -0.9239f) }),
		   Vertex({ XMFLOAT3(0.0000f, 0.0f, -1.0000f) }),
		   Vertex({ XMFLOAT3(0.3827f, 0.0f, -0.9239f) }),
		   Vertex({ XMFLOAT3(0.7071f, 0.0f, -0.7071f) }),
		   Vertex({ XMFLOAT3(0.9239f, 0.0f, -0.3827f) }),

		   // Haut (y = 1)
		   Vertex({ XMFLOAT3(1.0000f, 1.0f,  0.0000f) }),
		   Vertex({ XMFLOAT3(0.9239f, 1.0f,  0.3827f) }),
		   Vertex({ XMFLOAT3(0.7071f, 1.0f,  0.7071f) }),
		   Vertex({ XMFLOAT3(0.3827f, 1.0f,  0.9239f) }),
		   Vertex({ XMFLOAT3(0.0000f, 1.0f,  1.0000f) }),
		   Vertex({ XMFLOAT3(-0.3827f, 1.0f,  0.9239f) }),
		   Vertex({ XMFLOAT3(-0.7071f, 1.0f,  0.7071f) }),
		   Vertex({ XMFLOAT3(-0.9239f, 1.0f,  0.3827f) }),
		   Vertex({ XMFLOAT3(-1.0000f, 1.0f,  0.0000f) }),
		   Vertex({ XMFLOAT3(-0.9239f, 1.0f, -0.3827f) }),
		   Vertex({ XMFLOAT3(-0.7071f, 1.0f, -0.7071f) }),
		   Vertex({ XMFLOAT3(-0.3827f, 1.0f, -0.9239f) }),
		   Vertex({ XMFLOAT3(0.0000f, 1.0f, -1.0000f) }),
		   Vertex({ XMFLOAT3(0.3827f, 1.0f, -0.9239f) }),
		   Vertex({ XMFLOAT3(0.7071f, 1.0f, -0.7071f) }),
		   Vertex({ XMFLOAT3(0.9239f, 1.0f, -0.3827f) }),

		   // top and bottom middle point
		   Vertex({ XMFLOAT3(0.0f, 0.0f, 0.0f) }),
		   Vertex({ XMFLOAT3(0.0f, 1.0f, 0.0f) }),
		};

		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].Color.x = ((i * (1.0f / vertices.size())));
		}

		std::array<std::uint16_t, 64 * 3> indices =
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
	}
};

#endif // !MESH_H_INCLUDED