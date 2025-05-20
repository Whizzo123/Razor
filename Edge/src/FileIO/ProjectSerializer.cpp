#include "ProjectSerializer.h"
#include "../Project.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>


namespace EdgeEditor
{

	void ProjectSerializer::Serialize(const std::string& Path, Razor::Ref<Project> Project)
	{
		if (Project == nullptr)
		{
			RZ_ERROR("ProjectSerializer::Serialize Error: Project Ptr passed in was null");
			return;
		}

		if (Project->ProjectName == "")
		{
			RZ_ERROR("ProjectSerializer::Serialize Error: Project Name cannot be an empty string");
			return;
		}

		const std::string ProjectFolderPath = Path + "/" + Project->ProjectName;
		const std::string AssetFolderPath = ProjectFolderPath + "/" + "assets";
		const std::string DllFolderPath = ProjectFolderPath + "/" + "assembly";

		Project->AssetDirectory = AssetFolderPath;
		Project->DllDirectory = DllFolderPath;

		YAML::Emitter Out;
		Out << YAML::BeginMap;
		Out << YAML::Key << "ProjectName" << YAML::Value << Project->ProjectName;
		Out << YAML::Key << "AssetDirectory" << YAML::Value << Project->AssetDirectory;
		Out << YAML::Key << "DllDirectory" << YAML::Value << Project->DllDirectory;
		Out << YAML::Key << "MainScenePath" << YAML::Value << Project->MainScenePath;
		Out << YAML::EndMap;

		std::filesystem::create_directory(ProjectFolderPath);
		std::filesystem::create_directory(AssetFolderPath);
		std::filesystem::create_directory(DllFolderPath);

		const std::string PathPlusExt = ProjectFolderPath + "/" + Project->ProjectName + ".proj";

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
