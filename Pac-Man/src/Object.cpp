#include "Object.h"

ImageAssets::ImageAssets()
{

}

void ImageAssets::LoadSprites()
{
	auto load = [&](std::string name, std::string filename)
	{
		//std::shared_ptr<olc::Sprite> s = 
		Sprite[name] = std::make_shared<olc::Sprite>(filename);
	};

	load("Pacman",      "assets/PacmanSpriteSheet.png");
	load("Ghost",       "assets/GhostSpriteSheet.png");
	load("PowerUp",     "assets/PowerUp.png");
	load("Wall",        "assets/WallSpriteSheet.png");
	load("Rectangle",   "assets/Rect.png");
	load("DeadPacman",  "assets/PacmanDyingSpriteSheet.png");
	load("DeadPacman2", "assets/PacmanDyingSpriteSheet2.png");
}

void ImageAssets::Clear()
{
}

Object::Object(olc::PixelGameEngine* pge)
{
	this->pge = pge;
}

Object::Object(olc::PixelGameEngine* pge, const olc::vf2d& position)
{
	this->pge = pge;
	this->position = position;
}

Object::~Object()
{
}

void Object::SetSprite(const std::string& name, bool useDecal)
{
	sprite = ImageAssets::get().Sprite[name];
	this->useDecal = useDecal;

	if (this->useDecal)
		decal = std::make_unique<olc::Decal>(sprite.get());
}

void Object::SetPosition(const olc::vf2d& position)
{
	this->position = position;
}

void Object::Animate(int firstTileID, int lastTileID, float fElapsedTime, float timeBetweenFrames)
{
	timer += fElapsedTime;
	if (timer >= timeBetweenFrames)
	{
		timer -= timeBetweenFrames;
		if (tileID.x == lastTileID)
			tileID.x = firstTileID;
		else
			tileID.x++;
	}
}

void Object::Draw(const olc::vf2d& position, const olc::Pixel& color)
{
	if (useDecal)
		pge->DrawDecal(position, decal.get(), { 1.0f, 1.0f }, color);
	else
		pge->DrawSprite(position, sprite.get());
}

void Object::DrawPartial(const olc::vf2d& position, const olc::vi2d& sourceSize, const olc::Pixel& color)
{
	if (useDecal)
		pge->DrawPartialDecal(position, sourceSize, decal.get(), tileID * sourceSize, sourceSize, color);
	else
		pge->DrawPartialSprite(position, sprite.get(), tileID * sourceSize, sourceSize);
}

void Object::DrawPartial(const olc::vf2d& position, const olc::vi2d& tileID, const olc::vi2d& sourceSize, const olc::Pixel& color)
{
	if (useDecal)
		pge->DrawPartialDecal(position, sourceSize, decal.get(), tileID * sourceSize, sourceSize, color);
	else
		pge->DrawPartialSprite(position, sprite.get(), tileID * sourceSize, sourceSize);
}

void Object::FillRect(const olc::vi2d& position, const olc::vi2d& size, const olc::Pixel& color)
{
	pge->FillRect(position, size, color);
}

void Object::FillCircle(const olc::vi2d& position, int32_t radius, const olc::Pixel& color)
{
	pge->FillCircle(position, radius, color);
}