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
* Compiler is self-hosting (at least on sup
* Supports most (not all) essential C features with some extensions
    * Includes some Objective C-like OOP extensions
    * Basic floating-point support is included (assuming the target has such support)

## Lacking

Preprocessor and associated tools.

I recently began integrating such tooling, but ran into a couple of issues.

Most notably, any generally-useful preproccessor ends up more complex than the compiler, so self-hosting can become troublesome.

Secondly, the compiler itself (and LICE which was used as a foundation) are licensed under the terms of Unlicense, while other/third-party components usually imply other (or more-vague) licensing conditions.

And thirdly, the compiler part is more-or-less essential for all use cases. The preprocessor, linker, assembler, and so on are more naturally specialised for particular platforms or combinations of technologies. For example a regular desktop target might require a full preprocessor and other tools, whereas an embedded target may only need to recompile straightforward programs and may use the compiler directly with a special frontend & assembler limited to that target.
