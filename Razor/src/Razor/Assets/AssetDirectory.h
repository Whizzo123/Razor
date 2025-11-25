#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../Renderer/Model.h"

namespace Razor
{
	struct AssetKey
	{
		AssetKey() : key(std::string()) {}
		AssetKey(const std::string& assetPath) : key(assetPath)
		{
			
		}

		bool operator==(const AssetKey& other) const
		{
			return key == other.key;
		}
		
		std::string GetKey() const
		{
			return key;
		}

	private:
		std::string key;

		friend class AssetDirectory;
	};

	struct AssetKeyHash
	{
		size_t operator()(const AssetKey& key) const noexcept
		{
			return std::hash<std::string>()(key.GetKey());
		}
	};

	template<typename T>
	struct AssetWrapper
	{
		T asset;
	};

	class AssetDirectory
	{
	public:
		AssetDirectory(const std::string& assetFolder);
		~AssetDirectory();

		template<typename T> 
		AssetWrapper<T>* ProcessRequest(AssetKey requestkey)
		{
			if constexpr (std::is_same_v<T, Model>)
			{
				if (_mModelCache.find(requestkey) == _mModelCache.end())
				{
					LoadModel(requestkey.key);
				}
			}
			return &(_mModelCache[requestkey]);
		}

		std::vector<AssetKey> RequestKeys(const std::string& directoryPath);

	private:
		template<typename T>
		T* LoadAsset(AssetKey key)
		{
			return new T();
		}

		template<typename T>
		T* LoadCacheAsset(AssetKey key)
		{
			return new T();
		}

		std::vector<AssetKey> LoadCachedKeys(const std::string& directoryPath);

		std::vector<AssetKey> ProcessDirectory(const std::string& directoryPath);

		void LoadModel(const std::string& assetPath);

	private:
		std::string _mRootFolder;

		std::unordered_map<AssetKey, AssetWrapper<Model>, AssetKeyHash> _mModelCache;
	};
}

