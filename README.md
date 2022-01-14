# ZNCC
Zak's New(-ish) C(-ish) Compiler.

(Alternatively, ZNCC is a recursive acronym for ZNCC is Not a C Compiler.)

## Introduction

This is the latest version of my C-like compiler, which is distantly based on LICE: https://github.com/dorktype/LICE

This is NOT meant to be a full replacement for standards-compliant C/C++ compilers. It's rather intended for bootstrapping new architectures and for use in minimalist high-level computing systems (e.g. where code needs to be strictly audited for security reasons or needs to be kept simple for educational purposes, but still needs some complex/modern functionality).

The compiler mostly targets modern, 64-bit platforms and focuses on converting simple pre-processed C-like code into simple assembler code for the target architecture. It can be easily adapted for other use-cases or bundled with additional tools.

## Features

* Supports multiple architectures and is easy to retarget
    * Mostly tested on x86-64/AMD64 Linux
    * Partial support for Windows ABI
    * RV64 is mostly-supported, with minimal support for RV32 (enough for "Hello world")
    * Linux targets should also work for FreeBSD/OpenBSD/Solaris/etc. with minimal re-tooling (macOS may require a little more work, but not much)
    * Also includes some minimal/experimental support for other/new architectures
* Single compiler tool supports multiple targets (no need for per-architecture compiler builds)
* Compiler is mostly self-hosting (at least on fully-supported targets)
* Supports most (not all) essential C features with some extensions
    * Includes some Objective C-like OOP extensions
    * Basic floating-point support is included (assuming the target has such support)

## Language Features & Limitations

The compiler generally accepts C-like code with some (experimental/incomplete) Objective C-style extensions. It would roughly be on-par with a pre-standard C compiler, except for modern targets.

This means that features like integers, functions, structs, arrays, pointers, etc. **generally** work as per usual, but there are some C features which are **not** implemented in ZNCC:

 * Unpacking "vararg" parameters will not work
     * Varargs can be declared/called but not unpacked
     * This basically means you need to use a different/standard compiler to build any "printf"-like functions (but you can still access them)
 * Passing structures as arguments or return values of functions will not work
     * In other words, arguments are considered to be word-sized
 * String constants with the same text are **not** guaranteed to be `==` at runtime
     * This kind of stuff can generally be done and may sometimes be automatic, but depends on linker features
     * It's generally considered bad practice to rely on this anyway, but may be worth noting

### Bonus Bugs & Temporary Limitations

 * Floating-point support exists and should generally "work" on supported targets, but is minimal
     * This can probably be expanded quite easily in future versions, but will eventually require some platform-specific options
 * Large numbers of function arguments will **partly** work, but not reliably
     * Large numbers of integer/pointer arguments will "work", but may misbehave when combined with floating-point arguments (and may throw off the stack alignment for floating point inside the function)
     * This is just an incomplete feature (i.e. missing float support), and should be easy to fix incrementally
 * Number conversions and precise signed/unsigned/etc. semantics have not been thoroughly fuzz-tested
     * There are likely to be some issues with specific types or combinations, but these are usually easy to fix once identified
 * Pointer arithmetic exists, but is limited
     * For now, this means: Use simple addition/subtraction for pointers, don't rely on increment operators or inner pointer syntax working correctly to obtain offsets
     * These issues can probably be solved incrementally (if not, warnings/errors can be added to catch broken cases)
 * Error reporting is present, but is not ideal for catching bugs/suggesting solutions
 * There is no/minimal optimisation (this is intentional, the focus is on making the thing work first)
 * The object-oriented extensions have potential, but deciding on an exact ABI is difficult
     * Efficient implementations would easily become platform-specific, while unoptimised implementations may be impractical
     * This means that there are some OOP features, but they need to be tweaked and integrate them into some kind of platform to be useful

## Other Limitations/Design Features

There is no built-in preprocessor or associated tools. I recently began integrating such tooling, but ran into a couple of issues.

Most notably, any generally-useful preproccessor ends up more complex than the compiler, so self-hosting can become troublesome.

Secondly, the compiler itself (and LICE which was used as a foundation) are licensed under the terms of Unlicense, while other/third-party components usually imply other (or more-vague) licensing conditions.

And thirdly, the compiler part is more-or-less essential for all use cases. The preprocessor, linker, assembler, and so on are more naturally specialised for particular platforms or combinations of technologies. For example a regular desktop target might require a full preprocessor and other tools, whereas an embedded target may only need to recompile straightforward programs and may use the compiler directly with a special frontend & assembler limited to that target.

## See Also

 * https://github.com/ZYSF/ZNLC "fake libc" headers for self-hosting builds (to ensure the compiler doesn't choke on any obscure system headers)
