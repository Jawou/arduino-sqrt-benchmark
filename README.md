serialmoniture.py is ai generated, it works on linux

main.hex is the compiled binary for arduino uno r3 or ATmega328P since it uses that processor

main.c is the source code of main.hex it calculates how much iterations are possible in 2 seconds with each algorithm which are naive, binary search, newton and QUAKE III

you can upload the hex file to the arduino uno r3 and use the serialmoniture and it should work without modifications

my results are:

NAIVE: 2004

BINARY SEARCH: 29336

NEWTONS: 13338

QUAKE III: 26798

maybe there is a way to physically change crystal clock on the arduino to over clock it 🤔
