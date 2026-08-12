all:
	g++ ./Source/Wai.cpp -Wall -Wextra -fsanitize=address -o ../Wai.elf
