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


	void FinalizeUpload(); // libère les upload buffers 
private:
	Mesh* CreateCube();

	Renderer* m_renderer = nullptr;
	UploadContext* m_uploader = nullptr;

	std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;

};

