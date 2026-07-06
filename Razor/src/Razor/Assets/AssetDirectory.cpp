#include "AssetDirectory.h"
#include "ModelSerializer.h"
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
	std::vector<AssetKey> AssetDirectory::RequestKeys(const std::string& directoryPath)
	{
		return std::vector<AssetKey>();
	}
	std::vector<AssetKey> AssetDirectory::LoadCachedKeys(const std::string& directoryPath)
	{
		return std::vector<AssetKey>();
	}
	std::vector<AssetKey> AssetDirectory::ProcessDirectory(const std::string& directoryPath)
	{
		return std::vector<AssetKey>();
	}
	void AssetDirectory::LoadModel(const std::string& assetPath)
	{
		Ref<Model> model = CreateRef<Model>();
		FilePath path = FilePath(_mRootFolder) + FilePath(assetPath);
		ModelSerializer::Deserialize(path, model);
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
}