This page contains all the available opcodes in the VM.

The **[addr]** means the memory address of the data that you are going to use.

## Memory/Stack

The following opcodes are used for memory or stack operations.

| Name          | OpCode | Usage         |     Mnemonic        | Description                             |
| ------------- | ------ | ------------- | ------------------- | --------------------------------------- |
| Set AC        |   01   | 01 **[addr]** | MOV AC, **[addr]**  | Load to AC the contents of the address. |
| Save AC       |   02   | 02 **[addr]** | MOV **[addr]**, AC  | Save to address the contents of AC.     |
| Push to stack |   03   | 03            | PUSH AC             | Push to stack the AC contents.          |
| Pop to AC     |   04   | 04            | POP AC              | Get top item of stack and set it to AC. |
| Clean AC      |   05   | 05            | XOR AC, AC          | ⚠ Clean the AC register.               |
| Clean stack   |   06   | 06            | CALL CleanStack     | ⚠ Clean stack, remove all items.       |
| Print stack   |   07   | 07            | CALL PrintStack     | Print the stack.                        |

## Arithmetic

| Name          | OpCode | Usage         |     Mnemonic        | Description                                     |
| ------------- | ------ | ------------- | ------------------- | ----------------------------------------------- |
| Sum           |   10   | 10 **[addr]** | ADD AC, **[addr]**  | Sum AC with the contents of the address.        |
| Substract     |   11   | 11 **[addr]** | SUB AC, **[addr]**  | Substract AC with the contents of the address.  |
| Multiply      |   12   | 12 **[addr]** | MUL AC, **[addr]**  | Multiply AC with the contents of the address.   |
| Divide        |   13   | 13 **[addr]** | DIV AC, **[addr]**  | Divide AC with the contents of the address.     |

**All arithmetic opcodes stores the result in AC.**

## Conditions

**This section could change in time, conditions doesn't work at this moment but is in development.**

| Name    | OpCode | Usage         |     Mnemonic        | Description                                  |
| ------- | ------ | ------------- | ------------------- | -------------------------------------------- |
| CMP     |   20   | 20 **[addr]** | CMP AC, **[addr]**  | Compare AC with the contents of the address. |
| JMP     |   30   | 30 **[addr]** | JMP **[addr]**      | Unconditional jump to address.               |
| JE      |   31   | 31 **[addr]** | JE **[addr]**       | Jump if Equal (AC == **[addr]**).            |
| JNE     |   32   | 32 **[addr]** | JNE **[addr]**      | Jump if Not Equal (AC != **[addr]**).        |
| JG      |   33   | 33 **[addr]** | JG **[addr]**       | Jump if Greater (AC > **[addr]**).           |
| JGE     |   34   | 34 **[addr]** | JGE **[addr]**      | Jump if Greater or Equal (AC >= **[addr]**). |
| JB      |   35   | 35 **[addr]** | JB **[addr]**       | Jump if Below (AC < **[addr]**).             |
| JBE     |   36   | 36 **[addr]** | JBE **[addr]**      | Jump if Below or Equal (AC <= **[addr]**).   |

## Functions

⚠ When a function is called, the return value will be set to AC. Some functions may clean the stack, other not.

| Name            | OpCode | Usage |     Mnemonic    | Description                                     |
| --------------- | ------ | ----- | --------------- | ----------------------------------------------- |
| CALL_PRINT      |   50   | 50    | CALL print      | Prints the actual AC value.                     |
| CALL_SPRINTF    |   51   | 51    | CALL sprintf_s  | Write formatted data to string.                 |
| CALL_GETCH      |   52   | 52    | CALL getch      | Ask for input. The value will be set to AC.     |
| CALL_MESSAGEBOX |   53   | 53    | CALL MessageBox | Call native WinAPI MessageBoxA.                 |

<hr>

### CALL_PRINT

Prints the actual value of AC register.

⚠ The new line bytes will not be print. You have to provide it with **\n** inside the string, like in the example as follows.

⚠ If AC doesn't have any value or is empty, only a new line will be print.

**OpCode**: 50.

**Needs stack items?** ⛔

**Returns:** Nothing.

**Cleans the stack?** ⛔

**How many stack items needs?** None.


```cpp
0001 01 0100
0002 50

0100 "Hello world!\n"
```

<hr>

### CALL_SPRINTF

Returns a string produced according to the formatting string.

**OpCode**: 51.

**Needs stack items?** ✅

**Returns:** The formatted string.

**Cleans the stack?** ✅

**How many stack items needs?** The format item plus the quantity of **%pop%** found in the format.

For example, consider this example:

```cpp
...

0001 01 0101 //Set AC = Iribe
0002 03      //Push AC to stack
0003 01 0100 //Set AC = Brayan
0004 03      //Push AC to stack
0005 01 0102 //Set AC = "Welcome..."
0006 03      //Push AC to stack
0007 51      //Call sprintf, once is called, AC holds the formatted string
0008 50      //Call print, prints the actual AC value

0100 "Brayan"
0101 "Iribe"
0102 "Welcome, %pop% %pop% to IvyVM!"
```

In that example, before the **CALL_PRINT**, the stack is like this:

| Stack                          |
| ------------------------------ |
| Welcome, %pop% %pop% to IvyVM! |
| Brayan                         |
| Iribe                          |

Two %pop% are in the string, so it needs two items apart of itself in order to make CALL_SPRINTF work.

CALL_SPRINTF will pop the stack once is called. The first item got will be the format.
Once it have the format, it'll count the quantity of %pop% that are itself.
So it'll loop through the stack and pop every item to get the desired items.

The result will be as follows:

<p align="center">
<img src="https://user-images.githubusercontent.com/520683/65398137-6a3a7a80-dd6a-11e9-9a7c-88283fbfc719.png" ext="Return">
</p>

<hr>

### CALL_GETCH

Return the next user input in console.

⚠ If you **CALL_PRINT** before this function, you don't need to put the new line because it will be set once this function succeds.

**OpCode**: 52.

**Needs stack items?** ⛔

**Returns:** The next user input in console.

**Cleans the stack?** ⛔

**How many stack items needs?** None.


Example: 
```cpp
0001 01 0100 //Set AC to "Write your..."
0002 50      //Print the AC value
0003 52      //Call getch
0004 03      //Push to stack the input value
0005 01 0101 //Set AC to "Hello %pop%!"
0006 03      //Push to stack the format
0007 51      //Call sprintf
0008 50      //Print the formatted string

0100 "Write your name: "
0101 "Hello %pop%!"
```

Result:

<p align="center">
<img src="https://user-images.githubusercontent.com/520683/65398727-be932980-dd6d-11e9-9f02-26510a965d89.png" ext="Return">
</p>

<hr>

### CALL_MESSAGEBOX

Show a native WinAPI MessageBoxA.

**OpCode**: 53.

**Needs stack items?** ✅

**Returns:**  The WinAPI return value.

**Cleans the stack?** ✅

**How many stack items needs?** (optional) uType delimited with "|", lpCaption and lpText.

⚠ **Hint**: You can use **CALL_SPRINTF** to format the string and print on the MessageBox.

Example:

```cpp
0001 01 0100 //Set AC to uType
0003 01 0101 //Set AC = "lpCaption"
0004 03      //Push AC
0005 01 0102 //Set AC = "lpText"
0006 03      //Push AC
0007 53      //Call MessageBoxA
0008 50      //Call Print

0100 "MB_ICONEXCLAMATION|MB_YESNO"
0101 "lpCaption"
0102 "lpText"
```

The previous example shows a MessageBox. You can put in uType the params provided in [WinAPI MessageBox Function](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox?redirectedfrom=MSDN#6).

You can remove the uType from stack and the code will work properly. That argument is optional.