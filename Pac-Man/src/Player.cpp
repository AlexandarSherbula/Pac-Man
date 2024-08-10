#include "Player.h"

Player::Player(olc::PixelGameEngine* pge)
	: DynamicObject(pge)
{
	energized = false;
	energizedTimer = 0.0f;

	pelletCounter = 0;
	noPelletTimer = 0.0f;
}

void Player::Update(float fElapsedTime, Level* level)
{
	noPelletTimer += fElapsedTime;	

	if (pge->GetKey(olc::A).bPressed || pge->GetKey(olc::LEFT).bPressed)	newDir = { -1, 0 };
	if (pge->GetKey(olc::D).bPressed || pge->GetKey(olc::RIGHT).bPressed)	newDir = {  1, 0 };
	if (pge->GetKey(olc::W).bPressed || pge->GetKey(olc::UP).bPressed)		newDir = {  0,-1 };
	if (pge->GetKey(olc::S).bPressed || pge->GetKey(olc::DOWN).bPressed)	newDir = {  0, 1 };

	newPos = position + velocity * fElapsedTime;

	WallHit(level);

	Move(level);

	position = newPos;

	CheckDirection();

	if (velocity.x != 0.0f || velocity.y != 0.0f)
		Animate(0, 2, fElapsedTime, 0.045f);
	else if (tileID.x == 0)
			tileID.x = 1;

	if (energized)
	{
		energizedTimer += fElapsedTime;

		if (energizedTimer > 6.0f)
		{
			energizedTimer = 0.0f;
			energized = false;
		}
	}
}

void Player::WallHit(Level* level)
{
	if (velocity.x < 0)
	{
		if (level->IsWall(olc::vi2d(newPos.x, position.y)))
		{
			newPos.x = (int)newPos.x + 1;
			direction = { 0, 0 };
			velocity = { 0.0f, 0.0f };
		}
		if (level->GetTile(olc::vi2d(newPos.x, position.y)) == '.')
		{
			newPos.x = 30.0f;
		}
	}
	else if (velocity.x > 0)
	{
		if (level->IsWall(olc::vi2d(newPos.x + 1, position.y)))
		{
			newPos.x = (int)newPos.x;
			direction = { 0, 0 };
			velocity = { 0.0f, 0.0f };

		}
		if (level->GetTile(olc::vi2d(newPos.x + 1, position.y)) == '.')
		{
			newPos.x = 1.0f;
		}
	}
	else if (velocity.y < 0)
	{
		if (level->IsWall(olc::vi2d(position.x, newPos.y)))
		{
			newPos.y = (int)newPos.y + 1;
			direction = { 0, 0 };
			velocity = { 0.0f, 0.0f };
		}
	}
	else if (velocity.y > 0)
	{
		if (level->IsWall(olc::vi2d(position.x, newPos.y + 1)))
		{
			newPos.y = (int)newPos.y;
			direction = { 0, 0 };
			velocity = { 0.0f, 0.0f };
		}
	}
}

void Player::Move(Level* level)
{
	if ((direction.x == -1 && newDir.x == 1) ||
		(direction.x == 1 && newDir.x == -1))
		direction.x *= -1;
	
	if ((direction.y == -1 && newDir.y == 1) ||
		(direction.y == 1 && newDir.y == -1))
		direction.y *= -1;
	
	//Did Pacman change the direction and the axis
	for (olc::vi2d crossPos : level->crossOverPos)
	{
		//From Left or Right to Up or Down and the other way around
		if (cmp(newPos, (olc::vf2d)crossPos))
		{
			if (newDir.y == -1 && !level->IsWall(olc::vi2d(crossPos.x, crossPos.y - 1)))
			{
				newPos.x = (int)crossPos.x;
				direction = { 0, -1 };
			}
			else if (newDir.y == 1 && !level->IsWall(olc::vi2d(crossPos.x, crossPos.y + 1)))
			{
				newPos.x = (int)crossPos.x;
				direction = { 0, 1 };
			}
	
			if (newDir.x == -1 && !level->IsWall(olc::vi2d(crossPos.x - 1, crossPos.y)))
			{
				newPos.y = (int)crossPos.y;
				direction = { -1, 0 };
			}
			else if (newDir.x == 1 && !level->IsWall(olc::vi2d(crossPos.x + 1, crossPos.y)))
			{
				newPos.y = (int)crossPos.y;
				direction = { 1, 0 };
			}
		}
	}
}

void Player::CheckDirection()
{
	if (direction.x == 0 && direction.y == -1)
	{
		velocity = { 0.0f,	-speed };
		tileID.y = 2;
	}
	if (direction.x == -1 && direction.y == 0)
	{
		velocity = { -speed, 0.0f };
		tileID.y = 0;
	}
	if (direction.x == 0 && direction.y == 1)
	{
		velocity = { 0.0f,	speed };
		tileID.y = 3;
	}
	if (direction.x == 1 && direction.y == 0)
	{
		velocity = { speed,	 0.0f };
		tileID.y = 1;
	}
}

void Player::Restart()
{
	position = initPos;
	newPos = initPos;

	initSpeed = speed;

	direction = initDir;
	newDir = initDir;

	tileID = initTileID;

	energized = false;

	pelletCounter = 0;
	noPelletTimer = 0.0f;

	CheckDirection();
}

bool cmp(olc::vf2d a, olc::vf2d b, float epsilon)
{
	return (a.x >= b.x && a.x < b.x + epsilon &&
		a.y >= b.y && a.y < b.y + epsilon);
}
