#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <bitset>
#include <unordered_map>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Core.h"
#include <memory>

namespace Razor
{

	struct RAZOR_API Transform
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

	struct MeshData
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

		MeshData()
		{

		}

		MeshData(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures, unsigned int MaterialID)
		{
			this->Vertices = Vertices;
			this->Indices = Indices;
			this->Textures = Textures;
			MaterialId = MaterialID;
		}

		std::vector<Vertex> Vertices;
		std::vector<MeshData> Meshes;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;
		unsigned int VAO, VBO, EBO;
		unsigned int MaterialId;
		bool bHasIndices = true;
	};

	struct Light
	{
		glm::vec3 Position;
		glm::vec3 Diffuse;
		glm::vec3 Ambient;
		glm::vec3 Specular;
		Light() {}
		virtual ~Light() {}
	};

	struct DirectionalLight
	{
		glm::vec3 Position;
		glm::vec3 Diffuse;
		glm::vec3 Ambient;
		glm::vec3 Specular;
		glm::vec3 Direction;
	};

	struct PointLight : public Light
	{
		float Constant;
		float Linear;
		float Quadratic;
	};

	struct SpotLight : public DirectionalLight
	{
		float CutOff;
	};


	struct RAZOR_API Mesh
	{
		std::vector<MeshData> Data;
	};

	struct MaterialData
	{
		std::string MaterialName;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;
		MaterialData(glm::vec3 A, glm::vec3 D, glm::vec3 S, float SH) : Ambient(A), Diffuse(D), Specular(S), Shininess(SH) {}
	};

	struct Material
	{
		std::vector<MaterialData> Materials;
		uint8_t ShaderID;
	};

	struct Collider
	{
		float Radius;
	};

	
}
