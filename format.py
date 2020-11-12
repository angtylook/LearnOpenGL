#! /usr/bin/python3

import subprocess
import os
import sys

def format(file):
    args = ['clang-format', '-style=file', '-i', file]
    subprocess.call(args)

if __name__ == '__main__':
    for arg in sys.argv[1:]:
        if os.path.isfile(arg):
            format(sys.argv[1])
        elif os.path.isdir(arg):
            for folder, _, files in os.walk(arg):
                for src_file in files:
                    src_file = os.path.join(folder, src_file)
                    if src_file.endswith('.h') or src_file.endswith('.cpp') or src_file.endswith('.cc'):
                        print('format file: ' + src_file)
                        format(src_file)
        else:
            print("skip: " + arg)    
