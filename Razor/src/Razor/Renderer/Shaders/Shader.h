#pragma once

#include "../IRenderer.h"

#include <vector>
#include <memory>
#include <unordered_map>

namespace Razor
{

	template<typename T>
	struct ShaderParameter
	{
		ShaderParameter(std::string Name, T Value) : UniformName(Name), UniformValue(Value) {}
		std::string UniformName;
		T UniformValue;
	};

	class IShaderParameterArray
	{

	};

	template<typename T>
	class ShaderParameterArray : public IShaderParameterArray
	{
	public:

		void AddParameter(ShaderParameter<T> Parameter)
		{
			ParameterArray.push_back(Parameter);
		}

		std::vector<ShaderParameter<T>> GetParameters()
		{
			return ParameterArray;
		}
	private:
		std::vector<ShaderParameter<T>> ParameterArray;
	};

	class Shader
	{
	public:
		Shader(){};
		virtual ~Shader() {};

		virtual void Setup(CameraInfo* CamInfo, glm::mat4 model, MaterialData Mat, std::shared_ptr<std::vector<Light*>> Light) = 0;

		unsigned int ID;

		unsigned int GetUniformLocation(std::string Name);

		void SetInt(std::string Name, int Value);
		void SetMat4Float(std::string Name, glm::mat4 Value);

		void SetVec3(std::string Name, glm::vec3 Value);

		void SetFloat(std::string Name, float Value);

		void SetBool(std::string Name, bool Value);

	public:
		/*template<typename T>
		void AddParameter(ShaderParameter<T> Parameter)
		{
			const char* TypeName = typeid(T).name();
			std::shared_ptr <ShaderParameter<T>> ParameterArray;
			if (ShaderParameterTypes.find(TypeName) == ShaderParameterTypes.end())
			{
				ParameterArray = GetParameterArray<T>();
			}
			else
			{
				ParameterArray = CreateNewParameterArray<T>();
			}
			ParameterArray->AddParameter<T>(Parameter);
		}*/
		
		uint8_t NextShaderParameterType = 0;

	private:
		std::unordered_map < const char*, std::shared_ptr<IShaderParameterArray>> ShaderParameterArrays{};
		std::unordered_map<const char*, uint8_t> ShaderParameterTypes;
	};

}