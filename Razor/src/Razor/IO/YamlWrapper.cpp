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
}

namespace Razor
{

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

    int yaml_as_string(YamlNode* node, char* buffer, int bufferSize) {
        if (!node) return 0;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            impl->cache = impl->node.as<std::string>();
            if (buffer && bufferSize > 0) {
                strncpy(buffer, impl->cache.c_str(), bufferSize - 1);
                buffer[bufferSize - 1] = '\0';
            }
            return static_cast<int>(impl->cache.size());
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

    // Map access
    int yaml_map_size(YamlNode* node) {
        if (!node) return 0;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        if (!impl->node.IsMap()) return 0;
        return static_cast<int>(impl->node.size());
    }

    const char* yaml_map_key_at(YamlNode* node, int index) {
        if (!node) return nullptr;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        if (!impl->node.IsMap() || index < 0 || index >= (int)impl->node.size())
            return nullptr;
        try {
            auto it = impl->node.begin();
            std::advance(it, index);
            impl->cache = it->first.as<std::string>();
            return impl->cache.c_str();
        }
        catch (...) {
            return nullptr;
        }
    }

    YamlNode* yaml_map_value_at(YamlNode* node, int index) {
        if (!node) return nullptr;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        if (!impl->node.IsMap() || index < 0 || index >= (int)impl->node.size())
            return nullptr;
        try {
            auto it = impl->node.begin();
            std::advance(it, index);
            auto child = new YamlNodeImpl();
            child->node = it->second;
            return reinterpret_cast<YamlNode*>(child);
        }
        catch (...) {
            return nullptr;
        }
    }

    YamlNode* yaml_get_child(YamlNode* node, const char* key) {
        if (!node || !key) return nullptr;
        auto impl = reinterpret_cast<YamlNodeImpl*>(node);
        try {
            YAML::Node childNode = impl->node[key];
            if (!childNode) return nullptr;
            auto child = new YamlNodeImpl();
            child->node = childNode;
            return reinterpret_cast<YamlNode*>(child);
        }
        catch (...) {
            return nullptr;
        }
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

    void yaml_emitter_value_int(YamlEmitter* emitter, int value) {
        if (emitter) reinterpret_cast<YamlEmitterImpl*>(emitter)->out << YAML::Value << value;
    }

    void yaml_emitter_value_double(YamlEmitter* emitter, double value) {
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