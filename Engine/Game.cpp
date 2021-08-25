/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	mf( nMemes ),
	rng( std::random_device()() )
{
	/*
	// Reveals 100 random tiles for testing purposes
	std::uniform_int_distribution<int> gridPos ( 0, 20 );
		for ( int rev = 100; rev > 0; )
	{
		const Vei2 tile = Vei2(gridPos(rng), gridPos(rng));
		if ( mf.GetTile( tile ).GetState() != MemeField::Tile::State::Revealed )
		{
			mf.GetTile( tile ).Reveal();
			rev--;
		}
	}
	*/
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	// TODO: Implement mouse button hovering function / mousepress indecidedness
	if ( wnd.mouse.LeftIsPressed() && !mouseCooldown )
	{
		const Vei2 mousePos = Vei2( wnd.mouse.GetPosX(), wnd.mouse.GetPosY() );
		if ( mf.IsOnBoard( mousePos ) )
		{
			if ( mf.TileAt( mousePos ).GetState() != MemeField::Tile::State::Flag )
			{
				mf.TileAt( mousePos ).Reveal();
			}
		}
		mouseCooldown = true;
	}
	else if ( wnd.mouse.RightIsPressed() && !mouseCooldown )
	{
		const Vei2 mousePos = Vei2(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
		if ( mf.IsOnBoard( mousePos ) )
		{
			if ( 
				mf.TileAt( mousePos ).GetState()== MemeField::Tile::State::Hidden || 
				mf.TileAt( mousePos ).GetState()== MemeField::Tile::State::Flag 
				)
			{
				mf.TileAt( mousePos ).ToggleFlag();
			}
		}
		mouseCooldown = true;
	}
	else if ( !wnd.mouse.LeftIsPressed() && !wnd.mouse.RightIsPressed() && mouseCooldown )
	{
		mouseCooldown = false;
	}
}

void Game::ComposeFrame()
{
	mf.Draw( gfx );
}
