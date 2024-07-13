# Woody-Woodpacker
.
├── docs
│   ├── en.subject.pdf
│   ├── links
│   ├── linus_elf.h
│   ├── subject
│   └── Woody-Woodpacker.png
├── Makefile
├── README.md
├── res-intra
│   ├── sample
│   ├── sample.c
│   └── woody
├── samples
│   ├── clean.sh
│   ├── make_samples.sh
│   ├── return.c
│   ├── sample.c
│   └── sample_complex.c
├── src
│   ├── buzz_buzzard.s
│   ├── error.c
│   ├── main.c
│   ├── make_buzz.sh
│   ├── utils.c
│   └── woody.h
└── unit
    ├── a_b.sh
    ├── gdb.script
    └── pass.sh

## `make`

### Commons:
- `make all`, `make re`, `make clean` and `make fclean`

### Accessories (included in `make all`):
- `make samples`
- `make blob`

### Development:
- `make v`: runs Valgrind on `woody_woodpacker` then on the originated `woody`.
- `make g`: `gdb` over `woody_woodpacker sample`
- `make gg`: `gdb` over `woody` (grab crashes)
- `make see`: prints `woody` header and dumps 512 bytes from entry point.

### Tests:
- `make t`: tests for an empty program that returns 42.
- `make s`: tests for a "Hello, World!".
- `make c`: tests for a "complex" file that import libraries and leave an intentional leak.
- `make l`: tests over `/bin/ls`.
- All above can be preceded by `d`: `dt`, `ds`, `dc` and `dl`. Aggregates display differences using `vimdiff`.
