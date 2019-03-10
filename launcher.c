#include <Windows.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	// prologue already happens so
	// argc == ebp + 0x8
	// argv == ebp + 0xC
	__asm {
		mov ebx, [ebp + 0xC]	// Save argv into ebx
		cmp [ebp + 0x8], 2		// Check if there is a filename argument (argc == 2) 
		jne $ + 0x5B
		push 0x6272				// "rb"
		push esp				// pointer to "rb"
		push [ebx + 4]			// argv[1]
		call fopen				// fopen(argv[1], "rb")
		mov esi, eax			// esi = FILE
		push 0x2				// SEEK_END
		push 0					// 0
		push esi				// FILE
		call fseek				// fseek(FILE, 0, SEEK_END)
		call ftell				// ftell(FILE)
		mov edi, eax
		call rewind				// rewind(FILE)
		push edi				// length of FILE
		push 1
		call calloc				// calloc(LENGTH, 1)
		push esi				// FILE
		push 1
		push edi				// length of FILE
		push eax				// alloc'd mem
		call fread				// fread(MEM, length, 1, FILE)
		push ebx				// argv (trash pointer)
		push 0x40
		push edi				// length
		push [esp + 0xC]		// MEM
		call VirtualProtect		// VirtualProtect(MEM, length, 0x40, argv)
		int 3
		call [esp]				// Call shellcode
		int 3
	};
}
