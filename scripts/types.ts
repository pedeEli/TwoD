type OptionArgString = { readonly type: 'string'; }
type OptionArgNumber = { readonly type: 'number'; }
type OptionArgArray = {
	readonly type: 'array';
	readonly arr: OptionArgString | OptionArgNumber;
}
type OptionArg = OptionArgString | OptionArgNumber | OptionArgArray


type Option ={
	readonly name: string;
	readonly args?: OptionArg
	readonly shortName?:	string;
	readonly description?: string;
	readonly optional?: boolean;
	readonly alternative?: AlternativeOption;
}
type AlternativeOption = Omit<Option, 'alternative' | 'optional'>


type CommandArg = {
	readonly type: 'string' | 'number';
	readonly name: string;
	readonly description: string;
}


type Command = {
	readonly description: string;
	readonly args?: readonly CommandArg[];
	readonly options?: readonly Option[];
}

type ParseOptionArg<A extends OptionArg> =
	A extends OptionArgString ? string :
	A extends OptionArgNumber ? number :
	A extends OptionArgArray
		? A["arr"] extends OptionArgString ? string[] : number[]
		: never

type ParseCommandOption3<O extends Option> =
	O["args"] extends OptionArg
		? { args: ParseOptionArg<NonNullable<O["args"]>> }
		: {}
type ParseCommandOption2<O extends Option> =
	O["alternative"] extends AlternativeOption
		? ParseCommandOption3<O> | { alternative: ParseCommandOption3<NonNullable<O["alternative"]>>}
		: ParseCommandOption3<O>
type ParseCommandOption<O extends Option> =
	O["optional"] extends true
		? { [K in O["name"]]?: ParseCommandOption2<O> }
		: { [K in O["name"]]: ParseCommandOption2<O> }

type ParseCommandOptions2<OS extends readonly Option[]> =
	OS extends readonly [infer O extends Option, ...infer Rest extends readonly Option[]]
		? ParseCommandOption<O> & ParseCommandOptions2<Rest>
		: {}

type ParseCommandOptions<OS extends readonly Option[]> = {
	options: ParseCommandOptions2<OS>
}
type ParseCommandArgs<AS extends readonly CommandArg[]> = {
	args: {
		[K in AS[number] as K["name"]]: K["type"] extends "string" ? string : number;
	}
}

type ParseCommand<C extends Command> =
	(undefined extends C["args"] ? {} : ParseCommandArgs<NonNullable<C["args"]>>) &
	(undefined extends C["options"] ? {} : ParseCommandOptions<NonNullable<C["options"]>>)


type CreateOptions = <const Options extends readonly Option[]>(options: Options) => Options
type CreateCommands = <const Commands extends Record<string, Command>>(commands: Commands) => Commands

type GetCommand = <const Commands extends Record<string, Command>>(commands: Commands) => keyof Commands
type ParseArguments = <const C extends Command>(name: string, command: C) => ParseCommand<C>


type Macros = {
	comma: string;
	unwrap: string;
	expand: string;
	apply: string;
	trash: string;
	empty: string;
	stringify: string;
	concat: string;
	concatToken: string;
	eval: string;
	parenthesis: string;
	ifElse: string;
	applyIf: string;
	
	defer: string;
	deferIndirect: string,
	deferN: (n: number) => string;
	
	getN: (n: number) => string;
	setN: (n: number) => string;
	
	applyEach: string;
	applyEachIndirect: string;
	applyEachConcat: string;
	applyEachConcatIndirect: string;

	zipWith: string;
	zipWithIndirect: string;

	foldWith: string;
	foldWithIndirect: string;

	choose: string;
}

type FileGenerator = (
	out: import('fs').WriteStream,
	iterations: number,
	utils: Macros,
	internal: (value: string) => string
) => void