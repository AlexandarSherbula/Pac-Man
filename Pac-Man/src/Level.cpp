#include "Level.h"

Level::Level(olc::PixelGameEngine* pge)
	: Object(pge)
{
	Map ="__1555555555555hi5555555555552__"
		"__7x0000x00000xjkx00000x0000x8__"
		"__70amm90ammm90jk0ammm90amm908__"
		"__7*j__k0j___k0jk0j___k0j__k*8__"
		"__70cllb0clllb0cb0clllb0cllb08__"
		"__7x0000x00x00x00x00x00x0000x8__"
		"__70amm90a90ammmmmm90a90amm908__"
		"__70cllb0jk0clldellb0jk0cllb08__"
		"__7x0000xjkx00xjkx00xjkx0000x8__"
		"__3666690jfmm9_jk_ammgk0a66664__"
		"_______70jellb_cb_clldk08_______"
		"_______70jk+__+__+__+jk08_______"
		"_______70jk_s6w--y6t_jk08_______"
		"_555555b0cb_8______7_cb0c555555_"
		"._______x__+8______7+__x_______."
		"_66666690a9_8______7_a90a666666_"
		"_______70jk_v555555u_jk08_______"
		"_______70jk+________+jk08_*_*___"
		"_______70jk_ammmmmm9_jk08_______"
		"__15555b0cb_clldellb_cb0c55552__"
		"__7x0000x00x00xjkx00x00x0000x8__"
		"__70amm90ammm90jk0ammm90amm908__"
		"__70cldk0clllb0cb0clllb0jelb08__"
		"__7X0xjkx00x00x__x00x00xjkx0X8__"
		"__nm90jk0a90ammmmmm90a90jk0amr__"
		"__plb0cb0jk0clldellb0jk0cb0clq__"
		"__7x0x00xjkx00xjkx00xjkx00x0x8__"
		"__70ammmmgfmm90jk0ammgfmmmm908__"
		"__70cllllllllb0cb0cllllllllb08__"
		"__7x0000000000x00x0000000000x8__"
		"__3666666666666666666666666664__";

	levelWidth = 32;
	levelHeight = 31;

	int i = 0;
	for (int x = 0; x < levelWidth; x++)
		for (int y = 0; y < levelHeight; y++)
			if (GetTile(olc::vi2d(x, y)) == '+' ||
				GetTile(olc::vi2d(x, y)) == 'x' ||
				GetTile(olc::vi2d(x, y)) == 'X')
			{
				crossOverPos[i] = olc::vi2d(x, y);
				i++;
			}

	blackRects = std::make_unique<Object>(pge);
	blackRects->SetSprite("Rectangle", true);

	powerPelletFlick = true;
	powerPelletFlickTime = 0.0f;
}

void Level::Update(float fElapsedTime)
{
	powerPelletFlickTime += fElapsedTime;

	if (powerPelletFlickTime > 0.2f)
	{
		powerPelletFlick = !powerPelletFlick;
		powerPelletFlickTime = 0.0f;
	}
}

bool Level::IsWall(const olc::vi2d& id)
{
	return	GetTile(id) != '_' && GetTile(id) != '0' &&
			GetTile(id) != '*' && GetTile(id) != '+' &&
			GetTile(id) != '.' && GetTile(id) != 'x' &&
			GetTile(id) != 'X' && GetTile(id) != '-';
}

char Level::GetTile(const olc::vi2d& id)
{
	if (id.x >= 0 && id.x < levelWidth && id.y >= 0 && id.y < levelHeight)
		return Map[id.y * levelWidth + id.x];
	else
		return ' ';
}

void Level::SetTile(const olc::vi2d& id, char c)
{
	if (id.x >= 0 && id.x < levelWidth && id.y >= 0 && id.y < levelHeight)
		Map[id.y * levelWidth + id.x] = c;
}

void Level::Draw()
{
	for (int y = 0; y < levelHeight; y++)
	{
		for (int x = 0; x < levelWidth; x++)
		{
			char TileID = GetTile(olc::vi2d(x, y));
			switch (TileID)
			{
			case '*': case 'X': if (powerPelletFlick) FillCircle({ 184 + x * 16, 48 + y * 16 }, 8, olc::Pixel(255, 255, 155)); break;
			case '0': case 'x': FillRect({ 182 + x * 16, 46 + y * 16 }, olc::vi2d(4, 4), olc::Pixel(255, 255, 155)); break;
			case '1': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 0, 0 }, { 16, 16 }, olc::BLUE); break;
			case '2': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 1, 0 }, { 16, 16 }, olc::BLUE); break;
			case '3': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 2, 0 }, { 16, 16 }, olc::BLUE); break;
			case '4': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 3, 0 }, { 16, 16 }, olc::BLUE); break;
			case '5': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 4, 0 }, { 16, 16 }, olc::BLUE); break;
			case '6': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 0, 1 }, { 16, 16 }, olc::BLUE); break;
			case '7': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 1, 1 }, { 16, 16 }, olc::BLUE); break;
			case '8': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 2, 1 }, { 16, 16 }, olc::BLUE); break;
			case '9': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 3, 1 }, { 16, 16 }, olc::BLUE); break;
			case 'a': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 4, 1 }, { 16, 16 }, olc::BLUE); break;
			case 'b': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 0, 2 }, { 16, 16 }, olc::BLUE); break;
			case 'c': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 1, 2 }, { 16, 16 }, olc::BLUE); break;
			case 'd': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 2, 2 }, { 16, 16 }, olc::BLUE); break;
			case 'e': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 3, 2 }, { 16, 16 }, olc::BLUE); break;
			case 'f': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 4, 2 }, { 16, 16 }, olc::BLUE); break;
			case 'g': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 0, 3 }, { 16, 16 }, olc::BLUE); break;
			case 'h': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 1, 3 }, { 16, 16 }, olc::BLUE); break;
			case 'i': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 2, 3 }, { 16, 16 }, olc::BLUE); break;
			case 'j': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 3, 3 }, { 16, 16 }, olc::BLUE); break;
			case 'k': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 4, 3 }, { 16, 16 }, olc::BLUE); break;
			case 'l': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 0, 4 }, { 16, 16 }, olc::BLUE); break;
			case 'm': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 1, 4 }, { 16, 16 }, olc::BLUE); break;
			case 'n': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 2, 4 }, { 16, 16 }, olc::BLUE); break;
			case 'o': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 3, 4 }, { 16, 16 }, olc::BLUE); break;
			case 'p': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 4, 4 }, { 16, 16 }, olc::BLUE); break;
			case 'q': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 0, 5 }, { 16, 16 }, olc::BLUE); break;
			case 'r': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 1, 5 }, { 16, 16 }, olc::BLUE); break;
			case 's': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 2, 5 }, { 16, 16 }, olc::BLUE); break;
			case 't': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 3, 5 }, { 16, 16 }, olc::BLUE); break;
			case 'u': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 4, 5 }, { 16, 16 }, olc::BLUE); break;
			case 'v': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 0, 6 }, { 16, 16 }, olc::BLUE); break;
			case 'w': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 1, 6 }, { 16, 16 }, olc::BLUE); break;
			case 'y': DrawPartial({ 176.0f + x * 16, 40.0f + y * 16 }, { 2, 6 }, { 16, 16 }, olc::BLUE); break;
			case '-': FillRect({ 176 + x * 16, 50 + y * 16 }, { 16, 3 }); break;
			}
		}
	}

	blackRects->Draw(olc::vf2d(41.0f * 16.0f, 15.5f * 16.0f), olc::BLACK);
	blackRects->Draw(olc::vf2d(10.0f * 16.0f, 15.5f * 16.0f), olc::BLACK);
}
