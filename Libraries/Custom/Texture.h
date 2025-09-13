#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad//glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"
#include"imgui.h"

#include<vector>

class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};



class Image
{
public:

	//apparently I need to drop the static keyword.
	struct imageData
	{
		GLuint GL_DATA;
		const char* imagePath;
	};

	static std::vector<imageData> imageDatabase;

	static int ReturnImage(const char* imagePath);
	static GLuint GenerateImage(const char* imagePath);
};

#endif