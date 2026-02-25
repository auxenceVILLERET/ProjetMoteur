#pragma once
#include <vector>

class Tiles;
class ECS;
class Renderer;

class TilesManager
{
public:
	static TilesManager& Instance();

	void Initialize(ECS* ecs, Renderer* renderer);

	void AddTile(Tiles* tile);

private:

	std::vector<Tiles*> m_tiles;
};

