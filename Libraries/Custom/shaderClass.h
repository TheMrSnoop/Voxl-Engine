#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<glm/gtc/type_ptr.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;

	std::string VertexShaderName;
	std::string FragmentShaderName;

	std::string ShaderName;

	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* shaderFolder);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();


	void setMat4(const std::string& name, const glm::mat4& mat);
	void setInt(const std::string& name, int value);
	void setVec2(const std::string& name, float x, float y);
	void setVec3(const std::string& name, float x, float y, float z);
	void setFloat(const std::string& name, float value);


private:
	void compileErrors(unsigned int shader, const std::string& type);
};
#endif