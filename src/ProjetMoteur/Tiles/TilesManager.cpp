#include "TilesManager.h"

static TilesManager* s_instance = nullptr;

TilesManager& TilesManager::Instance()
{
	if (s_instance == nullptr)
		s_instance = new TilesManager();
	return *s_instance;
}

void TilesManager::Initialize(ECS* ecs, Renderer* renderer)
{
	// Initialize the tile manager if needed
}

void TilesManager::AddTile(Tiles* tile)
{
	m_tiles.push_back(tile);
}

