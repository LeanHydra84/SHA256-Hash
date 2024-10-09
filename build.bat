@echo off
echo Building hash.exe:
clang src/main.cpp src/hash.cpp src/fileio.cpp -Wall -I include -o build/bin/hash.exe

echo Building hash.lib:
clang src/hash.cpp -c -Wall -o build/lib/hash.o -I include && llvm-ar rc build/lib/hash.lib build/lib/hash.o