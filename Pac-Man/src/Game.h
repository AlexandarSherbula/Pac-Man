#pragma once

#include "olcPixelGameEngine.h"

#include "Level.h"
#include "Object.h"
#include "Player.h"
#include "Ghost.h"

#include <memory>
#include <array>

class Game : public olc::PixelGameEngine
{
public:
	std::unique_ptr<Player> player;
	std::array<std::unique_ptr<Ghost>, 4> ghosts;
	std::unique_ptr<Level> level;
	std::unique_ptr<Object> pacmanDying;

	bool ghostHit;
	bool isDead;
	bool pause;
	float wait;
public:
	Game();
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};