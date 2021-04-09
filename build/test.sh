rm -rf section.md symbol_a.md symbol_a_b.md symbol_a_b_ab.md relocat.md
./Visual-linker -f a.o,b.o,ab --S --print section.md
./Visual-linker -f a.o --s --print symbol_a.md
./Visual-linker -f a.o,b.o --s --print symbol_a_b.md
./Visual-linker -f a.o,b.o,ab --s --print symbol_a_b_ab.md
./Visual-linker -f a.o,b.o,ab --R --print relocat.md