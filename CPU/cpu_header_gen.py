#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Output params
HEADER = "cpu_arch.h"
SOURCE = "cpu_arch.c"

# Template files
HEADER_TEMPLATE = "templates/cpu_arch.h"
SOURCE_TEMPLATE = "templates/cpu_arch.c"

# Param indices for tuples
NAME = 0
CODE = 1
ARGS = 2

# Adress modes
ADDR_MODE_CONST = 2
ADDR_MODE_REGISTER = 3
ADDR_MODE_LABEL = 4

# Define commands and registers here
COMMANDS = [
        ("push", 128, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ), )),
        ("pop", 129, ()),
        ("add", 130, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                       ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ) )),
        ("sub", 131, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                       ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ) )),
        ("mul", 132, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                       ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ) )),
        ("div", 133, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                       ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ) )),
        ("sin", 134, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ), )),
        ("cos", 135, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ), )),
        ("sqrt", 136, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ), )),
        ("in", 137, ()),
        ("out", 138, ()),
        ("mov", 139, ( ( ADDR_MODE_REGISTER, ),
                       ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ) )),
        ("jmp", 140, ( ( ADDR_MODE_LABEL, ADDR_MODE_CONST, ADDR_MODE_REGISTER ), )),
        ("je", 141, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_LABEL, ADDR_MODE_CONST, ADDR_MODE_REGISTER ))),
        ("jne", 142, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_LABEL, ADDR_MODE_CONST, ADDR_MODE_REGISTER ))),
        ("ja", 143, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_LABEL, ADDR_MODE_CONST, ADDR_MODE_REGISTER ))),
        ("jae", 144, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_LABEL, ADDR_MODE_CONST, ADDR_MODE_REGISTER ))),
        ("jb", 145, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_LABEL, ADDR_MODE_CONST, ADDR_MODE_REGISTER ))),
        ("jbe", 146, ( ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_CONST, ADDR_MODE_REGISTER ),
                      ( ADDR_MODE_LABEL, ADDR_MODE_CONST, ADDR_MODE_REGISTER ))),
        ("call", 147, ( (ADDR_MODE_LABEL, ADDR_MODE_CONST), ) ),
        ("ret", 148, () )
        ]
REGISTERS = [
        ("ax", 2),
        ("bx", 3),
        ("cx", 4),
        ("dx", 5),
        ("sp", 6)  # Special register that points at the end
                   # of the CPU stack
        ]
ADDR_MODES = [
        ("const", ADDR_MODE_CONST),
        ("register", ADDR_MODE_REGISTER),
        ("label", ADDR_MODE_LABEL) # Only to be used within assembler
        ]
ADDR_FUNCS = {
        ADDR_MODE_CONST: "read_const_arg",
        ADDR_MODE_REGISTER: "read_reg",
        ADDR_MODE_LABEL: "read_label"
        }
ADDR_FUNC_ARGS = {
        ADDR_MODE_CONST: "arg",
        ADDR_MODE_REGISTER: "reg",
        ADDR_MODE_LABEL: "label"
        }

ENUM_TEMPLATE = "    {prefix}{name} = {code}{end}\n" 

def write_enum(prefix, consts, template):
    s = ""
    cnt = len(consts)
    for i in range(cnt):
        c = consts[i]
        s += template.format(
                prefix=prefix,
                name=c[NAME].upper(),
                code=c[CODE],
                end="," if i < cnt - 1 else "")
    
    return s


NAME_DECL_TEMPLATE = "extern const char *{prefix}NAME_{name};\n"

def write_name_decl(prefix, consts, template):
    s = ""
    cnt = len(consts)
    for i in range(cnt):
        c = consts[i]
        s += template.format(
                prefix=prefix,
                name=c[NAME].upper())
    
    return s


NAME_IMPL_TEMPLATE = "const char *{prefix}NAME_{name} = \"{name}\";\n"

def write_name_impl(prefix, consts, template):
    s = ""
    cnt = len(consts)
    for i in range(cnt):
        c = consts[i]
        s += template.format(
                prefix=prefix,
                name=c[NAME].upper())
    
    return s


READER_TEMPLATE = """#define {prefix}READER(_ptr_, _str_, _len_, _code_, _err_code_) \\
{{ \\
{commands}    else \\
    {{ \\
        return _err_code_; \\
    }} \\
}}
"""
CONST_READER_TEMPLATE = """    {if_} (strcasecmp({prefix}NAME_{name}, str) == 0) \\
    {{\\
        *_ptr_ += _len_; \\
        *_code_ = {code}; \\
    }}\\
"""

def make_reader(prefix, consts):
    s = ""
    first = True
    for c in consts:
        s += CONST_READER_TEMPLATE.format(
                if_="if" if first else "else if",
                prefix=prefix,
                name=c[NAME].upper(),
                code=c[CODE])
        first = False

    return READER_TEMPLATE.format(
                prefix=prefix,
                commands=s)

WRITER_TEMPLATE = """#define {prefix}WRITER(_fout_, _code_) \\
{{ \\
    switch (_code_) \\
    {{ \\
{command_writer}    default: \\
        return ZCPU_ERR_WRONG_BINARY; \\
    }} \\
}}
""" 
CONST_WRITER_TEMPLATE = """    case {code}: \\
        fprintf(_fout_, "%s", "{s}"); \\
        break; \\
"""

def make_writer(prefix, consts):
    s = ""
    for c in consts:
        s += CONST_WRITER_TEMPLATE.format(
                code=c[1],
                s=c[0].lower()
                )

    return WRITER_TEMPLATE.format(
            prefix=prefix,
            command_writer=s)

REG_TO_INDEX_TEMPLATE = """#define REG_TO_INDEX(_index_, _var_)\\
{{\\
    switch (_index_) \\
    {{ \\
{cases} \\
        default: \\
            return ZCPU_ERR_UNKNOWN_ADDR_MODE; \\
    }} \\
}}"""
REG_TO_INDEX_CASE_TEMPLATE = """        case {const}:\n
                                            _var_ = """

ARG_ASM_HANDLER_TEMPLATE = """
#define ARG_ASM_HANDLER(_cmd_code_) switch (_cmd_code_) \\
{{ \\
{commands}    default: \\
        return ZCPU_ERR_CMD_NOT_IMPLEMENTED; \\
}}"""
ARG_ASM_HANDLER_CASE_TEMPLATE = """        case {prefix}{name}:\\
{handler} \\
            break; \\
"""
ARG_ASM_HANDLER_CMD_TEMPLATE = """            addr_mode = ADDR_MODE_CONST; \\
            skip_comments(&str_ptr); \\
{handler}            else \\
            {{ \\
                return ZCPU_ERR_WRONG_INPUT; \\
            }} \\
            \\
            switch (addr_mode) \\
            {{\\
                case ADDR_MODE_CONST: \\
                    write_byte(&cur_ptr, addr_mode); \\
                    write_double(&cur_ptr, arg); \\
                    break; \\
                case ADDR_MODE_REGISTER: \\
                    write_byte(&cur_ptr, addr_mode); \\
                    write_byte(&cur_ptr, reg); \\
                    break; \\
                case ADDR_MODE_LABEL: \\
                    write_byte(&cur_ptr, ADDR_MODE_CONST); \\
                    ptrstack_push(&addrstack, (void *)cur_ptr); \\
                    intstack_push(&labelstack, label); \\
                    write_double(&cur_ptr, 0); \\
                    break; \\
                default: \\
                    return ZCPU_ERR_UNKNOWN_ADDR_MODE; \\
            }} \\
{comma}\\
"""
ARG_ASM_HANDLER_IF_TEMPLATE = """            {if_} ({func}(&str_ptr, &{arg}) == ZCPU_OK) \\
            {{ \\
                addr_mode = {addr_mode}; \\
            }} \\
"""
COMMA_CHECK = """            if (check_for_comma(&str_ptr) != ZCPU_OK) \\
            {{ \\
                return ZCPU_ERR_WRONG_INPUT; \\
            }}"""

def write_arg_handler():
    s = ""    
    for c in COMMANDS:
        handler = ""
        for i in range(len(c[ARGS])):
            first = True
            arg = c[ARGS][i]
            addr_mode_ch = ""
            for addr_mode in arg:
                addr_mode_ch += ARG_ASM_HANDLER_IF_TEMPLATE.format(
                        if_="if" if first else "else if",
                        func=ADDR_FUNCS[addr_mode],
                        arg=ADDR_FUNC_ARGS[addr_mode],
                        addr_mode=addr_mode
                        )
                first = False
            comma_check = COMMA_CHECK if i < len(c[ARGS]) - 1 else ""
            handler += ARG_ASM_HANDLER_CMD_TEMPLATE.format(
                    handler=addr_mode_ch,
                    comma=comma_check
                    ) 

        s += ARG_ASM_HANDLER_CASE_TEMPLATE.format(
                prefix="CMD_",
                name=c[NAME].upper(),
                handler=handler
                )

    return ARG_ASM_HANDLER_TEMPLATE.format(
            commands=s
            )

def write_header():
    header_template = ""
    with open(HEADER_TEMPLATE, "r") as fi:
        header_template = fi.read() 

    commands = write_enum("CMD_", COMMANDS, ENUM_TEMPLATE)
    registers = write_enum("REG_", REGISTERS, ENUM_TEMPLATE)
    address_modes = write_enum("ADDR_MODE_", ADDR_MODES, ENUM_TEMPLATE)

    cmd_names = write_name_decl("CMD_", COMMANDS, NAME_DECL_TEMPLATE)
    reg_names = write_name_decl("REG_", REGISTERS, NAME_DECL_TEMPLATE)

    header = header_template.format(
            header=HEADER.replace(".", "_"),
            command_codes=commands,
            register_codes=registers,
            address_modes=address_modes,
            command_names_decl=cmd_names,
            register_names_decl=reg_names,
            cmd_reader=make_reader("CMD_", COMMANDS),
            reg_reader=make_reader("REG_", REGISTERS),
            cmd_writer=make_writer("CMD_", COMMANDS),
            reg_writer=make_writer("REG_", REGISTERS),
            arg_handler=write_arg_handler()
            )
    with open(HEADER, "w") as fo:
        fo.write(header) 


def write_source():
    source_template = ""
    with open(SOURCE_TEMPLATE, "r") as fi:
        source_template = fi.read() 

    cmd_names_impl = write_name_impl("CMD_", COMMANDS, NAME_IMPL_TEMPLATE)
    reg_names_impl = write_name_impl("REG_", REGISTERS, NAME_IMPL_TEMPLATE)
    
    source = source_template.format(
            command_names_impl=cmd_names_impl,
            register_names_impl=reg_names_impl
            )
    with open(SOURCE, "w") as fo:
        fo.write(source) 

if __name__ == "__main__":
    write_header()
    write_source()

