# 2048

This repository features the hit game 2048 at the UEFI level.

# Compilation

This has only been tested and run on Ubuntu 24.04.5 LTS.

## Requirements

- gcc
- make

## Steps

Get and build `gnu-efi`

```
git submodule update --init gnu-efi
make -C gnu-efi
```

Compile into object files.

```
gcc -Ignu-efi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c main.c -o main.o
gcc -Ignu-efi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c 2048.c -o 2048.o
gcc -Ignu-efi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c rng.c -o rng.o
```

Link object files.

```
ld -shared -Bsymbolic -Lgnu-efi/x86_64/lib -Lgnu-efi/x86_64/gnuefi -Tgnu-efi/gnuefi/elf_x86_64_efi.lds gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o 2048.o rng.o main.o -o main.so -lgnuefi -lefi
```

Convert to EFI executable.

```
objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --output-target efi-app-x86_64 --subsystem=10 main.so main.efi
```

# Running

`main.efi` is now ready to run. I added this to my grub menu, but you can run them using whatever method you use.

_If on Linux_, add the following to `/ect/grub.d/40_custom`, replacing `/path/to/main.efi` with the executable's actual path.

```
menuentry '2048' {
	set root=(*partition with main.efi*)
	chainloader /path/to/main.efi
}
```

Update your grub menu.

```
sudo update-grub
```
