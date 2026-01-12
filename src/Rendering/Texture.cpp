#include"Texture.h"
std::vector<Image::imageData> Image::imageDatabase;

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, bool flipVertically)
{
	//Plugs in all the properties

	type = texType;

	int widthImg, heightImg, numColChr;
	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColChr, 0);


	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//Okay so, GL_RGB for .jpgs because they dont have an alpha channel, GL_RGBA for .pngs 
	glTexImage2D(texType, 0, GL_RGB, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);

}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}


int Image::ReturnImage(const char* imagePath)
{
	if (Image::imageDatabase.size() > 0)
	{
		for (int i = 0; i < Image::imageDatabase.size() - 1; i++)
		{
			Image::imageData data = Image::imageDatabase[i];

			if (data.imagePath == imagePath)
				return i;
		}
	}

	//failed to find.
	return -1;
}


GLuint Image::GenerateImage(const char* imagePath)
{
	if (Image::ReturnImage(imagePath) == -1)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);

		unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 4);
		if (!imageData) {
			std::cerr << "Failed to load image: " << imagePath << std::endl;
			return 0;
		}

		GLuint imageObject;
		glGenTextures(1, &imageObject);
		glBindTexture(GL_TEXTURE_2D, imageObject);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		glBindTexture(GL_TEXTURE_2D, 0); // unbind
		stbi_image_free(imageData);

		Image::imageDatabase.push_back({ imageObject, imagePath });

		//Returns the index of this data inside the database.
		return Image::imageDatabase[Image::imageDatabase.size() - 1].GL_DATA;
	}
	else
	{
		return Image::imageDatabase[Image::ReturnImage(imagePath)].GL_DATA;
	}
}
