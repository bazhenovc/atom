Atom
====

Atom is a library for manipulating human-readable ASCII string literals packed into 64 bit integers.

It allows creating and comparing strings that are up to 8 characters long very efficiently from the machine perspective.
It is a very small header-only library with NO additional dependencies. It does not #include anything and does not pollute
the global namespace with macros or global variables.

Usage
====

Use this to create and compare small strings. For example:

```
atom_t atom0 = literal("Atom 0");
atom_t atom1 = literal("Atom 1");
if (atom0 != atom1) {
 // do something useful
}
```

All compilers will replace literal(...) calls with statically calculated constant, so creating a new atom takes zero time.
For instance, Clang 3.4.1 will generate the following assembler code for the example above:

```
movabsq	$52915833107521, %rax   # Store "Atom 0"
movq	%rax, -8(%rsp)
movabsq	$54015344735297, %rax   # Store "Atom 1"
movq	%rax, -16(%rsp)
movq	-8(%rsp), %rax
cmpq	-16(%rsp), %rax         # Compare them
```

As you can see, it generates a single MOV instruction to create each atom (store a constant in the register)
and a single CMP instruction to compare them.
What can be faster?

Requirements
====

This library requires C++11 support in the compiler for static_assert usage, however this is not a strict requirement,
because static_assert can be easily implemented without C++11.

Officially supported compilers are:
   * MSVC  >= 2012
   * GCC   >= 4.4
   * ICC   >= 13.01
   * Clang >= 3.2

License
====

```
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
```
