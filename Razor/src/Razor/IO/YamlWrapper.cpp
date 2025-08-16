#include "YamlWrapper.h"
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <sstream>
#include <exception>

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

}