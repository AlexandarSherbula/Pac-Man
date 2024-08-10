#pragma once

#include "olcPixelGameEngine.h"

#include "DynamicObject.h"
#include "Level.h"
#include "Player.h"

enum class GhostMode
{
	SCATTER,
	CHASE,
	FRIGHTENED
};

enum class GhostHouse
{
	INSIDE,
	GOING_SIDE,
	GOING_UP,
	OUTSIDE
};


class Ghost : public DynamicObject
{
private:
	olc::vi2d targetTile;
	olc::vi2d blinkyPos;
	olc::vf2d prevCrossPos;
	GhostHouse ghostHouse;
	GhostHouse initGhostHouse;
	float timer;
	float lastTimer;
	float noPelletTimerUnlock;
	int pelletCounterUnlock;
public:
	bool InitFright;
	bool eaten;
	GhostMode mode;
	GhostMode lastMode;
public:
	Ghost(olc::PixelGameEngine* pge, GhostHouse gh, int pelletCounter);

	void Update(float fElapsedTime, Level* level, Player* player, short ghostID);

	void ChangeMode(float fElapsedTime, bool isEnergized);
	void CheckTargetTileAndMode(Level* level, Player* player, short ghostID, const olc::vi2d& blinkyPos);
	void CheckHouseMode();

	void GhostHouseMoving();
	void Move(Level* level) override;
	void WallHit(Level* level);
	void CheckDirection() override;
	void FrightenedMode(Level* level, float energizedTimer);
	void RetreatToHouse();

	void Restart() override;
};

int GetRandomExcluding(int num);
