#include "ModelSerializer.h"
#include <fstream>

namespace EdgeEditor
{

	void ModelSerializer::Serialize(const std::string& Path, Razor::Ref<Razor::Model> Model)
	{
		Razor::YamlEmitter* Out = Razor::yaml_emitter_new();
		Razor::yaml_emitter_begin_map(Out);
		Razor::yaml_emitter_key(Out, "ModelInfo");
		Razor::yaml_emitter_value_modelinfo(Out, Model->GetModelInfo());
		Razor::yaml_emitter_end_map(Out);

		const std::string PathPlusExt = Path + ".model";

		std::ofstream FOut(PathPlusExt.c_str());
		const char* ErrorMsg = new char(' ');
		std::perror(ErrorMsg);
		RZ_WARN("Error Msg: {0}", ErrorMsg);
		FOut << Razor::yaml_emitter_cstr(Out);
		FOut.close();
	}

	void ModelSerializer::Deserialize(const std::string& Path, Razor::Ref<Razor::Model> OutModel)
	{
		Razor::YamlNode* Data;
		const std::string PathPlusExt = Path + ".model";
		try
		{
			Data = Razor::yaml_load_file(PathPlusExt.c_str());
		}
		catch (std::exception e)
		{
			RZ_ERROR("Failed to load .model file '{0}'\n	{1}", PathPlusExt, e.what());
			return;
		}

		if (!Razor::yaml_get_child(Data, "ModelInfo"))
		{
			RZ_ERROR("Incomplete .model file missing 'ModelInfo' key");
			return;
		}

		Razor::ModelInfo Info = Razor::yaml_as_modelinfo(Razor::yaml_get_child(Data, "ModelInfo"));
		OutModel->SetModelInfo(Info);
	}
}