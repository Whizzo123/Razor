#pragma once
#include <string>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
class Shader
{
public:
	Shader() : ID(-1) {};
	~Shader() {};

	unsigned int ID;

	unsigned int GetUniformLocation(std::string Name);

	void AddTextures(std::vector<unsigned int>& InTextures);

	void SetTextures();

	void SetInt(std::string Name, int Value);
	void SetMat4Float(std::string Name, glm::mat4 Value);


private:
	std::vector<unsigned int> Textures;
};

