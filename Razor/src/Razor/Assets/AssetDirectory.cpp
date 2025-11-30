#include "AssetDirectory.h"
#include "../MeshRenderer.h"
#include "ModelSerializer.h"

namespace Razor
{
	AssetDirectory::AssetDirectory(const std::string& assetFolder)
	{
		_mRootFolder = assetFolder;
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
		ModelSerializer::Deserialize(assetPath, model);
		MeshRenderer::InitMesh(model->GetModelMeshData());
		_mModelCache[assetPath] = AssetWrapper<Model>({ *model });
	}
}