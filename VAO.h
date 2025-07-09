#pragma once
#include <glad/glad.h>

class VAO
{
public:
	GLuint ID;

	VAO();
	void Bind();
	void Unbind();
	void Delete();

	void LinkAttrib(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
};
