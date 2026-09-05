#pragma once
#include "BaseObject.h"

class Box : public BaseObject
{
public:
	bool doubleThick = false;
	int width = 2;
	int height = 2;
	int hitCount = 0; // tracking collision independantly from the brick's display color
	void Draw() const override;
	bool Contains(int x, int y);
};