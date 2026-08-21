#include <fstream>
#include "ModelSerializer.h"
#include "../Renderer/Model.h"
#include "../IO/YamlWrapper.h"
#include "../Log.h"

namespace Razor
{

	void ModelSerializer::Serialize(const std::string& Path, Ref<Model> Model)
	{
		YamlEmitter* Out = Razor::yaml_emitter_new();
		yaml_emitter_begin_map(Out);
		yaml_emitter_key(Out, "ModelInfo");
		yaml_emitter_value_modelinfo(Out, Model->GetModelInfo());
		yaml_emitter_end_map(Out);

		const std::string PathPlusExt = Path + ".model";

		std::ofstream FOut(PathPlusExt.c_str());
		const char* ErrorMsg = new char(' ');
		std::perror(ErrorMsg);
		RZ_CORE_WARN("Error Msg: {0}", ErrorMsg);
		FOut << Razor::yaml_emitter_cstr(Out);
		FOut.close();
	}

	void ModelSerializer::Deserialize(const std::string& Path, Razor::Ref<Razor::Model> OutModel)
	{
		Razor::YamlNode* Data;
		const std::string PathPlusExt = Path;
		try
		{
			Data = yaml_load_file(PathPlusExt.c_str());
		}
		catch (std::exception e)
		{
			RZ_CORE_ERROR("Failed to load .model file '{0}'\n	{1}", PathPlusExt, e.what());
			return;
		}

		if (!yaml_get_child(Data, "ModelInfo"))
		{
			RZ_CORE_ERROR("Incomplete .model file missing 'ModelInfo' key");
			return;
		}

		Razor::ModelInfo Info = yaml_as_modelinfo(yaml_get_child(Data, "ModelInfo"));
		OutModel->SetModelInfo(Info);
	}
}