#include "Model.h"
#include "../Texture/TextureLoader.h"

Model::Model()
{
}

Model::Model(std::string const& path, bool gamma) :gammaCorrection(gamma)
{
	loadModel(path);
}

void Model::draw(Shader& shader)
{
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].draw(shader);
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
		return;
	}
	directory = path.substr(0, path.find_last_of("/"));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//Vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 tempVector;
		tempVector.x = mesh->mVertices[i].x;
		tempVector.y = mesh->mVertices[i].z;
		tempVector.z = mesh->mVertices[i].y;
		vertex.position = tempVector;
		if (mesh->HasNormals())
		{
			tempVector.x = mesh->mNormals[i].x;
			tempVector.y = mesh->mNormals[i].z;
			tempVector.z = mesh->mNormals[i].y;
			vertex.normal = tempVector;
		}
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 tempVec;
			tempVec.x = mesh->mTextureCoords[0][i].x;
			tempVec.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = tempVec;
		}
		else
			vertex.texcoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	float bbox[6];
	bbox[0] = mesh->mAABB.mMin.x;
	bbox[1] = mesh->mAABB.mMin.y;
	bbox[2] = mesh->mAABB.mMin.z;
	bbox[3] = mesh->mAABB.mMax.x;
	bbox[4] = mesh->mAABB.mMax.y;
	bbox[5] = mesh->mAABB.mMax.z;

	return Mesh(vertices, indices, textures, bbox);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (int j = 0; j < texturesLoaded.size(); j++)
		{
			if (std::strcmp(texturesLoaded[i].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.ID = TextureLoader::textureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}