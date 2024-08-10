#include "Ghost.h"

#include <time.h>

Ghost::Ghost(olc::PixelGameEngine* pge, GhostHouse gh, int pelletCounter)
	: DynamicObject(pge)
{
	srand(time(NULL));
	mode = GhostMode::SCATTER;
	ghostHouse = gh;
	initGhostHouse = gh;
	InitFright = false;
	eaten = false;
	this->pelletCounterUnlock = pelletCounter;
	this->noPelletTimerUnlock = 4.0f;
}

void Ghost::Update(float fElapsedTime, Level* level, Player* player, short ghostID)
{
	newPos = position + velocity * fElapsedTime;

	if (ghostID == 0)
		blinkyPos = this->position;

	ChangeMode(fElapsedTime, player->energized);

	CheckTargetTileAndMode(level, player, ghostID, blinkyPos);

	if (ghostHouse == GhostHouse::GOING_SIDE || ghostHouse == GhostHouse::GOING_UP
		|| mode == GhostMode::FRIGHTENED)
		speed = initSpeed / 2;
	else if (!eaten)
		speed = initSpeed;

	// Movement
	switch (ghostHouse)
	{
	case GhostHouse::INSIDE:
	{
		if (player->noPelletTimer <= noPelletTimerUnlock && player->pelletCounter <= pelletCounterUnlock)
		{
			GhostHouseMoving();
		}
		else
		{
			ghostHouse = GhostHouse::GOING_SIDE;
			player->noPelletTimer = 0.0f;
		}
		break;
	}
	case GhostHouse::GOING_SIDE:
	{
		if (cmp(position, { 15.5f, 14.0f }))
		{
			ghostHouse = GhostHouse::GOING_UP;
		}
		else
		{
			// Ghost goes to center of the house
			position.y = 14.0f;
			if (initPos == olc::vf2d(13.5f, 14.0f))
				newDir = { 1, 0 };
			else if (initPos == olc::vf2d(17.5f, 14.0f))
				newDir = {-1, 0};
			//newDir = initDir;
		}
		break;
	}
	case GhostHouse::GOING_UP:
	{
		if (cmp(position, { 15.5f, 11.0f }))
		{
			position = { 15.5f, 11.0f };
			newDir = { -1, 0 };
			ghostHouse = GhostHouse::OUTSIDE;
		}
		else
		{
			// Ghost is heading out
			newDir = { 0, -1 };
		}
		break;
	}
	default:
	{
		WallHit(level);

		if (mode != GhostMode::FRIGHTENED || eaten)
			Move(level);
		break;
	}
	}

	direction = newDir;

	switch (ghostHouse)
	{
	case GhostHouse::INSIDE: case GhostHouse::GOING_UP: position.y = newPos.y; break;
	case GhostHouse::GOING_SIDE: position.x = newPos.x; break;
	default:
		position = newPos;
	}

	CheckDirection();
	Animate(tileID.x, tileID.x + 2, fElapsedTime, 0.09f);
}

void Ghost::ChangeMode(float fElapsedTime, bool isEnergized)
{
	if (ghostHouse == GhostHouse::OUTSIDE)
		timer += fElapsedTime;

	if (!eaten)
	{
		if (timer > 7.0f && mode == GhostMode::SCATTER)
		{
			mode = GhostMode::CHASE;
			timer = 0.0f;
		}

		if (timer > 20.0f && mode == GhostMode::CHASE)
		{
			mode = GhostMode::SCATTER;
			timer = 0.0f;
		}
	}


	if (isEnergized && !eaten)
	{		
		mode = GhostMode::FRIGHTENED;		
	}

	if (mode == GhostMode::FRIGHTENED && !isEnergized)
	{
		eaten = false;
		mode = lastMode;
		timer = lastTimer;
		tileID.y = initTileID.y;
	}
}

void Ghost::CheckTargetTileAndMode(Level* level, Player* player, short ghostID, const olc::vi2d& blinkyPos)
{
	olc::vi2d chaseTile;
	olc::vi2d scatterTile;

	switch (ghostID)
	{
	case 0:
	{
		chaseTile = player->position;
		scatterTile = { 26, 3 };
		break;
	}
	case 1:
	{
		if (player->direction.y == -1)
			chaseTile = olc::vi2d(player->position.x, player->position.y - 4);
		else if (player->direction.x == -1)
			chaseTile = olc::vi2d(player->position.x - 4, player->position.y);
		else if (player->direction.y == 1)
			chaseTile = olc::vi2d(player->position.x, player->position.y + 4);
		else if (player->direction.x == 1)
			chaseTile = olc::vi2d(player->position.x + 4, player->position.y);

		scatterTile = { 1, 3 };

		break;
	}
	case 2:
	{
		olc::vi2d twoTilesAheadPlayer;
		if (player->direction.y == -1)
			twoTilesAheadPlayer = olc::vi2d(player->position.x, player->position.y - 2);
		else if (player->direction.x == -1)
			twoTilesAheadPlayer = olc::vi2d(player->position.x - 2, player->position.y);
		else if (player->direction.y == 1)
			twoTilesAheadPlayer = olc::vi2d(player->position.x, player->position.y + 2);
		else if (player->direction.x == 1)
			twoTilesAheadPlayer = olc::vi2d(player->position.x + 2, player->position.y); 

		olc::vi2d ghostToPlayerDistance = blinkyPos - twoTilesAheadPlayer;
		float doubleDistance = ghostToPlayerDistance.mag2() * 2;

		int XPosPow2 = doubleDistance - std::pow(blinkyPos.x, 2);
		int YPosPow2 = doubleDistance - std::pow(blinkyPos.y, 2);

		if (mode == GhostMode::CHASE)
			chaseTile = olc::vi2d((int)std::sqrt(XPosPow2), (int)std::sqrt(YPosPow2));
		scatterTile = { 1, 27 };
		break;
	}
	case 3:
	{
		olc::vi2d ghostFromPacman = position - player->position;
		float distance = ghostFromPacman.mag();

		if (distance <= 4.0f)
			chaseTile = { 13, 23 };
		else
			chaseTile = player->position;

		scatterTile = { 26, 27 };
		break;
	}
	}

	if (!eaten)
	{
		switch (mode)
		{
		case GhostMode::SCATTER: targetTile = scatterTile; break;
		case GhostMode::CHASE: targetTile = chaseTile; break;
		case GhostMode::FRIGHTENED:
		{
			FrightenedMode(level, player->energizedTimer);
			break;
		}
		}
	}
	else
		RetreatToHouse();
}

void Ghost::CheckHouseMode()
{
	if (position == olc::vf2d(17.5f, 14.0f) ||
		position == olc::vf2d(13.5f, 14.0f))
		ghostHouse = GhostHouse::INSIDE;
	else if (position == olc::vf2d(15.5f, 14.0f))
		ghostHouse = GhostHouse::GOING_UP;
	else if (position == olc::vf2d(15.5f, 11.0f))
		ghostHouse = GhostHouse::OUTSIDE;
}

void Ghost::GhostHouseMoving()
{
	if (initPos == olc::vf2d(13.5f, 14.0f) || 
		initPos == olc::vf2d(17.5f, 14.0f))
	{		
		if (cmp(position, { initPos.x, initPos.y + 0.5f }, 0.2f))
		{
			newDir = { 0, -1 };
		}
		else if (cmp(position, { initPos.x, initPos.y - 0.5f }, 0.2f))
		{
			newDir = { 0, 1 };
		}
	}	
}

void Ghost::Move(Level* level)
{
	for (olc::vi2d& crossPos : level->crossOverPos)
	{
		olc::vf2d targetFromLeft = targetTile - olc::vf2d(newPos.x - 1, position.y);
		olc::vf2d targetFromRight = targetTile - olc::vf2d(newPos.x + 1, position.y);
		olc::vf2d targetFromUp = targetTile - olc::vf2d(position.x, newPos.y - 1);
		olc::vf2d targetFromDown = targetTile - olc::vf2d(position.x, newPos.y + 1);

		float upDistance = targetFromUp.mag();
		float leftDistance = targetFromLeft.mag();
		float downDistance = targetFromDown.mag();
		float rightDistance = targetFromRight.mag();

		float shortestDistance = 0.0f;

		if (cmp(newPos, crossPos) && prevCrossPos != crossPos)
		{
			if (level->IsWall(olc::vi2d(crossPos.x, crossPos.y - 1))) upDistance = 999.0f;
			if (level->IsWall(olc::vi2d(crossPos.x - 1, crossPos.y))) leftDistance = 999.0f;
			if (level->IsWall(olc::vi2d(crossPos.x, crossPos.y + 1))) downDistance = 999.0f;
			if (level->IsWall(olc::vi2d(crossPos.x + 1, crossPos.y))) rightDistance = 999.0f;

			if (direction.y == -1)
				shortestDistance = std::min(fminf(leftDistance, rightDistance), upDistance);
			else if (direction.x == -1)
				shortestDistance = std::min(leftDistance, fminf(upDistance, downDistance));
			else if (direction.y == 1)
				shortestDistance = std::min(fminf(leftDistance, rightDistance), downDistance);
			else if (direction.x == 1)
				shortestDistance = std::min(rightDistance, fminf(upDistance, downDistance));

			if (upDistance == shortestDistance &&
				direction.y != 1)
			{
				newDir = { 0, -1 };
			}
			else if (leftDistance == shortestDistance &&
				direction.x != 1)
			{
				newDir = { -1, 0 };
			}
			else if (downDistance == shortestDistance &&
				direction.y != -1)
			{
				newDir = { 0, 1 };
			}
			else if (rightDistance == shortestDistance &&
				direction.x != -1)
			{
				newDir = { 1, 0 };
			}
			prevCrossPos = crossPos;
		}
	}
}

void Ghost::WallHit(Level* level)
{
	if (velocity.y < 0)
	{
		if (level->IsWall(olc::vi2d(position.x, newPos.y)))
		{
			newPos.y = (int)newPos.y + 1;
		}
	}
	else if (velocity.x < 0)
	{
		if (level->IsWall(olc::vi2d(newPos.x, position.y)))
		{
			newPos.x = (int)newPos.x + 1;
		}
		if (level->GetTile(olc::vi2d(newPos.x, position.y)) == '.')
		{
			newPos.x = 30.0f;
		}
	}
	else if (velocity.y > 0)
	{
		if (level->IsWall(olc::vi2d(position.x, newPos.y + 1)))
		{
			newPos.y = (int)newPos.y;
		}
	}
	else if (velocity.x > 0)
	{
		if (level->IsWall(olc::vi2d(newPos.x + 1, position.y)))
		{
			newPos.x = (int)newPos.x;
		}
		if (level->GetTile(olc::vi2d(newPos.x + 1, position.y)) == '.')
		{
			newPos.x = 1.0f;
		}
	}
}

void Ghost::CheckDirection()
{
	if (direction.x == -1)
	{
		tileID.x = 0;
		velocity = { -speed, 0.0f };
	}
	else if (direction.x == 1)
	{
		tileID.x = 2;
		velocity = { speed, 0.0f };
	}
	else if (direction.y == -1)
	{
		tileID.x = 4;
		velocity = { 0.0f, -speed };
	}
	else if (direction.y == 1)
	{
		tileID.x = 6;
		velocity = { 0.0f, speed };
	}
}

void Ghost::FrightenedMode(Level* level, float energizedTimer)
{
	if (InitFright)
	{
		lastTimer = timer;
		timer = 0.0f;
		prevCrossPos = { 0, 0 };
		newDir *= -1;
		InitFright = false;
		tileID.y = 4;
	}

	for (olc::vi2d& crossPos : level->crossOverPos)
	{
		if (cmp(newPos, crossPos) && prevCrossPos != crossPos)
		{
			int dirID = rand() % 4;
			bool isNewDir = false;

			while (!isNewDir)
			{
				if (direction.y != 1 && dirID == 0)
				{
					if (!level->IsWall(olc::vi2d(crossPos.x, crossPos.y - 1)))
					{
						newDir = { 0, -1 };
						isNewDir = true;
					}
					else
					{
						dirID = GetRandomExcluding(0);						
					}
				}
				else if ((direction.y == 1 && dirID == 0))
					dirID = GetRandomExcluding(0);

				if (direction.x != 1 && dirID == 1)
				{
					if (!level->IsWall(olc::vi2d(crossPos.x - 1, crossPos.y)))
					{
						newDir = { -1, 0 };
						isNewDir = true;
					}
					else
					{
						dirID = GetRandomExcluding(1);						
					}
				}
				else if ((direction.x == 1 && dirID == 1))
					dirID = GetRandomExcluding(1);

				if (direction.y != -1 && dirID == 2)
				{
					if (!level->IsWall(olc::vi2d(crossPos.x, crossPos.y + 1)))
					{
						newDir = { 0, 1 };
						isNewDir = true;
					}
					else
					{
						dirID = GetRandomExcluding(2);						
					}
				}
				else if ((direction.y == -1 && dirID == 2))
					dirID = GetRandomExcluding(2);

				if (direction.x != -1 && dirID == 3)
				{
					if (!level->IsWall(olc::vi2d(crossPos.x + 1, crossPos.y)))
					{
						newDir = { 1, 0 };
						isNewDir = true;
					}
					else
					{
						dirID = GetRandomExcluding(3);						
					}
				}
				else if ((direction.x == -1 && dirID == 3))
					dirID = GetRandomExcluding(3);
			}
			
			prevCrossPos = crossPos;			
		}
	}

	if (energizedTimer > 4.0f)
	{
		if (timer > 0.2f)
		{
			timer = 0.0f;
			if (tileID.y == 4)
				tileID.y = 5;
			else if (tileID.y == 5)
				tileID.y = 4;
		}
	}
}

void Ghost::RetreatToHouse()
{
	mode = lastMode;
	tileID.y = 6;
	speed = 2 * initSpeed;			
	targetTile = olc::vf2d(15.5f, 11.0f);

	if (cmp(position, { 15.5f, 11.0f }, 0.5f))
	{
		position = { 15.5f, 11.0f };
		newDir = {0, 1};		
	}

	if (cmp(position, { 15.5f, 14.0f }, 0.5f))
	{
		Restart();
		eaten = false;
	}
}

void Ghost::Restart()
{
	if (eaten)
	{
		if (initPos == olc::vf2d(15.5f, 11.0f) || 
			initPos == olc::vf2d(15.5f, 14.0f))
		{
			ghostHouse = GhostHouse::GOING_UP;
			position = { 15.5f, 14.0f };
		}
		else if (initPos == olc::vf2d(13.5f, 14.0f) ||
			initPos == olc::vf2d(17.5f, 14.0f))
		{
			ghostHouse = GhostHouse::GOING_SIDE;
			position = initPos;
		}
		mode = lastMode;
		timer = lastTimer;
	}
	else
	{
		position = initPos;

		if (initGhostHouse == GhostHouse::OUTSIDE)
		{
			direction = { -1, 0 };
		}
		else
			direction = initDir;

		speed = initSpeed;
		newPos = initPos;
		newDir = direction;

		ghostHouse = initGhostHouse;

		mode = GhostMode::SCATTER;

		timer = 0.0f;

	}

	CheckHouseMode();
	GhostHouseMoving();
	CheckDirection();
	tileID = initTileID;	
}

int GetRandomExcluding(int num)
{
	int res = rand() % 4;
	if (res == num)
		res = GetRandomExcluding(num);
	else
		return res;
}
