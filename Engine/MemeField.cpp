#include "MemeField.h"

#include <assert.h>

MemeField::MemeField( int nMemes )
	:
	rng( std::random_device()() )
{
	std::uniform_int_distribution<int>newMemeX (0, width);
	std::uniform_int_distribution<int>newMemeY (0, height);

	for ( int i = 0; i < nMemes; i++ )
	{
		Vei2 newMeme( newMemeX(rng), newMemeY(rng) );
		while ( GetTile( newMeme ) == Tile::Meme )
		{
			newMeme = Vei2(newMemeX(rng), newMemeY(rng));
		}
		assert( GetTile( newMeme ) != Tile::Meme );
		SetTile( newMeme ) = Tile::Meme;
	}
}

void MemeField::Draw( Graphics& gfx )
{
	gfx.DrawRect(
		offset.x,
		offset.y,
		offset.x + SpriteCodex::tileSize * width,
		offset.y + SpriteCodex::tileSize * height,
		Color( 192, 192, 192 ) 
	);

	for ( int i = 0; i < width * height; i++ )
	{
		switch ( board[i] )
		{
		case Tile::Hidden:
			SpriteCodex::DrawTileButton( offset + ToVei2( i ) * SpriteCodex::tileSize, gfx );
			break;
		case Tile::Flag:
			SpriteCodex::DrawTileFlag(offset + ToVei2(i) * SpriteCodex::tileSize, gfx);
			break;
		case Tile::Revealed:
			SpriteCodex::DrawTileCross(offset + ToVei2(i) * SpriteCodex::tileSize, gfx);
			break;
		case Tile::Meme:
			SpriteCodex::DrawTileMeme(offset + ToVei2(i) * SpriteCodex::tileSize, gfx);
			break;
		}
		
	}
}

void MemeField::Reveal( const Vei2& gridPos )
{
}

const MemeField::Tile& MemeField::GetTile( const Vei2& gridPos ) const
{
	return board[ gridPos.y * width + gridPos.x ];
}

MemeField::Tile& MemeField::SetTile( const Vei2& gridPos )
{
	return board[gridPos.y * width + gridPos.x];
}

const Vei2& MemeField::ToVei2(const int index) const
{
	return Vei2( index % width ,index / width );
}
