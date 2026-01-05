# Folat v0.2.0
Folat is an imperative esolang and a Finite State Machine (FSM), partially inspired by Assembly.
This esolang uses fixed-width parsing, where the instruction and the two arguments must be exactly 3 characters long with one character between them. Folat was created on December 7, 2025, by the user @ytebbit.

# Contents
- "README.md"    : general repository documentation.
- "LICENSE"      : MIT license.
- "bin"          : binaries.
- "src"          : source codes.
- "src/flib.c"   : library of functions for the instructions.
- "src/flib.h"   : library header.
- "src/main.c"   : Folat interpreter and CLI.
- "src/examples" : a folder of code examples in Folat. The ".fol" extension is optional.
- "src/Makefile" : used to generate the "fol" binary and other functions; run `make help` for more information.
- "bin/fol"      : optimized binary for the CLI.

# Memory
MEM (Memory) is the main memory and is a 50-byte array. PNR (PoiNt Record) is a long that stores the index of the last line that starts with "---". FLG (FLaG) is a boolean that acts as a flag, and the instructions `cmp`, `gpt`, and `gpf` use FLG.

# Syntax
Folat uses a fixed-width parsing model following the `ist trg dat` format. The ist field consists of a 3-character instruction, while the trg field (target) must be a 3-digit number from 000 to 049 (padded with leading zeros). The dat field contains the data. Since empty fields are not permitted, any unused fields must be filled with placeholder text or spaces to avoid the `error: line too short` error or the `error: line too long` error. For documentation, start a line with ; for full-line comments. Inline comments are allowed without a semicolon, provided the total line length is either under 11 or over 150 characters; this prevents the C implementation's fgets function from misinterpreting empty fields.

# Instructions
## Arithmetic
- `add trg dat`: adds dat to the trg index byte in memory.

- `sub trg dat`: subtracts dat from the trg index byte in memory.

## Points
**What are 'points'?**
In Folat, points are lines that begin with "---". When the interpreter processes this line, it notes the line number in the PNR, and you can make the interpreter return to this line with the `gpt` and `gpf` instructions.

- `gpt`: returns to the line that PNR points to if FLG = true.

- `gpf`: return to the line that PNR points to if FLG = false.

## Utilities
- `cmp trg dat`: if MEM[trg] == dat, FLG = true; otherwise, FLG = false. The dat field accepts both ASCII characters and numeric values.

- `set trg dat`: writes dat to MEM starting from the trg index, and converts "\" to 0x0A and "-" to 0x07 (pseudo-null), which becomes 0x00 in MEM.

- `out trg`: simply prints MEM[trg].

- `ext trg`: if trg is 001, the program displays the MEM and terminates; otherwise, the program simply terminates.

# How to use
First, clone the repository and use the "make":
```bash
$ git clone https://github.com/ytebbit/Folat
$ cd folat/src/
$ make
```

To add the binary to the PATH, you can use `make addcli`. Then, you can run the example:
```bash
$ fol folat/src/ex.fol
Hello World!
```

# Examples
## Printing a byte
```
set 000 A--   MEM = A
out 000       STDOUT = MEM[0] (A)
```

## "Hello World!"
```folat
set 000 Hel   MEM = Hel
set 003 lo    MEM = Hello 
set 006 Wor   MEM = Hello Wor
set 009 ld!   MEM = Hello World!
set 012 \--   MEM = Hello World!\n
ext 001       STDOUT = MEM (Hello World!)
```

## Simulating condition
```
set 000 0tf   MEM = 0tf
---           PNR = 2
;          ;   First pass    ; Second pass ; Third pass
cmp 000 050    FLG = false   ; FLG = false ; FLG = true
add 000 001    MEM[0] = 1    ; MEM[0] = 2  ; MEM[0] = 3 (no effect)
out 002        STDOUT = f    ; STDOUT = ff ; STDOUT = fff
gpf            PC = 2        ; PC = 2      ; PC = PC (no effect)
out 001        STDOUT = ffft ;
ext 000        exit(0)
```
