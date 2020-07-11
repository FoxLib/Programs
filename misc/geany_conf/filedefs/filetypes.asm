# For complete documentation of this file, please see Geany's main documentation
[styling]
# Edit these in the colorscheme .conf file instead
default=default
comment=comment_line
commentblock=comment
commentdirective=comment
number=number_1
string=string_1
operator=operator
identifier=identifier_1
cpuinstruction=keyword_1
mathinstruction=keyword_2
register=type
directive=preprocessor
directiveoperand=keyword_3
character=character
stringeol=string_eol
extinstruction=keyword_4

[keywords]
# all items must be in one line
# this is by default a very simple instruction set; not of Intel or so
instructions=add or adc sbb and sub xor cmp push pop daa aaa das aas inc dec jo jno jb jnb jz jnz jbe jnbe js jnz jp jnp jl jnl jle jnle test mov xchg lea nop call cbw cwd cwde cdq callf popf pushf sahf lahf movsb movsw movsd cmpsb cmpsw cmpsd stosb stosw stosd lodsb lodsw lodsd scasb scasw scasd ret les lds lfs lgs retf int into iret aam aad xlat xlatb loopnz loop loopz jcxz in out jmp jmpf hlc cmc clc stc cli sti cld std rol ror rcl rcr shl shr sar sal pusha pushad popa popad bound arpl imul idiv mul div enter leave iretd salc not neg sldt str ltr lldt lidt lgdt sgdt sidt lar lsl wrmst rdmsr cpuid movzx movsx jc jnc je jne rep repz repnz
registers=ax cx dx bx sp bp si di al cl dl bl ah ch dh bh eax ecx edx ebx esp ebp esi edi es cs ss ds fs gs rax rcx rdx rbx rsp rbp rsi rdi
directives=org macro brk db dw dd dq equ word byte dword


[settings]
# default extension used when saving files
extension=asm

# the following characters are these which a "word" can contains, see documentation
#wordchars=_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789

# single comments, like # in this file
comment_single=;
# multiline comments
#comment_open=
#comment_close=

# set to false if a comment character/string should start at column 0 of a line, true uses any
# indentation of the line, e.g. setting to true causes the following on pressing CTRL+d
	#command_example();
# setting to false would generate this
#	command_example();
# This setting works only for single line comments
comment_use_indent=true

# context action command (please see Geany's main documentation for details)
context_action_cmd=

[indentation]
#width=4
# 0 is spaces, 1 is tabs, 2 is tab & spaces
#type=1

[build_settings]
# %f will be replaced by the complete filename
# %e will be replaced by the filename without extension
# (use only one of it at one time)
compiler=nasm "%f"

