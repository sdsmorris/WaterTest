#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	int ID;
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setMat4(const std::string& name, const glm::mat4& mat);
	void setVec3(const std::string& name, const glm::vec3& vec);
	void setVec4(const std::string& name, const glm::vec4& vec);
	void setInt(const std::string& name, const int num);
	void setFloat(const std::string& name, const float num);
private:
	void checkCompileErrors(GLuint shader, std::string type);
};

