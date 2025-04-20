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
#include "IO/RazorIO.h"

namespace Razor
{
	struct ISerializable
	{
	public:
		virtual std::unordered_map<std::string, std::string> Serialize() = 0;
	};


	struct Transform
	{
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::vec3 Rotation;

		Transform() : Position(0.0f, 0.0f, 0.0f), Scale(0.0f, 0.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), RotationQ(0.0f, 0.0f, 0.0f, 1.0f) {}
		Transform(glm::vec3 Pos, glm::vec3 Scl, glm::vec3 Rot) : Position(Pos), Scale(Scl), Rotation(Rot), RotationQ(0.0f, 0.0f, 0.0f, 1.0f) {}

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
		float Cutoff;
		float OuterCutoff;
	};


	struct   Mesh
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

	struct Camera
	{
		// TODO Potentially move this and pass responsiblity onto the developer
		Camera()
		{
			CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
			CameraDirection = glm::normalize(CameraPos - CameraTarget);

			Up = glm::vec3(0.0f, 1.0f, 0.0f);
			CameraRight = glm::normalize(glm::cross(Up, CameraDirection));
			CameraUp = glm::cross(CameraDirection, CameraRight);
			CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		}
		glm::vec3 CameraPos;
		glm::vec3 CameraTarget;
		glm::vec3 CameraDirection;

		glm::vec3 Up;
		glm::vec3 CameraRight;
		glm::vec3 CameraUp;
		glm::vec3 CameraFront;
	};

	struct CameraInfo
	{
		glm::mat4 CameraView;
		glm::mat4 CameraProjection;
		glm::vec3 CameraPosition;
	};

	struct Input
	{
		// KeyMap [Key] -> State (0, 1)
		RazorKey Keyboard;
		// MouseBut [Key] -> State (0, 1)
		// MousePos -> Pos
	};
}
