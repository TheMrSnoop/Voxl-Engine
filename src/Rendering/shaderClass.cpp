#include "shaderClass.h"
#include <filesystem>
#include <vector>
#include "VoxlEngine.h"

namespace fs = std::filesystem;

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

std::string findShader(std::string folderName, std::string shaderExtension)
{
	for (const auto& entry : fs::directory_iterator(folderName)) 
	{
		if (entry.is_regular_file() && entry.path().extension() == shaderExtension) {
			//entry.path is a "fs::path" type, so this converts it to a string first, then a const char.

			return entry.path().string();
		}
	}

	throw std::runtime_error("a " + shaderExtension + " was not found in " + folderName);
}


std::string shaderDisplayName(std::string shaderFolder)
{
	std::string str = shaderFolder;

	int index = str.find("/");


	if (index != std::string::npos)
	{
		str.erase(0, index + 1);
	}

	return str;
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* shaderFolder)
{

	//Important for setting shader varriables in main.cpp, as it references this varriable.
	//(Replaces VertexShaderName)

	ShaderName = shaderDisplayName(shaderFolder);

	// Read vertexFile and fragmentFile and store the strings
	std::string vertexPath = findShader(shaderFolder, ".vert");
	std::cout << "VERT = " << vertexPath << std::endl;

	std::string fragmentPath = findShader(shaderFolder, ".frag");
	std::cout << "FRAG = " << fragmentPath << std::endl;

	std::string vertexCode = get_file_contents(vertexPath.c_str());
	std::string fragmentCode = get_file_contents(fragmentPath.c_str());

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	compileErrors(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	compileErrors(fragmentShader, "FRAGMENT");

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const std::string& type)
{
    GLint success;
    char infoLog[1024];

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "SHADER COMPILATION ERROR (" << type << ")\n"
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "PROGRAM LINK ERROR\n"
                      << infoLog << std::endl;
        }
    }
}


void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setVec2(const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
