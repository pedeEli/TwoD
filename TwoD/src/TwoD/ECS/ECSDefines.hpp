#pragma once


namespace TwoD
{
	class Component;
	class Entity;
	class EntityHandle;
	class CompenentStorage;
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	class ComponentStorageImpl;
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	class ComponentHandle;
}

#define TD_INTERNAL_COMPONENT_FIELD(values) TD_CHOOSE_MACRO_3(TD_INTERNAL_COMPONENT_FIELD_NO_DEFAULT, TD_INTERNAL_COMPONENT_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_COMPONENT_FIELD_WITH_DEFAULT(type, name, value) type name = value;
#define TD_INTERNAL_COMPONENT_FIELD_NO_DEFAULT(type, name) type name;

#define TD_INTERNAL_COMPONENT_LOAD_DATA(...) struct internal_load_data { \
		TD_APPLY_EACH(TD_INTERNAL_COMPONENT_FIELD, __VA_ARGS__) \
	};

#define TD_INTERNAL_COMPONENT_LOAD_FIELD(values) TD_INTERNAL_COMPONENT_LOAD_FIELD_UNWRAP values
#define TD_INTERNAL_COMPONENT_LOAD_FIELD_UNWRAP(type, name, ...) name = loadData->name;

#define TD_INTERNAL_COMPONENT_LOAD(...) void Load(const void* data) override { \
		auto* loadData = static_cast<const internal_load_data*>(data); \
		TD_APPLY_EACH(TD_INTERNAL_COMPONENT_LOAD_FIELD, __VA_ARGS__) \
	}

#define TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD(values) TD_CHOOSE_MACRO_3(TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD_NO_DEFAULT, TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD_WITH_DEFAULT(type, name, value) if (node[#name]) { loadData->name = node[#name].as<type>(); }
#define TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD_NO_DEFAULT(type, name) TD_CORE_ASSERT(node[#name]); \
	loadData->name = node[#name].as<type>();

#define TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA(...) static const void* CreateLoadData(const YAML::Node& node) { \
		auto loadData = new internal_load_data(); \
		TD_APPLY_EACH(TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD, __VA_ARGS__) \
		return loadData; \
	}

#define TD_COMPONENT(...) TD_APPLY_EACH(TD_INTERNAL_COMPONENT_FIELD, __VA_ARGS__) \
	TD_INTERNAL_COMPONENT_LOAD_DATA(__VA_ARGS__) \
	TD_INTERNAL_COMPONENT_LOAD(__VA_ARGS__) \
	TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA(__VA_ARGS__)

#define TD_COMPONENT_FIELD(...) (__VA_ARGS__)