#pragma once
#include <vector>
#include "Box.h"
#include "Ball.h"

class Game
{
	Ball ball;
	Box paddle;


	// storing the bricks by value so the vector manages their lifetime
	std::vector<Box> bricks;

	//tracking whether the ball has reached the bottom of the window
	bool playerLost = false;

	// tracking whether the player has destroyed every brick
	bool playerWon = false;

public:
	Game();
	bool Update();
	void Render() const;
	void Reset();
	void ResetBall();
	void CheckCollision();
};