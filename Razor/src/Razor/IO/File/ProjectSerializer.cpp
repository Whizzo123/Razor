#include "ProjectSerializer.h"
#include "../../Scene/Project.h"
#include "../../Log.h"
#include "../YamlWrapper.h"
#include <fstream>
#include <filesystem>


namespace Razor
{

	void ProjectSerializer::Serialize(const std::string& Path, Razor::Ref<Project> Project)
	{
		if (Project == nullptr)
		{
			RZ_CORE_ERROR("ProjectSerializer::Serialize Error: Project Ptr passed in was null");
			return;
		}

		if (Project->m_ProjectName == "")
		{
			RZ_CORE_ERROR("ProjectSerializer::Serialize Error: Project Name cannot be an empty string");
			return;
		}

		std::string AssetFolderPath = Project->m_AssetDirectory;
		std::string DllFolderPath = Project->m_DllDirectory;
		if (Project->m_AssetDirectory.empty())
		{
			AssetFolderPath = Path + "/" + "assets";
			std::filesystem::create_directory(AssetFolderPath);
		}
		if (Project->m_DllDirectory.empty())
		{
			DllFolderPath = Path + "/" + "assembly";
			std::filesystem::create_directory(DllFolderPath);
		}

		Project->m_AssetDirectory = AssetFolderPath;
		Project->m_DllDirectory = DllFolderPath;

		YamlEmitter* Out =  yaml_emitter_new();
		yaml_emitter_begin_map(Out);
		yaml_emitter_key(Out, "ProjectName");
		yaml_emitter_value_string(Out, Project->m_ProjectName.c_str());
		yaml_emitter_key(Out, "AssetDirectory");
		yaml_emitter_value_string(Out, Project->m_AssetDirectory.c_str());
		yaml_emitter_key(Out, "DllDirectory");
		yaml_emitter_value_string(Out, Project->m_DllDirectory.c_str());
		yaml_emitter_key(Out, "MainScenePath");
		yaml_emitter_value_string(Out, Project->m_MainScenePath.c_str());
		yaml_emitter_end_map(Out);


		std::filesystem::create_directory(Path);

		const std::string PathPlusExt = Path + "/" + Project->m_ProjectName + ".proj";

		std::ofstream FOut(PathPlusExt.c_str());
		const char* ErrorMsg = new char(' ');
		std::perror(ErrorMsg);
		RZ_CORE_WARN("Error Msg: {0}", ErrorMsg);
		FOut << Razor::yaml_emitter_cstr(Out);
		FOut.close();
	}

	void ProjectSerializer::Deserialize(const std::string& Path, Razor::Ref<Project> OutProject)
	{

		Razor::YamlNode* Data;
		const std::string PathPlusExt = Path + ".proj";
		try
		{	
			Data = Razor::yaml_load_file(PathPlusExt.c_str());
		}
		catch (std::exception e)
		{
			RZ_CORE_ERROR("Failed to load .proj file '{0}'\n	{1}", PathPlusExt, e.what());
			return;
		}

		if (!Razor::yaml_get_child(Data, "ProjectName"))
		{
			RZ_CORE_ERROR("Incomplete .proj file missing 'ProjectName' key");
			return;
		}

		OutProject->m_ProjectName = Razor::yaml_as_string(Razor::yaml_get_child(Data, "ProjectName"));
		OutProject->m_AssetDirectory = Razor::yaml_as_string(Razor::yaml_get_child(Data, "AssetDirectory"));
		OutProject->m_DllDirectory = Razor::yaml_as_string(Razor::yaml_get_child(Data, "DllDirectory"));
		OutProject->m_MainScenePath = Razor::yaml_as_string(Razor::yaml_get_child(Data, "MainScenePath"));
		OutProject->m_ProjectPath = Path.substr(0, Path.find_last_of("/\\"));
	}

}
