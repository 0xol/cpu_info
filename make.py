import subprocess
import platform
import random
import string

#currently does not support cross compilation
#could just drop any form of mac os x support

CC = "g++"
CCFLAGS = " -O2"

def system(command):
    subprocess.run(str(command), shell=True)

CCFLAGS += ""

def compile_file(file, bin):
    print(CC + CCFLAGS + " " + file + " -o " + bin)
    #system(CC + CCFLAGS + " " + file + " -o " + bin)
    system(f"{CC} -c {file} -o {bin} {CCFLAGS}")

compile_file("src/main.cpp", "main.o")

system(f"{CC} *.o -o cpu.exe -lpthread")