#!/usr/bin/python3
# -*- coding: utf-8 -*-
import sys
"""

"""

if __name__ == "__main__":
    if (argv != 3):
        sys.stderr.write("Wrong arguments!\n")
        sys.exit()
    
    header_filename = argv[1]
    with open(header_filename, "w") as fo:
        fo.write("")

