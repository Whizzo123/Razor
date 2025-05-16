#include "ProjectSerializer.h"
#include "../Project.h"
#include <yaml-cpp/yaml.h>
#include <fstream>


namespace EdgeEditor
{

	void ProjectSerializer::Serialize(const std::string& Path, Razor::Ref<Project> Project)
	{

		YAML::Emitter Out;
		Out << YAML::BeginMap;
		Out << YAML::Key << "ProjectName" << YAML::Value << Project->ProjectName;
		Out << YAML::Key << "AssetDirectory" << YAML::Value << Project->AssetDirectory;
		Out << YAML::Key << "DllDirectory" << YAML::Value << Project->DllDirectory;
		Out << YAML::Key << "MainScenePath" << YAML::Value << Project->MainScenePath;
		Out << YAML::EndMap;

		const std::string PathPlusExt = Path + ".proj";

		std::ofstream FOut(PathPlusExt.c_str());
		const char* ErrorMsg = new char(' ');
		std::perror(ErrorMsg);
		RZ_WARN("Error Msg: {0}", ErrorMsg);
		FOut << Out.c_str();
		FOut.close();
	}

	void ProjectSerializer::Deserialize(const std::string& Path, Razor::Ref<Project> OutProject)
	{

		YAML::Node Data;
		const std::string PathPlusExt = Path + ".proj";
		try
		{
			Data = YAML::LoadFile(PathPlusExt);
		}
		catch (YAML::ParserException e)
		{
			RZ_ERROR("Failed to load .proj file '{0}'\n	{1}", PathPlusExt, e.what());
			return;
		}

		if (!Data["ProjectName"])
		{
			RZ_ERROR("Incomplete .proj file missing 'ProjectName' key");
			return;
		}
		
		OutProject->ProjectName = Data["ProjectName"].as<std::string>();
		OutProject->AssetDirectory = Data["AssetDirectory"].as<std::string>();
		OutProject->DllDirectory = Data["DllDirectory"].as<std::string>();
		OutProject->MainScenePath = Data["MainScenePath"].as<std::string>();
	}

}
