#pragma once

#include "Graphics.h"
#include "Vei2.h"
#include "SpriteCodex.h"

#include <random>

class MemeField
{
public:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Revealed,
			Flag,
			Meme
		};
		// Functions
	public:
		void Draw(const MemeField& field, const int index, Graphics& gfx);
		void NewMeme();
		State GetState() const;
		bool HasMeme() const;
		void Reveal();

		// Variables
	private:
		State state = State::Hidden;
		bool hasMeme = false;
	};
// Functions
public:
	MemeField( int nMemes );
	void Draw( Graphics& gfx );
	Vei2& ToVei2(const int index) const;
	const int ToIndex(const Vei2& gridPos) const;
	Tile& GetTile(const Vei2& gridPos);

// Variables
public:
	const Vei2 offset = {
		Graphics::ScreenWidth / 2 - SpriteCodex::tileSize * width / 2,
		Graphics::ScreenHeight / 2 - SpriteCodex::tileSize * height / 2
	};

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	std::mt19937 rng;
	Tile board[width * height];
};