# shellcode_launcher
A shellcode launcher written in assembly. Executes shellcode saved in a file.
Its written in a pseudo-PIC style assembly, and saved as inline assembly for easy compiling.

Note, there are `int 3` instructions in the launcher. This was written with the intent of debugging malware / malicious shellcode samples. If you want to include it in a project to actually launch and execute shellcode you might want to remove the `int 3` intsructions.

## Usage

    ./launcher.exe <shellcode_file>

## C Equivalent (for x86 OR x64)

    int main(int argc, char **argv)
    {
        if (argc != 2) { return 1; }
        FILE *fdShell = fopen(argv[1], "rb");
        fseek(fdShell, 0, SEEK_END);
        int length = ftell(fdShell);
        rewind(fdShell);
        void *sc = calloc(1, length);
        fread(sc, length, 1, fdShell);
        VirtualProtect(sc, length, 0x40, (unsigned long *)argv[0]);
        __debugbreak();
        (*(int(*)()) sc)();
        __debugbreak();
        return 0;
    }
