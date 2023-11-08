#pragma once
#include <string>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>

namespace Razor
{

	class Shader
	{
	public:
		Shader(){};
		virtual ~Shader() {};

		virtual void Setup(glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 lightColor) {};

		unsigned int ID;

		unsigned int GetUniformLocation(std::string Name);

		void SetInt(std::string Name, int Value);
		void SetMat4Float(std::string Name, glm::mat4 Value);

		void SetVec3(std::string Name, glm::vec3 Value);

		void SetFloat(std::string Name, float Value);

		void SetBool(std::string Name, bool Value);


	private:
		std::vector<unsigned int> Textures;
	};

}