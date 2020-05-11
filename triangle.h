#pragma once
#include "drawable.h"
class Triangle : public Drawable {
public:
	void buffer();
	virtual void draw();
private:
	GLuint vertexbuffer;
};