#pragma once
#include "TwoD/Debug/Debuggable.hpp"
#include "TwoD/Serialization/Serialization.hpp"
#include "TwoD/Core/Macros.hpp"

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

// component base

#define TDI_COMPONENT_BASE(data) TD_IF_ELSE((TD_GET4 data), : public TD_GET4 data, : public ::TwoD::Component)


// component load data (ld)

#define TDI_COMPONENT_LD_START(meta) TD_IF_ELSE( \
		(TDI_GET_BASE(meta)), \
		struct td_load_data : public TDI_GET_BASE(meta)::td_load_data {, \
		struct td_load_data { \
	)
#define TDI_COMPONENT_LD_FIELD(field) TD_UNWRAP TD_IF_ELSE( \
		(TDI_GET_NO_SERIALIZE(field)), \
		(), \
		(TDI_GET_FIELD_DEF(field)) \
	)
#define TDI_COMPONENT_LD(meta, ...) \
	TDI_COMPONENT_LD_START(meta) \
		__VA_OPT__(TD_APPLY_EACH_CONCAT(TDI_COMPONENT_LD_FIELD, __VA_ARGS__)) \
	};


// component fields

#define TDI_COMPONENT_FIELDS(...) __VA_OPT__(TD_APPLY_EACH_CONCAT(TDI_GET_FIELD_DEF, __VA_ARGS__))


// component create load data (ld)

#define TDI_COMPONENT_CREATE_LD_FIELD_WITH_INIT(name, type) \
	if (deserializer[#name] && !deserializer[#name].As<type>(data->name)) { \
		return false; \
	}
#define TDI_COMPONENT_CREATE_LD_FIELD_NO_INIT(name, type) \
	if (!deserializer[#name] || !deserializer[#name].As<type>(data->name)) { \
		return false; \
	}
#define TDI_COMPONENT_CREATE_LD_FIELD(field) TD_IF_ELSE( \
		(TDI_GET_NO_SERIALIZE(field)),, \
		TD_IF_ELSE( \
			(TDI_GET_INIT(field)), \
			TD_DEFER(TDI_COMPONENT_CREATE_LD_FIELD_WITH_INIT)(TDI_GET_NAME(field), TDI_GET_TYPE(field)), \
			TD_DEFER(TDI_COMPONENT_CREATE_LD_FIELD_NO_INIT)(TDI_GET_NAME(field), TDI_GET_TYPE(field)) \
		) \
	)
#define TDI_COMPONENT_CREATE_LD(...) \
	static bool TDCreateLoadData([[maybe_unused]] td_load_data* data, [[maybe_unused]] const ::TwoD::Deserializer& deserializer) { \
		__VA_OPT__(TD_APPLY_EACH_CONCAT(TDI_COMPONENT_CREATE_LD_FIELD, __VA_ARGS__)) \
		return true; \
	}


// component modify load data (ld)

#define TDI_COMPONENT_MODIFY_LD_FIELD_DEF(name, type) \
	if (deserializer[#name]) { \
		if (!deserializer[#name].As<type>(data->name)) { \
			return false; \
		} \
	}
#define TDI_COMPONENT_MODIFY_LD_FIELD(field) TD_IF_ELSE( \
		(TDI_GET_NO_SERIALIZE(field)),, \
		TD_DEFER(TDI_COMPONENT_MODIFY_LD_FIELD_DEF)(TDI_GET_NAME(field), TDI_GET_TYPE(field)) \
	)
#define TDI_COMPONENT_MODIFY_LD(...) \
	static bool TDModifyLoadData([[maybe_unused]] td_load_data* data, [[maybe_unused]] const ::TwoD::Deserializer& deserializer) { \
		__VA_OPT__(TD_APPLY_EACH_CONCAT(TDI_COMPONENT_MODIFY_LD_FIELD, __VA_ARGS__)) \
		return true; \
	}


// component constructor

#define TDI_COMPONENT_CON_INIT_BASE(meta) TD_IF_ELSE( \
		(TDI_GET_BASE(meta)), \
		TDI_GET_BASE(meta)(handle, static_cast<const TDI_GET_BASE(meta)::td_load_data*>(data)), \
		Component(handle) \
	)
#define TDI_COMPONENT_CON_INIT_FIELD(field) TD_UNWRAP TD_IF_ELSE( \
		(TDI_GET_NO_SERIALIZE(field)), \
		(), \
		(, TDI_GET_NAME(field)(data->TDI_GET_NAME(field))) \
	)
#define TDI_COMPONENT_CON_INIT_FIELDS(...) __VA_OPT__(TD_APPLY_EACH_CONCAT(TDI_COMPONENT_CON_INIT_FIELD, __VA_ARGS__))
#define TDI_COMPONENT_CON(meta, ...) \
	TDI_GET_NAME(meta)( \
		::TwoD::EntityHandle handle, [[maybe_unused]] const td_load_data* data \
	) : TDI_COMPONENT_CON_INIT_BASE(meta) TDI_COMPONENT_CON_INIT_FIELDS(__VA_ARGS__) {} \
	TDI_GET_NAME(meta)(::TwoD::EntityHandle handle) : TD_IF_ELSE( \
		(TDI_GET_BASE(meta)), \
		TDI_GET_BASE(meta), \
		Component \
	)(handle) {}


// component debug

#ifdef TD_CREATE_DEBUGGER
#define TDI_COMPONENT_DEBUG_BASE(meta) TD_IF_ELSE( \
		(TDI_GET_BASE(meta)), \
		TDI_GET_BASE(meta)::Debug();, \
	)
#define TDI_COMPONENT_DEBUG_FIELD_DRAW(field) ::TwoD::Debuggable<TDI_GET_TYPE(field)>::Draw( \
		TDI_GET_NAME(field), \
		std::format("{}##{}", TD_DEFER(TD_STRINGIFY)(TDI_GET_NAME(field)), name).c_str() \
	)
#define TDI_COMPONENT_DEBUG_FIELD(field) TD_IF_ELSE( \
		(TDI_GET_NO_DEBUG(field)),, \
		TD_IF_ELSE( \
			(TDI_GET_UPDATER(field)), \
			if (TDI_COMPONENT_DEBUG_FIELD_DRAW(field)) { TD_UNWRAP TDI_GET_UPDATER(field); }, \
			TDI_COMPONENT_DEBUG_FIELD_DRAW(field); \
		) \
	)
#define TDI_COMPONENT_DEBUG(meta, ...) \
	void Debug() override { \
		[[maybe_unused]] const char* name = TD_DEFER(TD_STRINGIFY)(TDI_GET_NAME(meta)); \
		TDI_COMPONENT_DEBUG_BASE(meta) \
		__VA_OPT__(TD_APPLY_EACH_CONCAT(TDI_COMPONENT_DEBUG_FIELD, __VA_ARGS__)) \
	}
#else
#define TDI_COMPONENT_DEBUG(meta, fields)
#endif


// component builder

#define TDI_COMPONENT_BUILD(meta, fields) \
	TDI_GET_NAMESPACE_START(meta) \
		TDI_GET_TEMPLATE_DEF(meta) \
		class TDI_GET_NAME(meta) TDI_COMPONENT_BASE(meta) { \
		public: \
			TDI_COMPONENT_LD(meta, TD_UNWRAP fields) \
			TDI_COMPONENT_FIELDS(TD_UNWRAP fields) \
			TDI_COMPONENT_CREATE_LD(TD_UNWRAP fields) \
			TDI_COMPONENT_MODIFY_LD(TD_UNWRAP fields) \
			TDI_COMPONENT_CON(meta, TD_UNWRAP fields) \
			TDI_COMPONENT_DEBUG(meta, TD_UNWRAP fields)


#define TD_COMPONENT_FIELD(type, name, ...)  TDI_DATA_COMBINE(TDI_EMPTY_COMPONENT_FIELDDATA, TD_NAME(name), TD_TYPE(type) __VA_OPT__(, __VA_ARGS__))
#define TD_COMPONENT(meta, fields) TD_EXPAND(TDI_COMPONENT_BUILD( \
		TDI_DATA_COMBINE(TDI_EMPTY_COMPONENT_METADATA, TD_UNWRAP meta), \
		fields \
	))