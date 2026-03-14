#pragma once
#include "TwoD/Generated/HelperMacros.hpp"
/*
* Everything is first converted into this data structure.
* This means that the struct metadata, struct fielddata, enum metadata and enum fielddata all share this layout.
* The datatype that is currently represented is put as the first element for debugging.
* Depending on what type of data is represented some tuple fields might remain empty (e.g. template is not used in enum metadata).
*
* Data layout: (datatype, name, namespace, base, template, type, init, noSerialize, noDebug, updater)
* struct meta:			  name  namespace  base  template
* struct field:			  name							   type  init  noSerialize  noDebug
* enum meta:			  name  namespace  base
* enum field:             name                             type  init  noSerialize  noDebug
* component field:        name							   type  init  noSerialize  noDebug  updater
*/


// data

#define TDI_EMPTY_DATA ,,,,,,,,,,
#define TDI_EMPTY_STRUCT_METADATA TD_APPLY(TD_SET1, STRUCT_METADATA, TDI_EMPTY_DATA)
#define TDI_EMPTY_STRUCT_FIELDDATA TD_APPLY(TD_SET1, STRUCT_FIELDDATA, TDI_EMPTY_DATA)
#define TDI_EMPTY_ENUM_METADATA TD_APPLY(TD_SET1, ENUM_METADATA, TDI_EMPTY_DATA)
#define TDI_EMPTY_ENUM_FIELDDATA TD_APPLY(TD_SET1, ENUM_FIELDDATA, TDI_EMPTY_DATA)
#define TDI_EMPTY_COMPONENT_METADATA TD_APPLY(TD_SET1, COMPONENT_METADATA, TDI_EMPTY_DATA)
#define TDI_EMPTY_COMPONENT_FIELDDATA TD_APPLY(TD_SET1, COMPONENT_FIELDDATA, TDI_EMPTY_DATA)


// data creation

#define TD_NAME(name) TD_APPLY(TD_SET2, name, TDI_EMPTY_DATA)
#define TD_NAMESPACE(ns) TD_APPLY(TD_SET3, ns, TDI_EMPTY_DATA)
#define TD_BASE(base) TD_APPLY(TD_SET4, base, TDI_EMPTY_DATA)
#define TD_TEMPLATE(...) TD_APPLY(TD_SET5, (__VA_ARGS__), TDI_EMPTY_DATA)
#define TD_TYPE(...) TD_APPLY(TD_SET6, (__VA_ARGS__), TDI_EMPTY_DATA)
#define TD_INIT(...) TD_APPLY(TD_SET7, (__VA_ARGS__), TDI_EMPTY_DATA)
#define TD_NO_SERIALIZE TD_APPLY(TD_SET8, NO_SERIALIZE, TDI_EMPTY_DATA)
#define TD_NO_DEBUG TD_APPLY(TD_SET9, NO_DEBUG, TDI_EMPTY_DATA)
#define TD_UPDATER(...) TD_APPLY(TD_SET10, (__VA_ARGS__), TDI_EMPTY_DATA)


// data retrieval

#define TDI_GET_NAME(data) TD_GET2 data
#define TDI_GET_NAMESPACE_START_CREATE(ns) namespace ns {
#define TDI_GET_NAMESPACE(data) TD_IF_ELSE((TD_GET3 data), ::, ) TD_GET3 data TD_IF_ELSE((TD_GET3, data), ::, )
#define TDI_GET_NAMESPACE_START(data) TD_APPLY_IF(TDI_GET_NAMESPACE_START_CREATE, (TD_GET3 data))
#define TDI_GET_NAMESPACE_CLOSE(data) TD_IF_ELSE((TD_GET3 data), }, )
#define TDI_GET_BASE(data) TD_GET4 data
#define TDI_GET_TEMPLATE(data) TD_GET5 data
#define TDI_GET_TYPE(data) TD_UNWRAP TD_GET6 data
#define TDI_GET_INIT(data) TD_GET7 data
#define TDI_GET_NO_SERIALIZE(data) TD_GET8 data
#define TDI_GET_NO_DEBUG(data) TD_GET9 data
#define TDI_GET_UPDATER(data) TD_GET10 data


// advanced data retrieval

#define TDI_GET_BASE_DEF(data) TD_IF_ELSE((TDI_GET_BASE(data)), : TDI_GET_BASE(data), )

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

#define TDI_GET_FIELD_DEF(data) TDI_GET_TYPE(data) TDI_GET_NAME(data) TD_IF_ELSE((TDI_GET_INIT(data)), = TD_UNWRAP TDI_GET_INIT(data), );


// data combinator

#define TDI_DATA_CONCAT(x, y) TD_DEFER(TD_ZIP_WITH)(TD_CONCAT_TOKEN, x, y)
#define TDI_DATA_COMBINE(d, ...) TD_EVAL(TD_FOLD_WITH(TDI_DATA_CONCAT, d, __VA_ARGS__))