#pragma once
#include "TwoD/Debug/Debuggable.hpp"

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
	
	class Transform;
	class UITransform;
	class Canvas;
}

#define TD_INTERNAL_COMPONENT_FIELD(values) TD_CHOOSE_MACRO_4(TD_INTERNAL_COMPONENT_FIELD_NO_DEFAULT, TD_INTERNAL_COMPONENT_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_COMPONENT_FIELD_WITH_DEFAULT(updater, type, name, value) type name = value;
#define TD_INTERNAL_COMPONENT_FIELD_NO_DEFAULT(updater, type, name) type name;

#define TD_INTERNAL_COMPONENT_LOAD_DATA(...) struct internal_load_data { \
		TD_APPLY_EACH(TD_INTERNAL_COMPONENT_FIELD, __VA_ARGS__) \
	};

#define TD_INTERNAL_COMPONENT_LOAD_FIELD(values) TD_INTERNAL_COMPONENT_LOAD_FIELD_UNWRAP values
#define TD_INTERNAL_COMPONENT_LOAD_FIELD_UNWRAP(updater, type, name, ...) name = loadData->name;

#define TD_INTERNAL_COMPONENT_LOAD(...) void Load(const void* data) override { \
		auto* loadData = static_cast<const internal_load_data*>(data); \
		TD_APPLY_EACH(TD_INTERNAL_COMPONENT_LOAD_FIELD, __VA_ARGS__) \
	}

#define TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD(values) TD_CHOOSE_MACRO_4(TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD_NO_DEFAULT, TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD_WITH_DEFAULT(updater, type, name, value) if (node[#name]) { loadData->name = node[#name].as<type>(); }
#define TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD_NO_DEFAULT(updater, type, name) TD_CORE_ASSERT(node[#name]); \
	loadData->name = node[#name].as<type>();

#define TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA(...) static void CreateLoadData(internal_load_data* loadData, const YAML::Node& node) { \
		TD_APPLY_EACH(TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA_FIELD, __VA_ARGS__) \
	}

#define TD_INTERNAL_COMPONENT_DEBUG_FIELD(values) TD_INTERNAL_COMPONENT_DEBUG_FIELD_UNWRAP values
#define TD_INTERNAL_COMPONENT_DEBUG_FIELD_UNWRAP(updater, type, f_name, ...) if(::TwoD::Debuggable<type>::Draw( \
		f_name, \
		std::format("{}##{}", #f_name, typeid(std::remove_pointer_t<decltype(this)>).name()).c_str()) \
	) { updater }

#define TD_INTERNAL_COMPONENT_DEBUG(...) void Debug() override { \
		TD_APPLY_EACH(TD_INTERNAL_COMPONENT_DEBUG_FIELD, __VA_ARGS__) \
	}

#define TD_COMPONENT(...) TD_APPLY_EACH(TD_INTERNAL_COMPONENT_FIELD, __VA_ARGS__) \
	TD_INTERNAL_COMPONENT_LOAD_DATA(__VA_ARGS__) \
	TD_INTERNAL_COMPONENT_LOAD(__VA_ARGS__) \
	TD_INTERNAL_COMPONENT_CREATE_LOAD_DATA(__VA_ARGS__) \
	TD_INTERNAL_COMPONENT_DEBUG(__VA_ARGS__)


#define TD_INTERNAL_COMPONENT_FIELD_NO_UPDATER
#define TD_INTERNAL_COMPONENT_FIELD_WITH_UPDATER_NO_DEFAULT(type, name, updater) (updater, type, name)
#define TD_INTERNAL_COMPONENT_FIELD_WITH_UPDATER_WITH_DEFAULT(type, name, value, updater) (updater, type, name, value)

#define TD_COMPONENT_FIELD(...) (TD_INTERNAL_COMPONENT_FIELD_NO_UPDATER, __VA_ARGS__)
#define TD_COMPONENT_FIELD_WITH_UPDATER(...) TD_CHOOSE_MACRO_4(TD_INTERNAL_COMPONENT_FIELD_WITH_UPDATER_NO_DEFAULT, TD_INTERNAL_COMPONENT_FIELD_WITH_UPDATER_WITH_DEFAULT, (__VA_ARGS__))