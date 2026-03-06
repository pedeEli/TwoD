#pragma once
#include "SerializationDefines.hpp"
#include "Deserializer.hpp"
#include "Deserializable.hpp"


#include "TwoD/Generated/HelperMacros.hpp"
#include "TwoD/Debug/Debuggable.hpp"
/*
* Everything is first converted into this data structure.
* This means that the struct metadata, struct fielddata, enum metadata and enum fielddata all share this layout.
* The datatype that is currently represented is put as the first element for debugging.
* Depending on what type of data is represented some tuple fields might remain empty (e.g. template is not used in enum metadata).
*
* Data layout: (datatype, name, namespace, base, template, type, init, noSerialize, noDebug)
* struct meta:			  name  namespace  base  template
* struct field:			  name							   type  init  noSerialize  noDebug
*/

// data
#define TDI_EMPTY_DATA ,,,,,,,,,
#define TDI_EMPTY_STRUCT_METADATA TD_APPLY(TD_SET1, STRUCT_METADATA, TDI_EMPTY_DATA)
#define TDI_EMPTY_STRUCT_FIELDDATA TD_APPLY(TD_SET1, STRUCT_FIELDDATA, TDI_EMPTY_DATA)
#define TDI_EMPTY_ENUM_METADATA TD_APPLY(TD_SET1, ENUM_METADATA, TDI_EMPTY_DATA)
#define TDI_EMPTY_ENUM_FIELDDATA TD_APPLY(TD_SET1, ENUM_FIELDDATA, TDI_EMPTY_DATA)

// data creation
#define TD_NAME(name) TD_APPLY(TD_SET2, name, TDI_EMPTY_DATA)
#define TD_NAMESPACE(ns) TD_APPLY(TD_SET3, ns, TDI_EMPTY_DATA)
#define TD_BASE(base) TD_APPLY(TD_SET4, base, TDI_EMPTY_DATA)
#define TD_TEMPLATE(...) TD_APPLY(TD_SET5, (__VA_ARGS__), TDI_EMPTY_DATA)
#define TD_TYPE(...) TD_APPLY(TD_SET6, (__VA_ARGS__), TDI_EMPTY_DATA)
#define TD_INIT(init) TD_APPLY(TD_SET7, init, TDI_EMPTY_DATA)
#define TD_NO_SERIALIZE TD_APPLY(TD_SET8, NO_SERIALIZE, TDI_EMPTY_DATA)
#define TD_NO_DEBUG TD_APPLY(TD_SET9, NO_DEBUG, TDI_EMPTY_DATA)

// data retrieval
#define TDI_GET_NAME(data) TD_GET2 data
#define TDI_GET_NAMESPACE_START_CREATE(ns) namespace ns {
#define TDI_GET_NAMESPACE(data) TD_IF_ELSE((TD_GET3 data), ::, ) TD_GET3 data TD_IF_ELSE((TD_GET3, data), ::, )
#define TDI_GET_NAMESPACE_START(data) TD_APPLY_IF(TDI_GET_NAMESPACE_START_CREATE, (TD_GET3 data))
#define TDI_GET_NAMESPACE_CLOSE(data) TD_IF_ELSE((TD_GET3 data), }, )
#define TDI_GET_BASE(data) TD_APPLY_IF(: TD_UNWRAP, (TD_GET4 data))
#define TDI_GET_TEMPLATE(data) TD_GET5 data
#define TDI_GET_TYPE(data) TD_UNWRAP TD_GET6 data
#define TDI_GET_INIT(data) TD_GET7 data
#define TDI_GET_NO_SERIALIZE(data) TD_GET8 data
#define TDI_GET_NO_DEBUG(data) TD_GET9 data

#define TDI_GET_TEMPLATE_DEF_TYPE(type) TD_GET1 type TD_GET2 type
#define TDI_GET_TEMPLATE_DEF(data, ...) TD_IF_ELSE( \
		(TDI_GET_TEMPLATE(data)), \
		template<TD_DEFER(TD_APPLY_EACH)(TDI_GET_TEMPLATE_DEF_TYPE, TD_UNWRAP TDI_GET_TEMPLATE(data))>, \
		__VA_OPT__(__VA_ARGS__) \
	)

#define TDI_GET_QUALIFIED_NAME(data) \
	TDI_GET_NAMESPACE(data) \
	TDI_GET_NAME(data) \
	TD_IF_ELSE( \
		(TDI_GET_TEMPLATE(data)), \
		<TD_DEFER(TD_APPLY_EACH)(TD_GET2 TD_UNWRAP, TD_UNWRAP TDI_GET_TEMPLATE(data))>, \
	)

// data combinator
#define TDI_DATA_CONCAT(x, y) TD_DEFER(TD_ZIP_WITH)(TD_CONCAT_TOKEN, x, y)
#define TDI_DATA_COMBINE(d, ...) TD_EVAL(TD_FOLD_WITH(TDI_DATA_CONCAT, d, __VA_ARGS__))


// struct defs
#define TDI_STRUCT_FIELD_DEF(field) TDI_GET_TYPE(field) TDI_GET_NAME(field) TD_APPLY_IF(= TD_EXPAND, (TDI_GET_INIT(field)));
#define TDI_STRUCT_DEF(meta, fields, ...) \
	TDI_GET_TEMPLATE_DEF(meta) \
	struct TDI_GET_NAME(meta) TDI_GET_BASE(meta) { \
	public: \
		TD_APPLY_EACH_CONCAT(TDI_STRUCT_FIELD_DEF, TD_UNWRAP fields) \
		__VA_ARGS__ \
	};

// struct serialization
#define TDI_STRUCT_DESERIALIZE_FIELD(field) TD_IF_ELSE( \
		(TDI_GET_NO_SERIALIZE(field)),, \
		TD_IF_ELSE((TDI_GET_INIT(field)), if (deserializer[TD_DEFER(TD_STRINGIFY)(TDI_GET_NAME(field))]) {, ) \
		success &= deserializer[TD_DEFER(TD_STRINGIFY)(TDI_GET_NAME(field))].As<TDI_GET_TYPE(field)>(value.TDI_GET_NAME(field)); \
		TD_IF_ELSE((TDI_GET_INIT(field)), }, ) \
	)
#define TDI_STRUCT_DESERIALIZABLE(meta, fields) \
	namespace TwoD { \
		TDI_GET_TEMPLATE_DEF(meta, template<>) \
		struct Deserializable<TDI_GET_QUALIFIED_NAME(meta)> { \
			static bool Deserialize(const Deserializer& deserializer, TDI_GET_QUALIFIED_NAME(meta)& value) { \
				bool success = true; \
				TD_APPLY_EACH_CONCAT(TDI_STRUCT_DESERIALIZE_FIELD, TD_UNWRAP fields) \
				return success; \
			} \
		}; \
	}

// struct debug
#ifdef TD_CREATE_DEBUGGER
#define TDI_STRUCT_DEBUG_FIELD(field) \
	TD_IF_ELSE( \
		(TDI_GET_NO_DEBUG(field)),, \
		changed |= Debuggable<TDI_GET_TYPE(field)>::Draw(value.TDI_GET_NAME(field), TD_DEFER(TD_STRINGIFY)(TDI_GET_NAME(field))); \
	)
#define TDI_STRUCT_DEBUG(meta, fields) \
	namespace TwoD { \
		TDI_GET_TEMPLATE_DEF(meta, template<>) \
		struct Debuggable<TDI_GET_QUALIFIED_NAME(meta)> { \
			static bool Draw(TDI_GET_QUALIFIED_NAME(meta)& value, const char* name) { \
				bool changed = false; \
				TD_APPLY_EACH_CONCAT(TDI_STRUCT_DEBUG_FIELD, TD_UNWRAP fields) \
				return changed; \
			} \
		}; \
	}
#else
#define TDI_STRUCT_DEBUG(meta, fields)
#endif

// struct builder
#define TDI_STRUCT_BUILD(meta, fields, ...) \
	TDI_GET_NAMESPACE_START(meta) \
		TDI_STRUCT_DEF(meta, fields, __VA_ARGS__) \
	TDI_GET_NAMESPACE_CLOSE(meta) \
	TDI_STRUCT_DESERIALIZABLE(meta, fields) \
	TDI_STRUCT_DEBUG(meta, fields)


// "exposed" macros
#define TD_STRUCT_FIELD(type, name, ...) TDI_DATA_COMBINE(TDI_EMPTY_STRUCT_FIELDDATA, TD_NAME(name), TD_TYPE(type) __VA_OPT__(, __VA_ARGS__))
#define TD_STRUCT(meta, fields, ...) TD_EXPAND(TDI_STRUCT_BUILD( \
		TDI_DATA_COMBINE(TDI_EMPTY_STRUCT_METADATA, TD_UNWRAP meta), \
		fields, \
		__VA_ARGS__ \
	))



//TD_STRUCT(
//	(TD_NAME(Test)),
//	(TD_STRUCT_FIELD(uint8_t, age))
//)
//TD_STRUCT(
//	(TD_NAME(Foo), TD_NAMESPACE(TwoD)),
//	(
//		TD_STRUCT_FIELD(int, age, TD_INIT(42)),
//		TD_STRUCT_FIELD(std::string, name),
//		TD_STRUCT_FIELD(float, speed, TD_INIT(6.9f)),
//		TD_STRUCT_FIELD(glm::fvec2, position, TD_NO_SERIALIZE, TD_NO_DEBUG)
//	),
//)

// enum defs
#define TDI_ENUM_FIELD_DEF(field) TDI_GET_NAME(field) TD_APPLY_IF(= TD_EXPAND, (TDI_GET_INIT(field)))
#define TDI_ENUM_DEF(meta, fields) \
	enum class TDI_GET_NAME(meta) TDI_GET_BASE(meta) { \
		TD_APPLY_EACH(TDI_ENUM_FIELD_DEF, TD_UNWRAP fields) \
	};
#define TDI_ENUM_UNDERLYING_TYPE(meta) std::underlying_type_t<TDI_GET_QUALIFIED_NAME(meta)>
#define TDI_ENUM_BIT_OPERATOR(meta, op) \
	constexpr TDI_GET_QUALIFIED_NAME(meta) operator op(TDI_GET_QUALIFIED_NAME(meta) a, TDI_GET_QUALIFIED_NAME(meta) b) noexcept { \
		return static_cast<TDI_GET_QUALIFIED_NAME(meta)>(static_cast<TDI_ENUM_UNDERLYING_TYPE(meta)>(a) op static_cast<TDI_ENUM_UNDERLYING_TYPE(meta)>(b)); \
	} \
	constexpr TDI_GET_QUALIFIED_NAME(meta)& operator op##=(TDI_GET_QUALIFIED_NAME(meta)& a, TDI_GET_QUALIFIED_NAME(meta) b) noexcept { \
		a = a op b; \
		return a; \
	}
#define TDI_ENUM_BOOL_OPERATOR(meta, op) \
	constexpr bool operator op(TDI_GET_QUALIFIED_NAME(meta) a, TDI_GET_QUALIFIED_NAME(meta) b) noexcept { \
		return static_cast<TDI_ENUM_UNDERLYING_TYPE(meta)>(a) op static_cast<TDI_ENUM_UNDERLYING_TYPE(meta)>(b); \
	}
#define TDI_ENUM_BIT_UNARY_OPERATOR(meta, op) \
	constexpr TDI_GET_QUALIFIED_NAME(meta) operator op(TDI_GET_QUALIFIED_NAME(meta) a) noexcept { \
		return static_cast<TDI_GET_QUALIFIED_NAME(meta)>(op static_cast<TDI_ENUM_UNDERLYING_TYPE(meta)>(a)); \
	}

// enum serialization
#define TDI_ENUM_DESERIALIZE_FIELD(field) TD_IF_ELSE( \
		(TDI_GET_NO_SERIALIZE(field)),, \
		if (str == TD_APPLY(TD_STRINGIFY, TDI_GET_NAME(field))) { \
			value = _type :: TDI_GET_NAME(field); \
			return true; \
		} \
	)
#define TDI_ENUM_DESERIALIZABLE(meta, fields) \
	namespace TwoD { \
		template<> \
		struct Deserializable<TDI_GET_QUALIFIED_NAME(meta)> { \
			static bool Deserialize(const Deserializer& deserializer, TDI_GET_QUALIFIED_NAME(meta)& value) { \
				using _type = TDI_GET_QUALIFIED_NAME(meta); \
				auto f = [](const Deserializer& deserializer, _type& value) { \
					std::string str; \
					if (!deserializer.As<std::string>(str)) { \
						return false; \
					} \
					TD_APPLY_EACH_CONCAT(TDI_ENUM_DESERIALIZE_FIELD, TD_UNWRAP fields) \
					return false; \
				}; \
				if (deserializer.IsSequence()) { \
					auto size = deserializer.GetSize(); \
					for (size_t i = 0; i < size; i ++) { \
						_type v; \
						if (!f(deserializer[i], v)) { \
							return false; \
						} \
						value |= v; \
					} \
					return true; \
				} else { \
					return f(deserializer, value); \
				} \
			} \
		}; \
	}

// enum debug
#ifdef TD_CREATE_DEBUGGER
#define TDI_ENUM_DEBUG_IS_FLAGS_FIELD(field) \
	TD_IF_ELSE( \
		(TDI_GET_NO_DEBUG(field)), \
		&& true, \
		&& (static_cast<_underlying>(_type::TDI_GET_NAME(field)) == 0 || std::has_single_bit(static_cast<_unsigned>(_type::TDI_GET_NAME(field)))) \
	)
#define TDI_ENUM_DEBUG_IS_FLAGS(fields) true TD_APPLY_EACH_CONCAT(TDI_ENUM_DEBUG_IS_FLAGS_FIELD, TD_UNWRAP fields)

#define TDI_ENUM_DEBUG_FLAGS_FIELD(field) \
	TD_IF_ELSE( \
		(TDI_GET_NO_DEBUG(field)),, \
		if constexpr (static_cast<_underlying>(_type::TDI_GET_NAME(field)) == 0) { \
			bool selected = static_cast<_underlying>(value) == 0; \
			if (ImGui::Selectable(TD_DEFER(TD_STRINGIFY)(TDI_GET_NAME(field)), selected) && !selected) { \
				value = _type::TDI_GET_NAME(field); \
				changed = true; \
			} \
		} else { \
			bool selected = (value & _type::TDI_GET_NAME(field)) != static_cast<_type>(0); \
			if (ImGui::Selectable(TD_DEFER(TD_STRINGIFY)(TDI_GET_NAME(field)), &selected)) { \
				if (selected) { \
					value |= _type::TDI_GET_NAME(field); \
				} else { \
					value &= ~_type::TDI_GET_NAME(field); \
				} \
				changed = true; \
			} \
		} \
	)
#define TDI_ENUM_DEBUG_FLAGS(fields) TD_APPLY_EACH_CONCAT(TDI_ENUM_DEBUG_FLAGS_FIELD, TD_UNWRAP fields)

#define TDI_ENUM_DEBUG_SELECT_FIELD(field) \
	TD_IF_ELSE( \
		(TDI_GET_NO_DEBUG(field)),, \
		{ \
			bool selected = value == _type::TDI_GET_NAME(field); \
			if (ImGui::Selectable(TD_DEFER(TD_STRINGIFY)(TDI_GET_NAME(field)), selected) && !selected) { \
				value = _type::TDI_GET_NAME(field); \
			} \
		} \
	)
#define TDI_ENUM_DEBUG_SELECT(fields) TD_APPLY_EACH_CONCAT(TDI_ENUM_DEBUG_SELECT_FIELD, TD_UNWRAP fields)

#define TDI_ENUM_DEBUG(meta, fields) \
	namespace TwoD { \
		template<> \
		struct Debuggable<TDI_GET_QUALIFIED_NAME(meta)> { \
			static bool Draw(TDI_GET_QUALIFIED_NAME(meta)& value, const char* name) { \
				using _type = TDI_GET_QUALIFIED_NAME(meta); \
				using _underlying = std::underlying_type_t<_type>; \
				using _unsigned = std::make_unsigned_t<_underlying>; \
				bool changed = false; \
				if constexpr (TDI_ENUM_DEBUG_IS_FLAGS(fields)) { \
					TDI_ENUM_DEBUG_FLAGS(fields) \
				} else { \
					TDI_ENUM_DEBUG_SELECT(fields) \
				} \
				return changed; \
			} \
		}; \
	}
#else
#define TDI_ENUM_DEBUG(meta, fields)
#endif

// enum builder
#define TDI_ENUM_BUILD(meta, fields) \
	TDI_GET_NAMESPACE_START(meta) \
		TDI_ENUM_DEF(meta, fields) \
		TDI_ENUM_BIT_OPERATOR(meta, |) \
		TDI_ENUM_BOOL_OPERATOR(meta, !=) \
		TDI_ENUM_BIT_OPERATOR(meta, &) \
		TDI_ENUM_BIT_OPERATOR(meta, ^) \
		TDI_ENUM_BOOL_OPERATOR(meta, ==) \
		TDI_ENUM_BIT_UNARY_OPERATOR(meta, ~) \
	TDI_GET_NAMESPACE_CLOSE(meta) \
	TDI_ENUM_DESERIALIZABLE(meta, fields) \
	TDI_ENUM_DEBUG(meta, fields)


// "exposed" macros
#define TD_ENUM_FIELD(name, ...) TDI_DATA_COMBINE(TDI_EMPTY_ENUM_FIELDDATA, TD_NAME(name) __VA_OPT__(, __VA_ARGS__))
#define TD_ENUM(meta, fields) TD_EXPAND(TDI_ENUM_BUILD( \
		TDI_DATA_COMBINE(TDI_EMPTY_ENUM_METADATA, TD_UNWRAP meta), \
		fields \
	))


//TD_ENUM(
//	(TD_NAME(Foo), TD_NAMESPACE(Bar), TD_BASE(uint8_t)),
//	(
//		TD_ENUM_FIELD(ONE, TD_INIT(8)),
//		TD_ENUM_FIELD(TWO, TD_INIT(10), TD_NO_SERIALIZE),
//		TD_ENUM_FIELD(FOUR)
//	)
//)