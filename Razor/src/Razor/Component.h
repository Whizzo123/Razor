#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <bitset>
#include <unordered_map>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
	glm::vec3 Position;
	glm::vec3 Scale;
	
public:
	glm::vec3 GetRotation()
	{
		return Rotation;
	}
	void Rotate(glm::vec3 EulerAngles)
	{
		Rotation = EulerAngles;
		RotationQ = glm::quat(glm::radians(EulerAngles));
	}
	glm::mat4 Get()
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		model = model * glm::toMat4(RotationQ);
		return model;
	}

private:
	glm::vec3 Rotation;
	glm::quat RotationQ;

	void Move(glm::vec3 MoveVector)
	{
		Position += MoveVector;
	}

};


	struct Mesh
	{
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
		};
		struct Texture
		{
			unsigned int Id;
			std::string Type;
			std::string Path;
		};

		Mesh()
		{

		}

		Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures)
		{
			this->Vertices = Vertices;
			this->Indices = Indices;
			this->Textures = Textures;
		}

		std::vector<Vertex> Vertices;
		std::vector<Mesh> Meshes;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;
		unsigned int VAO, VBO, EBO;
		bool bHasIndices = true;
	};



