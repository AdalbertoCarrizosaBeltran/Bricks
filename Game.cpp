#include "stdafx.h"
#include "Game.h"


Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();
	if (!bricks.empty())
	{
		while (!bricks.empty())
		{
			bricks.pop_back();
		}
	}


	// TODO #2 - Add this brick and 4 more bricks to the vector
	for (int i = 0; i < 5; i++)
	{
		Box _boxi;
		_boxi.width = 10;
		_boxi.height = 2;
		_boxi.x_position = i * Console::WindowWidth()/5;
		_boxi.y_position = 5;
		_boxi.doubleThick = true;
		_boxi.color = ConsoleColor::DarkGreen;
		bricks.push_back(_boxi);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering should be done between the locks in this function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < bricks.size(); i++)
	{
		bricks[i].Draw();
	}

	Console::Lock(false);
	if (bricks.size() == 0)
	{
		Console::SetCursorPosition(20, 20);
		std::cout << "Wow how pretty you won!! Press 'R' to play again...";
	}
	if (ball.y_position == Console::WindowWidth())
	{
		Console::SetCursorPosition(20, 20);
		std::cout << "Wow imagine losing you loser!! Press 'R' to play again...";
	}

}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < bricks.size(); i++)
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			bricks[i].color = ConsoleColor(bricks[i].color - 1);
			ball.y_velocity *= -1;
			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricks[i].color == Black)
			{
				bricks.erase(bricks.begin() + i);
			}
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display victory text with R to reset
	if (bricks.size() == 0)
	{
		ball.moving = false;
		Console::SetCursorPosition(20, 20);
		std::cout << "Wow how pretty you won!! Press 'R' to play again...";
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display defeat text with R to reset
	if (ball.y_position == Console::WindowWidth())
	{
		ball.moving = false;
		Console::SetCursorPosition(20, 20);
		std::cout << "Wow imagine losing you loser!! Press 'R' to play again...";
	}
}
