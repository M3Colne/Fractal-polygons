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

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	poly.resize(6);
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
	//Controls
	if (wnd.kbd.KeyIsPressed('W'))
	{
		angle += 0.0075f;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		angle -= 0.0075f;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		if (!inhibPolyIncr)
		{
			inhibPolyIncr = true;
			poly.resize(poly.size() + 1);
		}
	}
	else
	{
		inhibPolyIncr = false;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		if (!inhibPolyDecr)
		{
			inhibPolyDecr = true;
			poly.resize(poly.size() - 1);
		}
	}
	else
	{
		inhibPolyDecr = false;
	}

	if (wnd.kbd.KeyIsPressed('Q'))
	{
		if (!inhibIterIncr)
		{
			inhibIterIncr = true;
			iterations++;
		}
	}
	else
	{
		inhibIterIncr = false;
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		if (!inhibIterDecr)
		{
			inhibIterDecr = true;
			iterations--;
		}
	}
	else
	{
		inhibIterDecr = false;
	}

	if (wnd.kbd.KeyIsPressed(VK_SHIFT))
	{
		angle = 3.1415926f / 2.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		if (!inhibSRotation)
		{
			inhibSRotation = true;
			showRotation = !showRotation;
		}
	}
	else
	{
		inhibSRotation = false;
	}

	//Make the points for the big poly
	for (unsigned int i = 0; i < poly.size(); i++)
	{
		poly[i] = { cos(2 * i * PI / poly.size()) * radius, sin(2 * i * PI / poly.size()) * radius };
		poly[i] = { cos(2 * i * PI / poly.size()) * radius, sin(2 * i * PI / poly.size()) * radius };
	}
}

Vec2 Game::ChangeXY(float x, float y)
{
	return Vec2(x + float(Graphics::ScreenWidth / 2), -y + float(Graphics::ScreenHeight / 2));

	return Vec2(x + float(Graphics::ScreenWidth / 2), -y + float(Graphics::ScreenHeight / 2));
}

Vec2 Game::ChangeXY(Vec2 p)
{
	return Vec2(p.x + Graphics::ScreenWidth / 2, -p.y + Graphics::ScreenHeight / 2);
}

void Game::DrawL(Vec2 p0, Vec2 p1, Color c)
{
	gfx.DrawLine(ChangeXY(p0), ChangeXY(p1), c);
}

void Game::ComposeFrame()
{
	//Recursive draw
	for (int iter = 0; iter < iterations; iter++)
	{
		//Draw poly
		for (unsigned int v = 0; v < poly.size(); v++)
		{
			DrawL(poly[v % poly.size()], poly[(v + 1) % poly.size()], Colors::Yellow);
		}

		//Get rotations and set the next poly from these
		std::vector<Vec2> rotSides(poly.size());
		//Initialize the the rotSides as the sides
		for (unsigned int i = 0; i < rotSides.size(); i++)
		{
			rotSides[i] = poly[(i + 1) % poly.size()] - poly[i % poly.size()];
		}

		if (iter + 1 != iterations) //No need to draw the last legs as they do not form anything
		{
			for (unsigned int i = 0; i < rotSides.size(); i++)
			{
				rotSides[i].Rotate(angle);

				//Show rotation
				if (showRotation && angle != 0.0f)
				{
					DrawL(poly[i], poly[i] + rotSides[i], Colors::LightGray);
				}

				poly[i] += rotSides[i];
			}
		}
	}
}
