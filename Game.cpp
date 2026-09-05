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

	// beginning each game without an active win condition
	playerWon = false;
	playerLost = false;

	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();


	// removing any bricks from the previous game before rebuilding the row
	bricks.clear();

	// creating five bricks evenly spaced across one row
	for (int i = 0; i < 5; i++)
	{
		Box newBrick;
		newBrick.width = 10;
		newBrick.height = 2;
		newBrick.x_position = 5 + (i * 15); // leave five spaces between each 10-character-wide brick
		newBrick.y_position = 5;
		newBrick.doubleThick = true;
		newBrick.color = ConsoleColor::DarkGreen;

		bricks.push_back(newBrick); // copies the completed 'Box' into the vector
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

	if ((GetAsyncKeyState(VK_SPACE) & 0x1) && !playerWon && !playerLost)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();


	// drawing every brick currently stored in the vector
	for (const Box& brick : bricks)
	{
		brick.Draw();
	}

	if (playerWon)
	{
		const char* message = "You win! Press 'R' to play again.";
		const int messageWidth = 33;
		const int messageX = (Console::WindowWidth() - messageWidth) / 2;
		const int messageY = Console::WindowHeight() / 2;

		Console::ForegroundColor(ConsoleColor::Yellow);
		Console::WordWrap(messageX, messageY, messageWidth, message);
	}
	else if (playerLost)
	{
		const char* message = "You lose. Press 'R' to play again.";
		const int messageWidth = 34;
		const int messageX = (Console::WindowWidth() - messageWidth) / 2;
		const int messageY = Console::WindowHeight() / 2;

		Console::ForegroundColor(ConsoleColor::Red);
		Console::WordWrap(messageX, messageY, messageWidth, message);
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// checking the ball's next positions against every brick (updating current code logic)
	for (int i = 0; i < static_cast<int>(bricks.size()); i++)
	{

		Box& brick = bricks[i];

		if (brick.Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			brick.hitCount++;
			ball.y_velocity *= -1;

			//changing color to show that the brick has taken damage
			if (brick.hitCount == 1)
			{
				brick.color = ConsoleColor::DarkYellow;
			}
			else if (brick.hitCount == 2)
			{
				brick.color = ConsoleColor::DarkRed;
			}
			else
			{
				// erasing the brick when it receieves its third hit
				bricks.erase(bricks.begin() + i);
			}

			break; // only one brick should respond during this update

		}
	}

	// winning occurs after the final brick is removed
	if (bricks.empty())
	{
		ball.moving = false;
		playerWon = true;
	}


	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// the final valid row is the bottom of the window
	if (ball.y_position >= Console::WindowHeight() - 1)
	{
		ball.moving = false;
		playerLost = true;
	}
}
