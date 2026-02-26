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

	void Update(float deltaTime);

	void SpawnTile();
private:

	std::vector<Tiles*> m_tiles;

	std::vector<Tiles*> m_activeTiles;
	float m_spawnZ;
	int m_currentConnection;
};

