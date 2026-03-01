/** @import("./types") */

/**
 * @param {Record<string, Command>} commands
 */
function printUsage(commands) {
	console.error('commands:')
	for (const name in commands) {
		let str = `\t${name}`
		if (commands[name].args) {
			for (const arg of commands[name].args) {
				str += ` <${arg.name}>`
			}
		}
		if (commands[name].options) {
			str += ` [<options>]`
		}
		console.log(str)	
	}
}

/**
 * @param {string} name
 * @param {Command} command
 */
function printCommandUsage(name, command) {
	console.log(command.description)
	let str = `usage: ${name}`
	if (command.args) {
		for (const arg of command.args) {
			str += ` <${arg.name}>`
		}
	}
	if (command.options) {
		str += ` [<options>]`
	}
	console.log(str)
	if (command.args) {
		/** @type {[string, string][]} */
		const args = []
		/** @type {number} */
		let size = 0
		for (const arg of command.args) {
			args.push([arg.name + ':', arg.description]);
			size = Math.max(size, arg.name.length);
		}
		for (const [name, description] of args) {
			console.log(`   ${name.padEnd(size + 4, ' ')}${description}`)
		}
	}
	if (command.options) {
		console.log('options:')
		/** @type {[string, string][]} */
		const options = []
		/** @type {number} */
		let size = 0
		/** @param {Option} option */
		function add(option) {
			let name = option.name
			if (option.description) {
				name += ':'
			}
			options.push([option.name, option.description ?? ''])
			size = Math.max(size, option.name.length)
			if (option.alternative) {
				add(option.alternative)
			}
		}
		for (const option of command.options) {
			add(option)
		}
		for (const [name, description] of options) {
			console.log(`   ${name.padEnd(size + 4, ' ')}${description}`)
		}
	}
}

/** @type {GetCommand} */
export function getCommand(commands) {
	const argStr = process.argv[2]
	for (const name in commands) {
		if (name == argStr) {
			return name;
		}
	}
	printUsage(commands)
	process.exit()
}

/**
 * @param {string} name
 * @param {Option} option
 * @returns {boolean}
 */
function optionNameEqual(name, option) {
	return option.name == name || (!!option.shortName && option.shortName == name)
}
/**
 * @param {string} arg
 * @returns {false | number}
 */
function parseIntArg(arg) {
	if (!/^[1-9][0-9]*$/.test(arg)) {
		return false
	}
	return parseInt(arg)
}

/** @type {ParseArguments} */
export function parseArguments(name, command) {
	/** @type {Partial<ParseCommand<Required<Command>>>} */
	const result = {}
	
	let index = 3
	if (command.args) {
		result.args = {}
		for (const arg of command.args) {
			if (index == process.argv.length) {
				printCommandUsage(name, command)
				process.exit()
			}
			const argStr = process.argv[index]
			if (arg.type == "string") {
				result.args[arg.name] = /** @type {any} */(argStr)
			} else {
				const a = parseIntArg(argStr)
				if (a == false) {
					printCommandUsage(name, command)
					process.exit()
				}
				result.args[arg.name] = a
			}
			index++
		}
	}

	if (command.options) {
		/** @type {ParseCommandOption<Required<Option>>} */
		const resultOptions = {}
		result.options = resultOptions
		/** @type {number[]} */
		const nonOptionals = []
		for (let i = 0; i < command.options.length; i++) {
			if (!command.options[i].optional) {
				nonOptionals.push(i)
			}
		}

		while (index < process.argv.length) {
			const name = process.argv[index]
			let option = command.options.find(option =>
				optionNameEqual(name, option) ||
				(option.alternative && optionNameEqual(name, option.alternative))
			)
			if (!option || option.name in resultOptions) {
				printCommandUsage(name, command)
				process.exit()
			}

			/** @type {{alternative?: { args?: any }, args?: any }} */
			let resultOption = {}
			resultOptions[option.name] = /** @type {any} */(resultOption)

			if (!optionNameEqual(name, option)) {
				option = /** @type {Option} */(option.alternative)
				/** @type {{ args?: any }} */
				const alternativeOption = {}
				resultOption.alternative = alternativeOption
				resultOption = alternativeOption
			}
			index++
			if (option.args) {
				if (index == process.argv.length) {
					printCommandUsage(name, command)
					process.exit()
				}
				if (option.args.type == 'string') {
					resultOption.args = process.argv[index]
					index++
				} else if (option.args.type == 'number') {
					const a = parseIntArg(process.argv[index])
					if (a == false) {
						printCommandUsage(name, command)
						process.exit()
					}
					resultOption.args = a
					index++
				} else {
					/** @type {Array<string | number>} */
					const args = []
					while (index < process.argv.length) {
						const arg = process.argv[index]
						if (arg.startsWith('-')) {
							break
						}
						if (option.args.arr.type == 'string') {
							args.push(arg)
						} else {
							const a = parseIntArg(arg)
							if (a == false) {
								printCommandUsage(name, command)
								process.exit()
							}
							args.push(a)
						}
						index++
					}
					if (args.length == 0) {
						printCommandUsage(name, command)
						process.exit()
					}
					resultOption.args = args
				}
			}
		}

		for (const i of nonOptionals) {
			if (!(command.options[i].name in resultOptions)) {
				printCommandUsage(name, command)
				process.exit()
			}
		}
	}

	return /** @type {any} */(result)
}

/** @type {CreateOptions} */
export const createOptions = x => x

/** @type {CreateCommands} */
export const createCommands = x => x