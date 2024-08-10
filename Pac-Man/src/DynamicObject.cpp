#include "DynamicObject.h"

DynamicObject::DynamicObject(olc::PixelGameEngine* pge)
	: Object(pge){}

void DynamicObject::SetMovement(const olc::vf2d& position, float speed, const olc::vi2d& direction)
{
	SetPosition(position);
	this->speed = speed;
	this->direction = direction;

	initSpeed = speed;

	initPos = position;
	newPos = position;
	initDir = direction;
	newDir = direction;

	tileID = initTileID;

	CheckDirection();
}

void DynamicObject::SetTileID(const olc::vi2d& tileID)
{
	this->tileID = tileID;
	this->initTileID = tileID;
}

void DynamicObject::CheckDirection()
{
	if (direction.x == 0 && direction.y  == -1)	velocity = { 0.0f,	-speed };
	if (direction.x == -1 && direction.y == 0)	velocity = { -speed, 0.0f };
	if (direction.x == 0 && direction.y  == 1)	velocity = { 0.0f,	speed };
	if (direction.x == 1 && direction.y  == 0)	velocity = { speed,	 0.0f };
}

void DynamicObject::Restart()
{	
}
