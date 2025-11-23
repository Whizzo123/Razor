#pragma once

#include "../Core.h"
#include <vector>
#include <unordered_map>

namespace Razor
{

	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct ModelInfo;

	// ====== Error API ======
	RAZOR_API const char* yaml_get_last_error();

	// ====== Node API ======
	typedef struct YamlNode YamlNode;

	// Load a YAML file -> root node
	RAZOR_API YamlNode* yaml_load_file(const char* filename);

	// Load from string
	RAZOR_API YamlNode* yaml_load_string(const char* yamlText);

	// Free a node (and any children created via API)
	RAZOR_API void yaml_free_node(YamlNode* node);

	RAZOR_API void yaml_node_push_back(YamlNode* node, void* val);

	RAZOR_API std::vector<YamlNode*> yaml_get_children(YamlNode* node, const char* key);
	RAZOR_API std::vector<YamlNode*> yaml_get_children(YamlNode* node);
	RAZOR_API std::unordered_map<const char*, YamlNode*> yaml_get_children_map(YamlNode* node, const char* key);
	RAZOR_API std::vector<std::pair<std::string, YamlNode*>> yaml_get_children_map(YamlNode* node);

	// Query scalar types
	RAZOR_API std::string yaml_as_string(YamlNode* node);
	RAZOR_API char yaml_as_char(YamlNode* node);
	RAZOR_API int yaml_as_int(YamlNode* node, int default_val);
	RAZOR_API uint32_t yaml_as_int32(YamlNode* node, uint32_t default_val);
	RAZOR_API float yaml_as_float(YamlNode* node, float default_val);
	RAZOR_API double yaml_as_double(YamlNode* node, double default_val);
	RAZOR_API int yaml_as_bool(YamlNode* node, int default_val); 
	RAZOR_API Vector2 yaml_as_vec2(YamlNode* node);
	RAZOR_API Vector3 yaml_as_vec3(YamlNode* node);
	RAZOR_API ModelInfo yaml_as_modelinfo(YamlNode* node);

	// Map access
	RAZOR_API int yaml_map_size(YamlNode* node);
	RAZOR_API YamlNode* yaml_get_child(YamlNode* node, const char* key);

	// Sequence access
	RAZOR_API int yaml_seq_size(YamlNode* node);
	RAZOR_API YamlNode* yaml_seq_element(YamlNode* node, int index);

	// ====== Emitter API ======
	typedef struct YamlEmitter YamlEmitter;

	RAZOR_API YamlEmitter* yaml_emitter_new();
	RAZOR_API void yaml_emitter_free(YamlEmitter* emitter);

	RAZOR_API void yaml_emitter_begin_map(YamlEmitter* emitter);
	RAZOR_API void yaml_emitter_end_map(YamlEmitter* emitter);
	RAZOR_API void yaml_emitter_begin_seq(YamlEmitter* emitter);
	RAZOR_API void yaml_emitter_end_seq(YamlEmitter* emitter);

	RAZOR_API void yaml_emitter_key(YamlEmitter* emitter, const char* key);

	RAZOR_API void yaml_emitter_value_string(YamlEmitter* emitter, const char* value);
	RAZOR_API void yaml_emitter_value_char(YamlEmitter* emitter, char value);
	RAZOR_API void yaml_emitter_value_int(YamlEmitter* emitter, int value);
	RAZOR_API void yaml_emitter_value_double(YamlEmitter* emitter, double value);
	RAZOR_API void yaml_emitter_value_float(YamlEmitter* emitter, float value);
	RAZOR_API void yaml_emitter_value_bool(YamlEmitter* emitter, int value);
	RAZOR_API void yaml_emitter_value_seq(YamlEmitter* emitter);
	RAZOR_API void yaml_emitter_value_map(YamlEmitter* emitter);
	RAZOR_API void yaml_emitter_value_int32(YamlEmitter* emitter, uint32_t value);
	RAZOR_API void yaml_emitter_value_vec2(YamlEmitter* emitter, const Vector2& value);
	RAZOR_API void yaml_emitter_value_vec3(YamlEmitter* emitter, const Vector3& value);
	RAZOR_API void yaml_emitter_value_vec4(YamlEmitter* emitter, const Vector4& value);
	RAZOR_API void yaml_emitter_value_modelinfo(YamlEmitter* emitter, const ModelInfo& value);

	RAZOR_API const char* yaml_emitter_cstr(YamlEmitter* emitter);

	// Retrieve final emitted string
	RAZOR_API int yaml_emitter_get_string(YamlEmitter* emitter, char* buffer, int bufferSize);

	std::pair<std::string, YamlNode*> yaml_map_entry(YamlNode* node, size_t index);
}