#include "YamlWrapper.h"
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <sstream>
#include <exception>
#include "../Utils/Vector.h"
#include "../Renderer/Model.h"

namespace YAML
{
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Razor::Vector3>
    {
        static Node encode(const Razor::Vector3& rhs)
        {
            Node node;
            node.push_back(rhs.X);
            node.push_back(rhs.Y);
            node.push_back(rhs.Z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Razor::Vector3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.X = node[0].as<float>();
            rhs.Y = node[1].as<float>();
            rhs.Z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Razor::Vector2>
    {
        static Node encode(const Razor::Vector2& rhs)
        {
            Node node;
            node.push_back(rhs.X);
            node.push_back(rhs.Y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Razor::Vector2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.X = node[0].as<float>();
            rhs.Y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };



    template<>
    struct convert<Razor::MeshData::Vertex>
    {
        static Node encode(const Razor::MeshData::Vertex& rhs)
        {
            Node node;
            node.push_back(rhs.Position);
            node.push_back(rhs.Normal);
            node.push_back(rhs.TexCoords);
            node.push_back(rhs.Tangent);
            node.push_back(rhs.Bitangent);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Razor::MeshData::Vertex& rhs)
        {
            if (!node.IsSequence() || node.size() != 5)
                return false;

            rhs.Position = node[0].as<glm::vec3>();
            rhs.Normal = node[1].as<glm::vec3>();
            rhs.TexCoords = node[2].as<glm::vec2>();
            rhs.Tangent = node[3].as<glm::vec3>();
            rhs.Bitangent = node[4].as<glm::vec3>();
            return true;
        }
    };

    template<>
    struct convert<Razor::MeshData::Texture>
    {
        static Node encode(const Razor::MeshData::Texture& rhs)
        {
            Node node;
            node.push_back(rhs.Id);
            node.push_back(rhs.Type);
            node.push_back(rhs.Path);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Razor::MeshData::Texture& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.Id = node[0].as<unsigned int>();
            rhs.Type = node[1].as<std::string>();
            rhs.Path = node[2].as<std::string>();
            return true;
        }
    };

    template<>
    struct convert<Razor::MeshData>
    {
        static Node encode(const Razor::MeshData& rhs)
        {
            Node node;
            node.push_back(rhs.Vertices);
            node.push_back(rhs.Meshes);
            node.push_back(rhs.Indices);
            node.push_back(rhs.Textures);
            node.push_back(rhs.VAO);
            node.push_back(rhs.VBO);
            node.push_back(rhs.EBO);
            node.push_back(rhs.MaterialId);
            node.push_back(rhs.bHasIndices);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Razor::MeshData& rhs)
        {
            if (!node.IsSequence() || node.size() != 9)
                return false;

            rhs.Vertices = node[0].as<std::vector<Razor::MeshData::Vertex>>();
            rhs.Meshes = node[1].as<std::vector<Razor::MeshData>>();
            rhs.Indices = node[2].as<std::vector<unsigned int>>();
            rhs.Textures = node[3].as<std::vector<Razor::MeshData::Texture>>();
            rhs.VAO = node[4].as<unsigned int>();
            rhs.VBO = node[5].as<unsigned int>();
            rhs.EBO = node[6].as<unsigned int>();
            rhs.MaterialId = node[7].as<unsigned int>();
            rhs.bHasIndices = node[8].as<bool>();
            return true;
        }
    };

    template<>
    struct convert<Razor::MaterialData>
    {
        static Node encode(const Razor::MaterialData& rhs)
        {
            Node node;
            node.push_back(rhs.MaterialName);
            node.push_back(rhs.Ambient);
            node.push_back(rhs.Diffuse);
            node.push_back(rhs.Specular);
            node.push_back(rhs.Shininess);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Razor::MaterialData& rhs)
        {
            if (!node.IsSequence() || node.size() != 5)
                return false;

            rhs.MaterialName = node[0].as<std::string>();
            rhs.Ambient = node[1].as<glm::vec3>();
            rhs.Diffuse = node[2].as<glm::vec3>();
            rhs.Specular = node[3].as<glm::vec3>();
            rhs.Shininess = node[4].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Razor::Material>
    {
        static Node encode(const Razor::Material& rhs)
        {
            Node node;
            node.push_back(rhs.Materials);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Razor::Material& rhs)
        {
            if (!node.IsSequence() || node.size() != 1)
                return false;

            rhs.Materials = node[0].as<std::vector<Razor::MaterialData>>();
            return true;
        }
    };

    template<>
    struct convert<Razor::ModelInfo>
    {
        static Node encode(const Razor::ModelInfo& rhs)
        {
            Node node;
            node.push_back(rhs.ModelMeshData);
            node.push_back(rhs.ModelMaterial);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, Razor::ModelInfo& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.ModelMeshData = node[0].as<std::vector<Razor::MeshData>>();
            rhs.ModelMaterial = node[1].as<Razor::Material>();
            return true;
        }
    };
}

namespace Razor
{
    template<typename T>
    YAML::Emitter& operator<<(YAML::Emitter& Out, const T& rhs)
    {
        Out << YAML::convert<T>::encode(rhs);
        return Out;
    }

    // ===== Error Handling =====
    static thread_local std::string g_lastError;

    static void set_error(const std::string& err) {
        g_lastError = err;
    }

    const char* yaml_get_last_error() {
        return g_lastError.c_str();
    }

    // ===== Internal Impl Structures =====
    struct YamlNodeImpl {
        YAML::Node node;
        std::string cache; // used for temporary key/string returns
    };

    struct YamlEmitterImpl {
        YAML::Emitter out;
        std::string cache;
    };

    // ===== Node API =====
    YamlNode* yaml_load_file(const char* filename) {
        try {
            auto impl = new YamlNodeImpl();
            impl->node = YAML::LoadFile(filename);
            return reinterpret_cast<YamlNode*>(impl);
        }
        catch (const std::exception& e) {
            set_error(e.what());
            return nullptr;
        }
        catch (...) {
            set_error("Unknown error in yaml_load_file");
            return nullptr;
        }
    }

    YamlNode* yaml_load_string(const char* yamlText) {
        try {
            auto impl = new YamlNodeImpl();
            impl->node = YAML::Load(yamlText ? yamlText : "");
            return reinterpret_cast<YamlNode*>(impl);
        }
        catch (const std::exception& e) {
            set_error(e.what());
            return nullptr;
        }
        catch (...) {
            set_error("Unknown error in yaml_load_string");
            return nullptr;
        }
    }

    void yaml_free_node(YamlNode* node) {
        delete reinterpret_cast<YamlNodeImpl*>(node);
    }

	std::vector<YamlNode*> yaml_get_children(YamlNode* node, const char* key) 
    {
		std::vector<YamlNode*> outChildren;
        if (!node || !key)
        {
            return outChildren;
        }
		auto impl = reinterpret_cast<YamlNodeImpl*>(node);
		try 
        {
			YAML::Node children = impl->node[key];
			if (children.IsSequence()) 
            {
				for (YAML::Node child : children) 
                {
					auto childImpl = new YamlNodeImpl();
					childImpl->node = child;
					outChildren.push_back(reinterpret_cast<YamlNode*>(childImpl));
				}
			}
		}
		catch (...) 
        {
			set_error("Error getting children from YAML node");
		}

		return outChildren;
	}

    std::vector<YamlNode*> yaml_get_children(YamlNode* node)
    {
        std::vector<YamlNode*> result;
        if (!node)
            return result;

        auto impl = reinterpret_cast<YamlNodeImpl*>(node);

        for (int i = 0; i < impl->node.size(); i++)
        {
            auto childImpl = new YamlNodeImpl();
            childImpl->node = impl->node[i];
            result.push_back(reinterpret_cast<YamlNode*>(childImpl));
        }

        return result;
    }

    std::vector<std::pair<std::string, YamlNode*>> yaml_get_children_map(YamlNode* node)
    {
        std::vector<std::pair<std::string, YamlNode*>> result;
        if (!node)
            return result;

        auto impl = reinterpret_cast<YamlNodeImpl*>(node);

        if (!impl->node.IsMap())
            return result;

        size_t count = yaml_map_size(node);

        for (size_t i = 0; i < count; i++)
        {
			std::pair<std::string, YamlNode*> pair = yaml_map_entry(node, i);

            if (!pair.first.empty() && pair.second)
                result.push_back(pair);
        }

        return result;
    }

    std::pair<std::string, YamlNode*> yaml_map_entry(YamlNode* node, size_t index)
    {
        std::pair<std::string, YamlNode*> result{ "", nullptr };

        if (!node)
            return result;

        auto impl = reinterpret_cast<YamlNodeImpl*>(node);

        if (!impl->node.IsMap())
            return result;
            
        int i = 0;
        for (auto it = impl->node.begin(); it != impl->node.end(); ++it, ++i)
        {
            if (i == index)
            {
                YamlNodeImpl* child = new YamlNodeImpl();
                child->node = it->second;
                std::string name = it->first.as<std::string>();
                return result = { name, reinterpret_cast<YamlNode*>(child) };
            }
        }

        return result;
    }

    std::unordered_map<const char*, YamlNode*> yaml_get_children_map(YamlNode* node, const char* key)
    {
        std::unordered_map<const char*, YamlNode*> outChildren;
        if (!node || !key)
        {
            return outChildren;
        }
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try
        {
            YAML::Node children = impl->node[key];
            if (children.IsSequence())
            {
                for (int i = 0; i < children.size(); i++) 
                {
                    auto childImpl = new YamlNodeImpl();
                    childImpl->node = children[i];
                    std::string name = children[i].as<std::string>();
                    outChildren[name.c_str()] = reinterpret_cast<YamlNode*>(childImpl);
                }
            }
            if (children.IsMap())
            {
                for (auto it = children.begin(); it != children.end(); ++it)
                {
                    auto childImpl = new YamlNodeImpl();
                    childImpl->node = it->second;
                    outChildren[it->first.as<std::string>().c_str()] = reinterpret_cast<YamlNode*>(childImpl);
                }
            }
        }
        catch (...)
        {
            set_error("Error getting children from YAML node");
        }

        return outChildren;
    }

    std::string yaml_as_string(YamlNode* node) 
    {
        if (!node) return 0;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            impl->cache = impl->node.as<std::string>();
            return impl->cache;
        }
        catch (...) {
            return 0;
        }
    }

    char yaml_as_char(YamlNode* node)
    {
        if (!node) return 0;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            return impl->node.as<char>();
        }
        catch (...) {
            return 0;
        }
    }

    int yaml_as_int(YamlNode* node, int default_val) {
        if (!node) return default_val;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            return impl->node.as<int>();
        }
        catch (...) {
            return default_val;
        }
    }

    uint32_t yaml_as_int32(YamlNode* node, uint32_t default_val) {
        if (!node) return default_val;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            return impl->node.as<uint32_t>();
        }
        catch (...) {
            return default_val;
        }
    }

    float yaml_as_float(YamlNode* node, float default_val)
    {
        if (!node) return default_val;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            impl->cache = impl->node.as<float>();
            return impl->node.as<float>();
        }
        catch (...) {
            return default_val;
        }
    }

    double yaml_as_double(YamlNode* node, double default_val) {
        if (!node) return default_val;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            return impl->node.as<double>();
        }
        catch (...) {
            return default_val;
        }
    }

    int yaml_as_bool(YamlNode* node, int default_val) {
        if (!node) return default_val;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            return impl->node.as<bool>() ? 1 : 0;
        }
        catch (...) {
            return default_val;
        }
    }

    Vector2 yaml_as_vec2(YamlNode* node)
    {
        if (!node)
        {
            return Vector2();
        }
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try
        {
            return impl->node.as<Vector2>();
        }
        catch (...)
        {
            return Vector2();
        }
    }

	Vector3 yaml_as_vec3(YamlNode* node) 
    {
        if (!node)
        {
            return Vector3();
        }
		auto impl = reinterpret_cast<YamlNodeImpl*>(node);
		try 
        {
			return impl->node.as<Vector3>();
		}
		catch (...) 
        {
			return Vector3();
		}
	}

    ModelInfo yaml_as_modelinfo(YamlNode* node)
    {
        if (node)
        {
            if (YamlNodeImpl* impl = reinterpret_cast<YamlNodeImpl*>(node))
            {
                return impl->node.as<ModelInfo>();
            }
        }
		return ModelInfo();
    }

    // Map access
    int yaml_map_size(YamlNode* node) {
        if (!node) return 0;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        if (!impl->node.IsMap()) return 0;
        return static_cast<int>(impl->node.size());
    }

    YamlNode* yaml_get_child(YamlNode* node, const char* key) {
        if (!node || !key)
        {
            return nullptr;
        }
        if (YamlNodeImpl* impl = reinterpret_cast<YamlNodeImpl*>(node))
        {
            YAML::Node childNode = impl->node[key];
            if (!childNode)
            {
                return nullptr;
            }
            YamlNodeImpl* child = new YamlNodeImpl();
            child->node = childNode;
            return reinterpret_cast<YamlNode*>(child);
        }
        return nullptr;
    }

    // Sequence access
    int yaml_seq_size(YamlNode* node) {
        if (!node) return 0;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        if (!impl->node.IsSequence()) return 0;
        return static_cast<int>(impl->node.size());
    }

    YamlNode* yaml_seq_element(YamlNode* node, int index) {
        if (!node) return nullptr;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        if (!impl->node.IsSequence() || index < 0 || index >= (int)impl->node.size())
            return nullptr;
        try {
            auto child = new YamlNodeImpl();
            child->node = impl->node[index];
            return reinterpret_cast<YamlNode*>(child);
        }
        catch (...) {
            return nullptr;
        }
    }

    // ===== Emitter API =====
    YamlEmitter* yaml_emitter_new() {
        return reinterpret_cast<YamlEmitter*>(new YamlEmitterImpl());
    }

    void yaml_emitter_free(YamlEmitter* emitter) {
        delete reinterpret_cast<YamlEmitterImpl*>(emitter);
    }

    void yaml_emitter_begin_map(YamlEmitter* emitter) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::BeginMap;
    }

    void yaml_emitter_end_map(YamlEmitter* emitter) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::EndMap;
    }

    void yaml_emitter_begin_seq(YamlEmitter* emitter) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::BeginSeq;
    }

    void yaml_emitter_end_seq(YamlEmitter* emitter) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::EndSeq;
    }

    void yaml_emitter_key(YamlEmitter* emitter, const char* key) {
        if (emitter && key) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Key << key;
    }

    void yaml_emitter_value_string(YamlEmitter* emitter, const char* value) {
        if (emitter && value) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << value;
    }

    void yaml_emitter_value_char(YamlEmitter* emitter, char value) {
        if (emitter && value) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << value;
    }

    void yaml_emitter_value_int(YamlEmitter* emitter, int value) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << value;
    }

    void yaml_emitter_value_double(YamlEmitter* emitter, double value) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << value;
    }

    void yaml_emitter_value_float(YamlEmitter* emitter, float value) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << value;
    }

    void yaml_emitter_value_bool(YamlEmitter* emitter, int value) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << (value ? true : false);
    }

    void yaml_emitter_value_int32(YamlEmitter* emitter, uint32_t value) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << value;
    }

    void yaml_emitter_value_vec2(YamlEmitter* emitter, const Vector2& vec) {
        if (emitter) {
            auto impl = reinterpret_cast<YamlEmitterImpl*>(emitter);
            impl->out << YAML::Value << YAML::Flow << YAML::BeginSeq << vec.X << vec.Y << YAML::EndSeq;
        }
	}

    void yaml_emitter_value_vec3(YamlEmitter* emitter, const Vector3& vec) {
        if (emitter) {
            auto impl = reinterpret_cast<YamlEmitterImpl*>(emitter);
            impl->out << YAML::Value << YAML::Flow << YAML::BeginSeq << vec.X << vec.Y << vec.Z << YAML::EndSeq;
        }
	}

    void yaml_emitter_value_vec4(YamlEmitter* emitter, const Vector4& vec) {
        if (emitter) {
            auto impl = reinterpret_cast<YamlEmitterImpl*>(emitter);
            impl->out << YAML::Value << YAML::Flow << YAML::BeginSeq << vec.X << vec.Y << vec.Z << vec.W << YAML::EndSeq;
        }
    }

    RAZOR_API void yaml_emitter_value_modelinfo(YamlEmitter* emitter, const ModelInfo& value)
    {
		if (emitter) {
			auto impl = reinterpret_cast<YamlEmitterImpl*>(emitter);
            impl->out << YAML::Value << value;
		}
    }

    int yaml_emitter_get_string(YamlEmitter* emitter, char* buffer, int bufferSize) {
        if (!emitter) return 0;
        auto impl = reinterpret_cast<YamlEmitterImpl*>(emitter);
        impl->cache = impl->out.c_str();
        if (buffer && bufferSize > 0) {
            strncpy(buffer, impl->cache.c_str(), bufferSize - 1);
            buffer[bufferSize - 1] = '\0';
        }
        return static_cast<int>(impl->cache.size());
    }

	void yaml_emitter_value_seq(YamlEmitter* emitter) 
    {
		if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << YAML::BeginSeq;
	}

    void yaml_emitter_value_map(YamlEmitter* emitter)
    {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << YAML::BeginMap;
    }

	const char* yaml_emitter_cstr(YamlEmitter* emitter) {
		if (!emitter) return nullptr;
		auto impl = reinterpret_cast<YamlEmitterImpl*>(emitter);
		impl->cache = impl->out.c_str();
		return impl->cache.c_str();
	}


    void yaml_node_push_back(YamlNode* node, void* val) {
        if (!node || !val) return;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            impl->node.push_back(*reinterpret_cast<YAML::Node*>(val));
        }
        catch (const std::exception& e) {
            set_error(e.what());
        }
        catch (...) {
            set_error("Unknown error in yaml_node_push_back");
        }
	}
}