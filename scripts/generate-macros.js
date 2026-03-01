import fs from 'fs'
import path from 'path'
import {createCommands, createOptions, getCommand, parseArguments} from './parse-args.js'

const generateOptions = createOptions([
	{
		name: '--prefix',
		shortName: '-p',
		args: {
			type: 'string'
		},
		description: 'prefix used for every macro',
		alternative: {
			name: '--no-prefix'
		}
	},
	{
		name: '--internal-prefix',
		args: {
			type: 'string'
		},
		description: 'prefix used for macros that are not meant for outside use (default <prefix>INTERNAL)',
		optional: true
	},
	{
		name: '--files',
		args: {
			type: 'array',
			arr: {
				type: 'string'
			}
		},
		description: 'only specified files are generated (run list to get all generated files)',
		optional: true
	}
]);
const commands = createCommands({
	generate: {
		description: 'generate all files. to get a list of all the generated files run `generate-macros.js files`',
		args: [
			{
				type: 'string',
				name: 'dir',
				description: 'output directory'
			},
			{
				type: 'number',
				name: 'iterations',
				description: 'how many iterations are supported in the generated macros'
			}
		],
		options: generateOptions
	},
	files: {
		description: 'lists all generated files'
	}
})


/** @type {FileGenerator} */
function utils(out, iterations, macros, internal) {
	out.write("#pragma once\n")
	out.write(`#define ${macros.unwrap}(...) __VA_ARGS__\n`)
	out.write(`#define ${macros.expand}(x) x\n`)
	out.write(`#define ${macros.apply}(F, ...) F(__VA_ARGS__)\n`)
	out.write(`#define ${macros.trash}(...)\n`)
	out.write(`#define ${macros.empty}()\n`)
	out.write('\n')

	out.write(`#define ${macros.stringify}(x) #x\n`)
	out.write(`#define ${macros.concat}(x, y) x y\n`)
	out.write(`#define ${macros.concatToken}(x, y) x ## y\n`)
	out.write('\n')

	const eval1 = internal('EVAL1')
	const eval2 = internal('EVAL2')
	out.write(`#define ${macros.eval}(...)  ${eval1}(${eval1}(${eval1}(__VA_ARGS__)))\n`)
	out.write(`#define ${eval1}(...) ${eval2}(${eval2}(${eval2}(__VA_ARGS__)))\n`)
	out.write(`#define ${eval2}(...) __VA_ARGS__\n`)
}

/** @type {FileGenerator} */
function applyEach(out, iterations, macros, internal) {
	out.write('#pragma once\n')
	out.write('#include "UtilsMacros.hpp"\n')

	// generate APPLY_EACH_i
	for (let i = 1; i <= iterations; i++) {
		const arr = Array(i).fill(null).map((_, j) => j + 1)
		let str = '#define '
		// macro header
		str += internal(`APPLY_EACH_${i}`)
		str += `(F, ${arr.map(j => `X${j}`).join(', ')}) `
		// macro body
		str += `${arr.map(j => `F(X${j})`).join(' ')}\n`
		out.write(str)
	}

	// generate APPLY_EACH_N
	const applyEachN = internal('APPLY_EACH_N')
	{
		const applyEachNParams = Array(iterations).fill(null).map((_, j) => `_${j + 1}`).join(', ')
		let str = '#define '
		str += applyEachN
		str += `(${applyEachNParams}, macro, ...) macro\n`
		out.write(str)
	}

	// generate APPLY_EACH
	{
		const PER_ROW = 4
		let str = '#define '
		// macro header
		str += macros.applyEach
		str += '(F, ...) '
		// macro body
		str += `${applyEachN}( \\\n`
		str += '\t__VA_ARGS__, \\\n'
		for (let i = iterations; i > 0; i -= PER_ROW) {
			const arr = Array(Math.min(PER_ROW, i)).fill(null).map((_, j) => i - j);
			str += `\t${arr.map(j => internal(`APPLY_EACH_${j}`)).join(', ')}`
			str += `${i - PER_ROW <= 0 ? '' : ','} \\\n`
		}
		str += ')(F, __VA_ARGS__)\n'
		out.write(str)
	}
	// generate APPLY_EACH_INDIRECT
	out.write(`#define ${macros.applyEachIndirect}() ${macros.applyEach}\n`)
}

/** @type {FileGenerator} */
function zipWith(out, iterations, macros, internal) {
	out.write('#pragma once\n')
	out.write('#include "UtilsMacros.hpp"\n')
	out.write('#include "DeferMacros.hpp"\n')

	// generate ZIP_WITH_i
	for (let i = 1; i <= iterations; i++) {
		const arr = Array(i).fill(null).map((_, j) => j + 1)
		let str = '#define '
		// macro header
		str += internal(`ZIP_WITH_${i}`)
		str += `(F, ${arr.map(j => `X${j}`).join(', ')}, ${arr.map(j => `Y${j}`).join(', ')}) `
		// macro body
		str += `(${arr.map(j => `F(X${j}, Y${j})`).join(', ')})\n`
		out.write(str)
	}

	// generate ZIP_WITH_N
	const zipWithN = internal('ZIP_WITH_N')
	{
		const zipWithNParams = Array(iterations).fill(null).map((_, j) => `_${j + 1}`).join(', ')
		let str = '#define '
		str += zipWithN
		str += `(${zipWithNParams}, macro, ...) macro\n`
		out.write(str)
	}

	// generate ZIP_WITH
	{
		const PER_ROW = 4
		let str = '#define '
		// macro header
		str += macros.zipWith
		str += '(F, xs, ys) '
		// macro body
		str += `${macros.defer}(${zipWithN})( \\\n`
		str += `\t${macros.unwrap} xs, \\\n`
		for (let i = iterations; i > 0; i -= PER_ROW) {
			const arr = Array(Math.min(PER_ROW, i)).fill(null).map((_, j) => i - j);
			str += `\t${arr.map(j => internal(`ZIP_WITH_${j}`)).join(', ')}`
			str += `${i - PER_ROW <= 0 ? '' : ','} \\\n`
		}
		str += `)(F, ${macros.unwrap} xs, ${macros.unwrap} ys)\n`
		out.write(str)
	}
	// generate ZIP_WITH_INDIRECT
	out.write(`#define ${macros.zipWithIndirect}() ${macros.zipWith}\n`)
}

/** @type {FileGenerator} */
function defer(out, iterations, macros, internal) {
	out.write("#pragma once\n")
	out.write(`#define ${macros.defer}(x) x ${macros.empty}()\n`)
	out.write(`#define ${macros.deferIndirect}(x) ${macros.defer}\n`)
	for (let i = 0; i < iterations; i++) {
		let str = '#define '
		str += `${macros.deferN(i + 1)}(x) ${macros.defer}`
		str += Array(i).fill(null).map(() => `(${macros.deferIndirect})()`).join('')
		str += '(x)\n'
		out.write(str)
	}
}

/** @type {FileGenerator} */
function getterSetter(out, iterations, macros, internal) {
	out.write("#pragma once\n")
	for (let i = 0; i < iterations; i++) {
		let str = '#define '
		str += `${macros.getN(i + 1)}(`
		str += Array(i).fill(null).map((_, j) => `_${j + 1}, `).join('')
		str += 'x, ...) x\n'
		out.write(str)
	}
	out.write('\n')

	for (let i = 0; i < iterations; i++) {
		let str = '#define '
		str += `${macros.setN(i + 1)}(x, `
		str += Array(i + 1).fill(null).map((_, j) => `_${j + 1}, `).join('')
		str += '...) ('
		str += Array(i).fill(null).map((_, j) => `_${j + 1}, `).join('')
		str += 'x, __VA_ARGS__)\n'
		out.write(str)
	}
}

/** @type {FileGenerator} */
function foldWith(out, iterations, macros, internal) {
	out.write('#pragma once\n')
	out.write('#include "DeferMacros.hpp"\n')
	
	// generate FOLD_WITH_i
	let body = 'F(X1, X2)'
	for (let i = 2; i <= iterations; i++) {
		let str = '#define '
		// macro header
		str += internal(`FOLD_WITH_${i}`)
		str += `(F, ${Array(i).fill(null).map((_, j) => `X${j + 1}`).join(', ')}) `
		// macro body
		str += body
		out.write(str + '\n')
		body = `${macros.deferN(i - 1)}(F)(` + body + `, X${i + 1})`
	}

	// generate FOLD_WITH_N
	const foldWithN = internal('FOLD_WITH_N')
	{
		const foldWithNParams = Array(iterations - 1).fill(null).map((_, j) => `_${j + 2}`).join(', ')
		let str = '#define '
		str += foldWithN
		str += `(${foldWithNParams}, macro, ...) macro\n`
		out.write(str)
	}

	// generate FOLD_WITH
	{
		const PER_ROW = 4
		let str = '#define '
		// macro header
		str += macros.foldWith
		str += '(F, DEFAULT, ...) '
		// macro body
		str += `${foldWithN}( \\\n`
		str += '\t__VA_ARGS__, \\\n'
		for (let i = iterations; i > 1; i -= PER_ROW) {
			const arr = Array(Math.min(PER_ROW, i - 1)).fill(null).map((_, j) => i - j);
			str += `\t${arr.map(j => internal(`FOLD_WITH_${j}`)).join(', ')}`
			str += `${i - PER_ROW <= 1 ? '' : ','} \\\n`
		}
		str += ')(F, DEFAULT, __VA_ARGS__)\n'
		out.write(str)
	}
	// generate FOLD_WITH_INDIRECT
	out.write(`#define ${macros.foldWithIndirect}() ${macros.foldWith}\n`)
}

/** @type {FileGenerator} */
function choose(out, iterations, macros, internal) {
	out.write('#pragma once\n')
	out.write('#include "FoldWithMacros.hpp"\n')

	// generate CHOOSE_i
	for (let i = 2; i <= iterations; i++) {
		let str = '#define '
		// macro header
		str += internal(`CHOOSE_${i}`)
		str += `(${Array(i).fill(null).map((_, j) => `_${j + 1}`).join(', ')}, macro, ...) `
		// macro body
		str += 'macro'
		out.write(str + '\n')
	}

	// generate CHOOSE_N_SELECT
	const chooseNSelect = internal('CHOOSE_N_SELECT')
	{
		let str = '#define '
		str += chooseNSelect
		str += `(${Array(iterations).fill(null).map((_, j) => `_${j + 1}`).join(', ')}, macro, ...) macro\n`
		out.write(str)
	}

	// generate CHOOSE_N
	const chooseN = internal('CHOOSE_N')
	{
		const PER_ROW = 4
		let str = '#define '
		// macro header
		str += `${chooseN}(...) `
		// macro body
		str += `${chooseNSelect}( \\\n`
		str += '\t__VA_ARGS__, \\\n'
		for (let i = iterations; i > 1; i -= PER_ROW) {
			const arr = Array(Math.min(PER_ROW, i - 1)).fill(null).map((_, j) => i - j);
			str += `\t${arr.map(j => internal(`CHOOSE_${j}`)).join(', ')}`
			str += `${i - PER_ROW <= 1 ? '' : ','} \\\n`
		}
		str += ')\n'
		out.write(str)
	}

	// generate CHOOSE
	out.write(`#define ${macros.choose}(macros, ...) ${macros.defer}(${chooseN}(${macros.unwrap} macros))(__VA_ARGS__, ${macros.unwrap} macros)(__VA_ARGS__)\n`)
}

/** @type {FileGenerator} */
function helperMacros(out, iterations, macros, internal) {
	out.write("#pragma once\n")
	for (const FILE of FILES) {
		if (FILE.name != 'helper-macros')
		out.write(`#include "${FILE.fullname}"\n`)
	}
}

/**
 * @param {(value: string) => string} prefix
 * @returns {Macros}
 */
function createMacroNames(prefix) {
	return {
		unwrap: prefix('UNWRAP'),
		expand: prefix('EXPAND'),
		apply: prefix('APPLY'),
		trash: prefix('TRASH'),
		empty: prefix('EMPTY'),
		stringify: prefix('STRINGIFY'),
		concat: prefix('CONCAT'),
		concatToken: prefix('CONCAT_TOKEN'),
		eval: prefix('EVAL'),
		
		defer: prefix('DEFER'),
		deferIndirect: prefix('DEFER_INDIRECT'),
		deferN: n => prefix(`DEFER${n}`),

		getN: n => prefix(`GET${n}`),
		setN: n => prefix(`SET${n}`),

		applyEach: prefix('APPLY_EACH'),
		applyEachIndirect: prefix('APPLY_EACH_INDIRECT'),

		zipWith: prefix('ZIP_WITH'),
		zipWithIndirect: prefix('ZIP_WITH_INDIRECT'),
		
		foldWith: prefix('FOLD_WITH'),
		foldWithIndirect: prefix('FOLD_WITH_INDIRECT'),
		
		choose: prefix('CHOOSE'),
	}
}

/** @type {Array<{
 * 		name: string;
 * 		fullname: string;
 * 		generator: FileGenerator;
 * }>} */
const FILES = [
	{
		name: "utils",
		fullname: "UtilsMacros.hpp",
		generator: utils
	},
	{
		name: 'apply-each',
		fullname: 'ApplyEachMacros.hpp',
		generator: applyEach
	},
	{
		name: 'zip-with',
		fullname: 'ZipWithMacros.hpp',
		generator: zipWith
	},
	{
		name: 'defer',
		fullname: 'DeferMacros.hpp',
		generator: defer
	},
	{
		name: 'getter-setter',
		fullname: 'GetterSetterMacros.hpp',
		generator: getterSetter
	},
	{
		name: 'helper-macros',
		fullname: 'HelperMacros.hpp',
		generator: helperMacros
	},
	{
		name: 'fold-with',
		fullname: 'FoldWithMacros.hpp',
		generator: foldWith
	},
	{
		name: 'choose',
		fullname: 'ChooseMacros.hpp',
		generator: choose
	}
]


const name = getCommand(commands)
if (name == 'files') {
	const result = parseArguments(name, commands[name]);
	/** @type {[string, string][]} */
	const files = []
	let size = 0
	for (const file of FILES) {
		files.push([file.name, file.fullname])
		size = Math.max(size, file.name.length)
	}
	for (const [name, fullname] of files) {
		console.log(name.padEnd(size + 3, ' ') + fullname)
	}
} else {
	const result = parseArguments(name, commands[name]);

	const dir = path.resolve(result.args.dir)
	try {
		fs.readdirSync(dir)
	} catch {
		fs.mkdirSync(dir, { recursive: true })
	}

	const iterations = result.args.iterations

	const prefix = 'args' in result.options['--prefix'] ? result.options['--prefix'].args : ''
	const internal = result.options['--internal-prefix']?.args ?? prefix + "INTERNAL_"
	
	let files = FILES
	if (result.options['--files']) {
		files = []
		for (const file of result.options['--files'].args) {
			const FILE = FILES.find(({name}) => name == file)
			if (!FILE) {
				console.log(`${file} does not exist`)
				process.exit()
			}
			files.push(FILE)
		}
	}

	/**
	 * @param {string} value
	 * @returns {string}
	 */
	function prefixF(value) {
		return prefix + value
	}

	
	/**
	 * @param {string} value
	 * @returns {string}
	 */
	function internalF(value) {
		return internal + value
	}

	const utils = createMacroNames(prefixF)

	for (const file of files) {
		const out = fs.createWriteStream(path.join(dir, file.fullname), 'utf-8')
		file.generator(out, iterations, utils, internalF)
		out.close()
	}
}