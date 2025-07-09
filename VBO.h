#pragma once
#include <glad/glad.h>

class VBO
{
public:
	GLuint ID;

	VBO(const void* data, GLsizeiptr size);  // generic constructor
	void Bind();
	void Unbind();
	void Delete();
};
