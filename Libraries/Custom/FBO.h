#pragma once
#include <glad/glad.h>
#include <iostream>

class FBO {
public:
	GLuint ID;
	GLuint textureID;
	GLuint rbo;

	FBO(int width, int height);
	void Bind();
	void Unbind();
	void Delete();
};
