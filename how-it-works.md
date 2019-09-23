# How it works

IvyVM emulates a primitive computer with a CPU that do simple operations. If you know **nothing** of assembly, I'd suggest you to take a look to this [book](https://en.wikibooks.org/wiki/X86_Disassembly).

The VM runs at the most lower level of a computer. There is not any operating system, **no memory pagination, memory permissions, drivers, kernel, etc**. Just only instructions that are executed by the CPU in raw.

I'll explain some things in this wiki, but I'm pretty sure that I'll not cover all of the ASM chapters.

This VM is inspired in Von Neumann Architecture. You can take a look to it [here](https://en.wikipedia.org/wiki/Von_Neumann_architecture).

The VM follows the [STDCALL standard](https://en.wikibooks.org/wiki/X86_Disassembly/Calling_Conventions#STDCALL).