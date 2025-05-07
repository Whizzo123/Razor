#pragma once
#include "Razor.h"
#include <yaml-cpp/yaml.h>

namespace EdgeEditor
{
	class ModelSerializer
	{
	public:
		/**
		* Serialize a Model object and save to the provided path in YAML format
		* 
		* @param Path The path which we will save the YAML file to
		* @param Model The model object to be serialized
		*/
		static void Serialize(const std::string& Path, Razor::Ref<Razor::Model> Model);
		/**
		* Deserialize a Model object from the provided path
		*
		* @param Path The path which we will load the YAML file from
		* @param Model The model object to be created from the data in the file
		*/
		static void Deserialize(const std::string& Path, Razor::Ref<Razor::Model> OutModel);
	};
}

