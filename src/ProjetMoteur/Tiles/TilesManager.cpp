#include "TilesManager.h"
#include "Tiles.h"
#include "Engine/ECS/ECS.h"
#include "Render/Renderer.h"
#include "Tiles/Tile1.h"
#include "Tiles/Tile2.h"
#include "Tiles/Tile3.h"
#include "Tiles/Tile4.h"
#include "Core/Random.h"
#include <iostream>

static TilesManager* s_instance = nullptr;

TilesManager& TilesManager::Instance()
{
	if (s_instance == nullptr)
		s_instance = new TilesManager();
	return *s_instance;
}

void TilesManager::Initialize(ECS* ecs, Renderer* renderer)
{
	m_spawnZ = 0.0f;
	m_currentConnection = 0;

	Tile1* tile1 = new Tile1();
	tile1->Initialize(ecs, renderer);
	AddTile(tile1);

	Tile2* tile2 = new Tile2();
	tile2->Initialize(ecs, renderer);
	AddTile(tile2);

	Tile3* tile3 = new Tile3();
	tile3->Initialize(ecs, renderer);
	AddTile(tile3);

	Tile4* tile4 = new Tile4();
	tile4->Initialize(ecs, renderer);
	AddTile(tile4);

    for (int i = 0; i < 2; i++)
    {
        SpawnTile();
    }
}

void TilesManager::AddTile(Tiles* tile)
{
	m_tiles.push_back(tile);
}

void TilesManager::Update(float deltaTime)
{
    for (Tiles* tiles : m_tiles)
    {
		tiles->Update(deltaTime);
    }
    
    for (int i = 0; i < m_activeTiles.size(); )
    {
        Tiles* tile = m_activeTiles[i];

        if (tile->GetPosition().z < -18.0f )
        {
            tile->SetActive(false);
            m_activeTiles.erase(m_activeTiles.begin() + i);
			m_spawnZ -= 18.0f;
            SpawnTile();
        }
        else
        {
            tile->Update(deltaTime);
            i++;
        }
    }
}

void TilesManager::SpawnTile()
{
    std::vector<Tiles*> compatibles;

    for (Tiles* tile : m_tiles)
    {
        if (tile->GetIn() == m_currentConnection)
        {
            compatibles.push_back(tile);
        }
    }

    if (compatibles.empty())
        return;

	int index = Random::RandomInt(0, compatibles.size() - 1);
	std::cout << index << std::endl;
    Tiles* chosen = compatibles[index];

    chosen->SetActive(true);

    XMFLOAT3 pos = { 0, 0, m_spawnZ };
    chosen->SetPosition(pos);

    m_spawnZ += 18.0f;
    m_currentConnection = chosen->GetOut();

    m_activeTiles.push_back(chosen);
}
