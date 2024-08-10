#pragma once

#include "olcPixelGameEngine.h"
#include "Object.h"

#include <string>
#include <array>
#include <memory>

class Level : public Object
{
private:
	std::string Map;
	int levelWidth;
	int levelHeight;
	std::unique_ptr<Object> blackRects;
	bool powerPelletFlick;
	float powerPelletFlickTime;
public:
	std::array<olc::vi2d, 64> crossOverPos;
public:
	Level(olc::PixelGameEngine* pge);

	void Update(float fElapsedTime);

	bool IsWall(const olc::vi2d& id);

	char GetTile(const olc::vi2d& id);
	void SetTile(const olc::vi2d& id, char c);

	void Draw();
};