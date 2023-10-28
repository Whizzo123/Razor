#pragma once
#include <string>
#include <vector>
#include "../Component.h"

class Material
{
public:
	Material(std::string VertexShader, std::string FragmentShader, std::vector<std::string>& TextureNames);
	~Material();

	virtual void PassValues(Transform ObjTransform) {};
	void BindMaterial();

protected:
	unsigned int ID;
	std::vector<unsigned int> Textures;
};

