rm -rf *.map
rm -rf  section_ld.md symbol_a_b_ld.md relocat_ld.md
# ./Visual-linker -f a.o,b.o --S --print section.md
./Visual-linker -f a.o,b.o  --S -T test.lds -o ab --print section_ld.md

# ./Visual-linker -f a.o,b.o --s --print symbol_a_b.md
./Visual-linker -f a.o,b.o  --s -T test.lds -o ab --print symbol_a_b_ld.md
# ./Visual-linker -f a.o,b.o --R --print relocat.md
./Visual-linker -f a.o,b.o --R -T test.lds -o ab --print relocat_ld.md
rm -rf *.map
./Visual-linker --W

