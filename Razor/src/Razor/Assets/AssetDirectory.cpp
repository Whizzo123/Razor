#include "AssetDirectory.h"
#include "../MeshRenderer.h"

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
		Model model = Model();
		model.LoadMesh(assetPath);
		MeshRenderer::InitMesh(model.GetModelMeshData());
		_mModelCache[AssetKey(assetPath)] = AssetWrapper<Model>({ model });
	}
}