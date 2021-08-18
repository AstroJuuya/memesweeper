#pragma once

#include "Graphics.h"
#include "Vei2.h"
#include "SpriteCodex.h"

#include <random>

class MemeField
{
public:
	enum class Tile
	{
		Hidden,
		Revealed,
		Flag,
		Meme
	};
public:
	MemeField( int nMemes );
	void Draw( Graphics& gfx );
	void Reveal( const Vei2& gridPos );
	const Tile& GetTile( const Vei2& gridPos ) const;
	
private:
	Tile& SetTile( const Vei2& gridPos );
	const Vei2& ToVei2( const int index ) const;
	const int ToIndex( const Vei2& gridPos ) const;

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	const Vei2 offset = { 
		Graphics::ScreenWidth	/ 2 - SpriteCodex::tileSize * width / 2,
		Graphics::ScreenHeight	/ 2 - SpriteCodex::tileSize * height / 2
	};
	std::mt19937 rng;
	Tile board[width * height] = { Tile::Hidden };
};