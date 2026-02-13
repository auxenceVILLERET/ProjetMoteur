#pragma once
#include <unordered_map>
#include <memory>
#include <string>

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
	Mesh* GetSphere2();

	void FinalizeUpload();

private:
	Mesh* CreateCube();
	Mesh* CreateCylinder();
	Mesh* CreateSphere();
	Mesh* CreateSphere2();

	Renderer* m_renderer = nullptr;
	UploadContext* m_uploader = nullptr;

	Mesh* m_cubeMesh = nullptr;
	Mesh* m_cylinderMesh = nullptr;
	Mesh* m_sphereMesh = nullptr;
	Mesh* m_sphere2Mesh = nullptr;
};

