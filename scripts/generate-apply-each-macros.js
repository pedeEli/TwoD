import fs from 'fs'
import path from 'path'
import {Console} from 'console'

const PER_ROW = 4

const nStr = process.argv.at(2)
if (nStr == undefined) {
	console.log('Usage: node', process.argv[1], '<number>')
	process.exit()
}
const n = parseInt(nStr)
if (isNaN(n)) {
	console.log('Usage:', nStr, 'is not a number')
	process.exit()
}

const out = fs.createWriteStream(path.resolve('./TwoD/src/TwoD/Core/ApplyEachMacro.hpp'))
const logger = new Console({stdout: out})

// generate TD_INTERNAL_APPLY_EACH_N
for (let i = 1; i <= n; i++) {
	const arr = Array(i).fill(null).map((_, j) => j + 1)
	logger.log(`#define TD_INTERNAL_APPLY_EACH_${i}(F, ${arr.map(j => `X${j}`).join(', ')}) ${arr.map(j => `F(X${j})`).join(' ')}`)
}

logger.log(`#define TD_INTERNAL_GET_EACH_MACRO(${Array(n).fill(null).map((_, j) => `_${j + 1}`).join(', ')}, macro, ...) macro`)
logger.log('#define TD_APPLY_EACH(F, ...) TD_EXPAND_MACRO(TD_INTERNAL_GET_EACH_MACRO( \\')
logger.log('\t__VA_ARGS__, \\')
for (let i = n; i > 0; i -= PER_ROW) {
	const arr = Array(Math.min(PER_ROW, i)).fill(null).map((_, j) => i - j);
	logger.log(`\t${arr.map(j => `TD_INTERNAL_APPLY_EACH_${j}`).join(', ')}${i - PER_ROW <= 0 ? '' : ','} \\`)
}
logger.log(')(F, __VA_ARGS__))')

/*
#define TD_INTERNAL_APPLY_EACH_1(F, X1) F(X1)
#define TD_INTERNAL_GET_EACH_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, MACRO, ...) MACRO
#define TD_APPLY_EACH(F, ...) TD_EXPAND_MACRO(TD_INTERNAL_GET_EACH_MACRO( \
	__VA_ARGS__, \
	TD_INTERNAL_APPLY_EACH_18, TD_INTERNAL_APPLY_EACH_17, \
	TD_INTERNAL_APPLY_EACH_16, TD_INTERNAL_APPLY_EACH_15, \
	TD_INTERNAL_APPLY_EACH_14, TD_INTERNAL_APPLY_EACH_13, \
	TD_INTERNAL_APPLY_EACH_12, TD_INTERNAL_APPLY_EACH_11, \
	TD_INTERNAL_APPLY_EACH_10, TD_INTERNAL_APPLY_EACH_9, \
	TD_INTERNAL_APPLY_EACH_8, TD_INTERNAL_APPLY_EACH_7, \
	TD_INTERNAL_APPLY_EACH_6, TD_INTERNAL_APPLY_EACH_5, \
	TD_INTERNAL_APPLY_EACH_4, TD_INTERNAL_APPLY_EACH_3, \
	TD_INTERNAL_APPLY_EACH_2, TD_INTERNAL_APPLY_EACH_1 \
)(F, __VA_ARGS__))
*/