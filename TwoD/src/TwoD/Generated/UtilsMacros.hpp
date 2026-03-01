#pragma once
#define TD_UNWRAP(...) __VA_ARGS__
#define TD_EXPAND(x) x
#define TD_APPLY(F, ...) F(__VA_ARGS__)
#define TD_TRASH(...)
#define TD_EMPTY()

#define TD_STRINGIFY(x) #x
#define TD_CONCAT(x, y) x y
#define TD_CONCAT_TOKEN(x, y) x ## y

#define TD_EVAL(...)  TD_INTERNAL_EVAL1(TD_INTERNAL_EVAL1(TD_INTERNAL_EVAL1(__VA_ARGS__)))
#define TD_INTERNAL_EVAL1(...) TD_INTERNAL_EVAL2(TD_INTERNAL_EVAL2(TD_INTERNAL_EVAL2(__VA_ARGS__)))
#define TD_INTERNAL_EVAL2(...) __VA_ARGS__
