#!/usr/bin/python3
# -*- coding: utf-8 -*-
import sys

HEADER_TEMPLATE = "templates/stack.h"
SOURCE_TEMPLATE = "templates/stack.c"

def write_header(template, filename, prefix, name, T):
    return template.format(filename=filename.replace(".", "_").upper(), 
                           prefix=prefix,
                           name=name,
                           T=T)

def write_source(template, filename, prefix, name, T):
    return template.format(header=filename, 
                           prefix=prefix,
                           name=name,
                           T=T)
if __name__ == "__main__":
    if (len(sys.argv) != 6):
        sys.stderr.write("Wrong arguments!\n")
        sys.exit()
    
    header_filename = sys.argv[1]
    source_filename = sys.argv[2]
    prefix = sys.argv[3]
    name = sys.argv[4]
    T = sys.argv[5]

    header_template = ""
    source_template = ""
    with open(HEADER_TEMPLATE, "r") as fi:
        header_template = fi.read()
    with open(SOURCE_TEMPLATE, "r") as fi:
        source_template = fi.read()

    with open(header_filename, "w") as fo:
        fo.write(write_header(
            header_template,
            header_filename,
            prefix,
            name,
            T
            ))

    with open(source_filename, "w") as fo:
        fo.write(write_source(
            source_template,
            header_filename,
            prefix,
            name,
            T
            ))

