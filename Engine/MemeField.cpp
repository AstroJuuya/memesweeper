#include "MemeField.h"

#include <assert.h>

MemeField::MemeField( int nMemes )
	:
	rng( std::random_device()() )
{
	std::uniform_int_distribution<int>newMemeX (0, width - 1);
	std::uniform_int_distribution<int>newMemeY (0, height - 1);

	for ( int i = 0; i < nMemes; i++ )
	{
		Vei2 newMeme( newMemeX(rng), newMemeY(rng) );
		while ( GetTile( newMeme ).HasMeme() == true )
		{
			newMeme = Vei2(newMemeX(rng), newMemeY(rng));
		}
		assert( GetTile( newMeme ).HasMeme() == false );
		GetTile( newMeme ).NewMeme();
	}
}

void MemeField::Tile::Draw( const MemeField& field, const int index, Graphics& gfx)
{
	switch (state)
	{
	case State::Hidden:
		SpriteCodex::DrawTileButton( field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx );
		break;
	case State::Flag:
		SpriteCodex::DrawTileFlag( field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx );
		break;
	case State::Revealed:
		SpriteCodex::DrawTile0( field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx );
		break;
	case State::Meme:
		SpriteCodex::DrawTileMeme( field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx );
		break;
	}
}

MemeField::Tile::State MemeField::Tile::GetState() const
{
	return state;
}

void MemeField::Tile::NewMeme()
{
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Reveal()
{
	if (hasMeme)
	{
		state = State::Meme;
	}
	else
	{
		state = State::Revealed;
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
		board[i].Draw( *this , i, gfx );
	}
}

MemeField::Tile& MemeField::GetTile( const Vei2& gridPos )
{
	return board[ ToIndex( gridPos ) ];
}

Vei2& MemeField::ToVei2(const int index) const
{
	return Vei2( index % width ,index / width );
}

const int MemeField::ToIndex(const Vei2& gridPos) const
{
	return gridPos.y * width + gridPos.x;
}