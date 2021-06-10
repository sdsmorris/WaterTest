#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include <iostream>

class TextureLoader
{
public:
	static unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);
	static unsigned int textureFromFileToTarget(const char* path, GLenum target);
};