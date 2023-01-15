import subprocess
import platform
import random
import string

#currently does not support cross compilation
#could just drop any form of mac os x support

CC = "g++"
CCFLAGS = " -O0"

def system(command):
    subprocess.run(str(command), shell=True)

compiler_ver = ((str(subprocess.check_output(CC + " -dumpversion", shell=True)).split("'")[1]).split("\\")[0]).replace(".", "v")

CCFLAGS += ' -UBUILD_PLATFORM -DBUILD_PLATFORM=\\"' + platform.machine() + "_" + platform.system() + "_" + CC.replace("+", "plus") + "_" + compiler_ver + "_" + (''.join(random.choice(string.ascii_letters) for _ in range(8))) + '\\"'
CCFLAGS += " -pthread"

def compile_file(file, bin):
    print(CC + CCFLAGS + " " + file + " -o " + bin)
    system(CC + CCFLAGS + " " + file + " -o " + bin)

compile_file("src/main.cpp", "cpu.exe")