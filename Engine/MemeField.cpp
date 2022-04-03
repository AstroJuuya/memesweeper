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
		SpawnMeme( newMeme );
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
		SpriteCodex::DrawTileButton( field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx );
		SpriteCodex::DrawTileFlag( field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx );
		break;
	case State::Revealed:
		switch (proximity)
		{
		case 0:
			SpriteCodex::DrawTile0(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		case 1:
			SpriteCodex::DrawTile1(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		case 2:
			SpriteCodex::DrawTile2(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		case 3:
			SpriteCodex::DrawTile3(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		case 4:
			SpriteCodex::DrawTile4(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		case 5:
			SpriteCodex::DrawTile5(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		case 6:
			SpriteCodex::DrawTile6(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		case 7:
			SpriteCodex::DrawTile7(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		case 8:
			SpriteCodex::DrawTile8(field.offset + field.ToVei2(index) * SpriteCodex::tileSize, gfx);
			break;
		}
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

void MemeField::Tile::ToggleFlag()
{
	assert( state != State::Revealed && state != State::Meme );
	if ( state == State::Hidden )
	{
		state = State::Flag;
	}
	else if ( state == State::Flag )
	{
		state = State::Hidden;
	}
}

void MemeField::Tile::NewMeme()
{
	assert( !hasMeme );
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Reveal()
{
	if ( hasMeme )
	{
		state = State::Meme;
	}
	else
	{
		state = State::Revealed;
	}
}

int MemeField::Tile::GetProximity() const
{
	return proximity;
}

void MemeField::Tile::IncProximity()
{
	assert( proximity >= 0 && proximity <= 8 );
	proximity += 1;
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

void MemeField::SpawnMeme( Vei2& gridPos )
{
	GetTile( gridPos ).NewMeme();
	for ( int y = -1; y <= 1; y++ )
	{
		for ( int x = -1; x <= 1; x++ )
		{
			if ( !GetTile( gridPos + Vei2( x, y ) ).HasMeme() )
			{
				GetTile( gridPos + Vei2( x, y ) ).IncProximity();
			}
		}
	}
}

void MemeField::Reveal( Vei2& gridPos )
{
	GetTile( gridPos ).Reveal();

	if (GetTile(gridPos).GetProximity() == 0 && !GetTile(gridPos).HasMeme())
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				if (
					!GetTile(gridPos + Vei2(x, y)).HasMeme() &&
					GetTile(gridPos + Vei2(x, y)).GetState() == Tile::State::Hidden &&
					GetTile(gridPos + Vei2(x, y)).GetProximity() == 0
					)
				{
					Reveal((gridPos + Vei2(x, y)));
				}
				else if (
					!GetTile(gridPos + Vei2(x, y)).HasMeme() &&
					GetTile(gridPos + Vei2(x, y)).GetState() == Tile::State::Hidden &&
					GetTile(gridPos + Vei2(x, y)).GetProximity() > 0
					)
				{
					GetTile(gridPos + Vei2(x, y)).Reveal();
				}
			}
		}
	}
}

MemeField::Tile& MemeField::GetTile( const Vei2& gridPos )
{
	if (
		gridPos.x	>=	0		&&
		gridPos.x	<	width	&&
		gridPos.y	>=	0		&&
		gridPos.y	<	height
		)
	{
		return board[ToIndex(gridPos)];
	}
}

const bool MemeField::IsOnBoard(const Vei2& screenPos) const
{
	return RectI(screenPos, screenPos).IsOverlappingWith(rect);
}

MemeField::Tile& MemeField::TileAt(const Vei2& screenPos)
{
	if ( IsOnBoard( screenPos ) )
	{
		const Vei2 relPos = screenPos - offset;
		return GetTile( relPos / SpriteCodex::tileSize );
	}
}

Vei2& MemeField::TileAtPos(const Vei2& screenPos) const
{
	if (IsOnBoard(screenPos))
	{
		const Vei2 relPos = screenPos - offset;
		return relPos / SpriteCodex::tileSize;
	}
}

Vei2& MemeField::ToVei2(const int index) const
{
	return Vei2( index % width ,index / width );
}

const int MemeField::ToIndex(const Vei2& gridPos) const
{
	return gridPos.y * width + gridPos.x;
}