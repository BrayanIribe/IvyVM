<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/29/Data_stack.svg/782px-Data_stack.svg.png" width="300" alt="Example"/>
</p>

> Generally speaking, a stack is a data structure that stores data values contiguously in memory. Unlike an array, however, you access (read or write) data only at the "top" of the stack.
><cite>https://en.wikibooks.org/wiki/X86_Disassembly/The_Stack</cite>

In other words, the stack is another type of temporal memory. Where you can put whatever you want and store it, by this way you don't need to worry with the AC register.

The stack is commonly used for calling functions, and since the stack works from top to bottom, you should pass arguments in **right to left order.** (for more information, see the STDCALL standard. The URL is at the top of this document).

Let's take in account the following snippet (hello-world.ivy) that do a **CALL_MESSAGEBOX**:
```cpp
// https://github.com/BrayanIribe/IvyVM
// Simple IvyVM program that shows a MessageBox with "Hello World".
// Like ASM, when you call functions, the stack should have
// the arguments with right-to-left sense.

0001 01 0101 //Load string in AC
0002 03      //Push to stack the AC value
0003 01 0100 //Load string in AC
0004 03      //Push to stack the AC value
0005 53      //Call MessageBox

0100 "Title"
0101 "Hello world!"
```

**CALL_MESSAGEBOX** requires at least two items in stack.
In C++, MessageBox should be:
```cpp
MessageBoxA(hWnd, lpText, lpCaption, uType);
```

However, since we are calling the function in ASM, and the VM follows the **STDCALL** standard, the function reads the stack to get the arguments (**like any function that you are going to use**) and the return value is stored in AC.

Now, let's dissect the snippet:

- The script loads **0101** which have "Hello World!", to AC.
- Then, pushes the AC contents to stack.
- Now, loads **0100** which have "Title", to AC.
- Then, pushes the AC contents to stack.

At this moment, the stack should look like this (this is imaginary):

|     Stack    |
| ------------ |
|     Title    |
| Hello world! |

So, if you call pop, you will get the top item of the stack. Which is **Title**. And will remove it from stack.

This is what happens in VM:
```cpp
string lpCaption = this->pop();
string lpText = this->pop();
MessageBoxA(NULL, lpText.c_str(), lpCaption.c_str(), MB_OK);
stack.clear();
```

When you do CALL_MESSAGEBOX, the previous snippet will be executed.

- lpCaption is the title of the MessageBox, so the top item is **Title**.
- lpText is the caption or body of the MessageBox, the next item is **Hello World**.

Once the VM get the values from stack, it calls the native WinAPI function [MessageBoxA](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxa).

After calling this function, the stack is cleared and the return value is stored in AC. Some functions may not return nothing and they will leave AC as it was before the call.