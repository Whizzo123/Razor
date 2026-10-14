#include "AssetDirectory.h"
#include "../Engine.h"
#include "../Renderer/Shaders/DefaultMeshShader.h"
#include "../Renderer/Font/FontLoader.h"
#include "FilePath.h"
#include "../Renderer/IRenderer.h"

namespace Razor
{
	AssetDirectory::AssetDirectory(const std::string& assetFolder, Ref<IRenderer> renderer)
	{
		_mRootFolder = assetFolder;
		_mRenderer = renderer;
	}
	AssetDirectory::~AssetDirectory()
	{
	}
	void AssetDirectory::LoadModel(const std::string& assetPath)
	{
		FilePath path = FilePath(_mRootFolder) + FilePath(assetPath);
		Ref<Model> model = CreateRef<Model>(path);
		_mRenderer->SetupMesh(model->GetModelMeshData());
		model->SetModelShader(Engine::Get().GetShaderForType(typeid(DefaultMeshShader).name())->ID);
		_mModelCache[assetPath] = AssetWrapper<Model>({ *model });
	}

	void AssetDirectory::LoadFont(const std::string& assetPath)
	{
		FilePath path = FilePath(_mRootFolder) + FilePath(assetPath);
		std::string pathStr = static_cast<std::string>(path);
		Font font(Engine::Get().GetFontLoader().LoadFontMap(pathStr));
		_mFontCache[assetPath] = AssetWrapper<Font>({font});
	}

	bool AssetDirectory::SaveModelToProject(const Model& model)
	{
		model.Serialize(_mRootFolder);
		// Probably should have some kind of error handling here what happens if the serialization fails
		return true;
	}
}