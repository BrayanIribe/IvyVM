### Registers

A register is a small piece of memory of high speed that is inside of the CPU. This memory is temporary, so if you shutdown the computer it will be lost.

#### The AC register

AC means **Accumulator**. The AC register is used for storing temporal data, and doing simple operations. In real assembly, there are different registers. But since this is a simple machine we have only one.

Example: You can use AC register to store a number, and then call a function to sum a number in memory with the AC. The result of this function will be in AC.

#### The PC register

PC means **Program Counter**. The PC register is used to track the current program flow. This register stores the instruction address that is being executed. This register can change if you do a jump.

If you change the PC register with a **JMP** for example, the program flow will be changed to the address provided in jump.

#### The RP register

RP means **Real Program Counter**. The RP register is used internally by the VM. This register stores the number of instructions that has been executed.