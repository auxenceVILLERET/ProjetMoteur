#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Engine/Utils/Shape.h"

class Mesh;
class Renderer;
class UploadContext;

class RessourceManager
{
public:
	static RessourceManager& Instance();

	void Initialize(Renderer* renderer, UploadContext* uploder);

	Mesh* GetCube();
	Mesh* GetCylinder();
	Mesh* GetSphere();
	Mesh* GetMeshByShape(Shape type);

	void FinalizeUpload();

private:
	Mesh* CreateCube();
	Mesh* CreateCylinder();
	Mesh* CreateSphere();

	Renderer* m_renderer = nullptr;
	UploadContext* m_uploader = nullptr;

	Mesh* m_cubeMesh = nullptr;
	Mesh* m_cylinderMesh = nullptr;
	Mesh* m_sphereMesh = nullptr;
	Mesh* m_sphere2Mesh = nullptr;
};

