#pragma once
#define TD_COMMA ,
#define TD_UNWRAP(...) __VA_ARGS__
#define TD_EXPAND(x) x
#define TD_APPLY(F, ...) F(__VA_ARGS__)
#define TD_TRASH(...)
#define TD_EMPTY()
#define TD_PARENTHESIS(...) ()

#define TDI_IF_ELSE(i, e, ...) TD_EXPAND(TD_UNWRAP __VA_OPT__(TD_PARENTHESIS)(e)) __VA_OPT__(i)
#define TD_IF_ELSE(con, i, e) TDI_IF_ELSE(i, e, TD_UNWRAP con)
#define TD_APPLY_IF(f, x) TD_IF_ELSE(x, f, TD_EMPTY) x

#define TD_STRINGIFY(x) #x
#define TD_CONCAT(x, y) x y
#define TD_CONCAT_TOKEN(x, y) x ## y

#define TD_EVAL(...)  TDI_EVAL1(TDI_EVAL1(TDI_EVAL1(__VA_ARGS__)))
#define TDI_EVAL1(...) TDI_EVAL2(TDI_EVAL2(TDI_EVAL2(__VA_ARGS__)))
#define TDI_EVAL2(...) __VA_ARGS__
