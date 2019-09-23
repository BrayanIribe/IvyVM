
## Syntax

All the files **should** have a .ivy extension. If you don't do so, the VM will not recognize the argument provided as file.

The following snippet prints **"Hello World"** in console.

```cpp
//Simple IvyVM program that prints Hello World.

0001 01 0100
0002 50

0100 "Hello World"
```

### Memory

Let's dissect the first instruction:

<p align="center">
<img src="https://user-images.githubusercontent.com/520683/65395260-59c7d700-dd4d-11e9-8a18-1499942756dc.png" width="400" alt="Syntax">
</p>

- **Address**: The address is an unique identifier used by the CPU to track the instruction or data. The addresses **should be** consecutive, the limit is of 9999 and the addresses cannot be repeated. You can start with the address 1 or 0. Be careful, don't mix instructions with data. If you do so, the program will end.

- **Operation Code (OpCode)**: Is the signal that receives the CPU to know what you want to do.

- **Arguments**: Some OpCodes doesn't require arguments. Generally, the arguments are addresses relative to memory. 

**Avoid mixing instructions with data**. You can make a padding between instructions and data, like the snippet above. Where instructions start at 0001 and the data starts in 0100.

You need to add a padding at addresses and instructions.

For example:

```cpp
1 1 100      // WRONG
0001 01 0100 // OK
```

The addresses have four digits, so you will make a pad of that size.
The instructions have two digits, the pad should be of two.

All notation used in the VM is decimal, don't use hexadecimal or binary because it will not work.