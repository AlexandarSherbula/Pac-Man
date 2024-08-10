#pragma once
#include "olcPixelGameEngine.h"

#include <memory>

class ImageAssets
{
public:
	static ImageAssets& get()
	{
		static ImageAssets me;
		return me;
	}

	ImageAssets(ImageAssets const&) = delete;
	void operator=(ImageAssets const&) = delete;

	std::map<std::string, std::shared_ptr<olc::Sprite>> Sprite;

	void Clear();

	void LoadSprites();
private:
	ImageAssets();
};

class Object
{
public:
	olc::vf2d position;
	olc::vi2d tileID;
protected:
	std::shared_ptr<olc::Sprite> sprite;
	std::shared_ptr<olc::Decal> decal;
	bool useDecal;
	olc::PixelGameEngine* pge;
private:
	float timer;
public:
	Object(olc::PixelGameEngine* pge);
	Object(olc::PixelGameEngine* pge, const olc::vf2d& position);
	~Object();

	void SetSprite(const std::string& name, bool useDecal);
	void SetPosition(const olc::vf2d& position);

	void Animate(int firstTileID, int lastTileID, float fElapsedTime, float timeBetweenFrames);

	void Draw(const olc::vf2d& position, const olc::Pixel& color = olc::WHITE);
	void DrawPartial(const olc::vf2d& position, const olc::vi2d& sourceSize, const olc::Pixel& color = olc::WHITE);
	void DrawPartial(const olc::vf2d& position, const olc::vi2d& tileID, const olc::vi2d& sourceSize, const olc::Pixel& color = olc::WHITE);

	void FillRect(const olc::vi2d& position, const olc::vi2d& size, const olc::Pixel& color = olc::WHITE);
	void FillCircle(const olc::vi2d& position, int32_t radius, const olc::Pixel& color = olc::WHITE);
};