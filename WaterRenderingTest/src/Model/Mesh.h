#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OpenGL/Shader.h"
#include <string>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

struct Texture {
	unsigned int ID;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	glm::vec3 bboxMin, bboxMax;
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, float* bbox);
	void draw(Shader& shader);
private:
	void setupMesh();
};

