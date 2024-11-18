#pragma once
#include <string>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include "../../Component.h"
#include "../IRenderer.h"

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
		template<typename T>
		void AddParameter(ShaderParameter<T> Parameter)
		{
			ParameterArray.push_back(Parameter);
		}
		template<typename T>
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

		virtual void Setup(CameraInfo* CamInfo, glm::mat4 model, MaterialData Mat, std::shared_ptr<std::vector<Light*>> Light) {};

		unsigned int ID;

		unsigned int GetUniformLocation(std::string Name);

		void SetInt(std::string Name, int Value);
		void SetMat4Float(std::string Name, glm::mat4 Value);

		void SetVec3(std::string Name, glm::vec3 Value);

		void SetFloat(std::string Name, float Value);

		void SetBool(std::string Name, bool Value);

	private:
		/*template<typename T>
		std::shared_ptr<ShaderParameterArray<T>> GetParameterArray()
		{
			const char* TypeName = typeid(T).name();

			std:shared_ptr<IShaderParameterArray> IArray = ShaderParameterArrays[TypeName];
			if (!IArray)
			{
				RZ_CORE_ERROR("Attempting to grab shader parameter array of: " + TypeName + " but that array does not exist");
				return nullptr;
			}
			std::shared_ptr<ShaderParameterArray<T>> ParameterArray = std::dynamic_pointer_cast<ShaderParameterArray<T>>(IArray);
			return ParameterArray;
		}
		template<typename T>
		std::shared_ptr<ShaderParameterArray<T>> CreateNewParameterArray()
		{
			const char* TypeName = typeid(T).name();
			ShaderParameterTypes.insert({ TypeName, NextShaderParameterType });
			ShaderParametersArrays.insert({ TypeName, std::make_shared<ShaderParameterArray<T>>() });
			++NextShaderParameterType;
		}*/

	public:
		template<typename T>
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
		}
		
		uint8_t NextShaderParameterType = 0;

	private:
		std::unordered_map < const char*, std::shared_ptr<IShaderParameterArray>> ShaderParameterArrays{};
		std::unordered_map<const char*, uint8_t> ShaderParameterTypes;
	};

}