#pragma once

#include "../../Core.h"

namespace Razor
{
	struct Project;

	class ProjectSerializer
	{
	public:
		/**
		* Serialize a Project object and save to the provided path in YAML format
		*
		* @param Path The path which we will save the YAML file to
		* @param Project The project object to be serialized
		*/
		static void Serialize(const std::string& Path, Razor::Ref<Project> Project);
		/**
		* Deserialize a Project object from the provided path
		*
		* @param Path The path which we will load the YAML file from
		* @param OutProject The project object to be created from the data in the file
		*/
		static void Deserialize(const std::string& Path, Razor::Ref<Project> OutProject);
	};
}

