## Assignment 2

1. What does it mean when a function does not return a value? How do you state that in a
program? How can the function (or more precisely, the procedure) perform anything useful?

A function not returning a value means it doesn't give back a value like a mathemical function, but rather does some kind of action. A non-returning function is declared with the prefix "void". The function can still perform actions through modifying global variables, by using pointers or printing to the terminal.



2. How did you implement the side effect needed to make print_number behave correctly?

I used a global variable as a counter to keep track of how many columns had been printed over multiple function calls and when to write a new line.



## Assigment 3

1. How did you represent the marking of 'prime' and 'not a prime' in the memory array?

Each number was represented by the element with the corresponding index. Primes where marked with the value 1 and non-primes with the value 0.



2. What are the main steps in the algorithm? How have you implemented these steps?

The algorithm starts at the first prime which is 2, then marks all multiples of it up to the given parameter. Then advances to the next number not marked as a multiple and marks all multiples of that. This is repeated up to the root of the given parameter.

This is implemented as two loops. The first outer loop goes from 2 up to the root of the parameter number. For each number it checks if it has already been marked as a prime. If not, it enters the inner loop that goes through all multiples and marks them as not primes.




3. What is the largest prime number that you can print within 2 seconds of computation? What
is the largest number you can print within 10 seconds? Is it the same for print_prime.c,
sieves.c, and sieves-heap.c? Why or why not?

for sieves we get a segmentation error before it takes longer than 2 seconds to execute. This is due to the stack being limited to around 8 MB (8192 kb = 8192 * 1024 = 8 388 608 bytes), meaning the largest byte is somewhere around there depending on how much free space is on the stack.

print-primes prints up to around 4 499 969 within 2s and 17 999 911 within 10s 

sieves-heap prints up to around 29 999 999 within 2s and 159 999 957 within 10s



## Assigment 4

1. Explain how you get the pointer addresses to the two char arrays (text1 and text2) and the counter variable (count) in function work().

Text1 and text2 are declared as string literals, which makes them pointers to a char array containing the string.

they along with counter are global variables that can be accessed directly by work(), text1 and text2 are already pointers containing the adress. and the adress for counter can be accessed using the '&' sign.



2. What does it mean to increment a pointer? What is the difference between incrementing the
pointer that points to the ASCII text string, and incrementing the pointer that points to the
integer array? In what way is the assembler code and the C code different?

To increment a pointer changes adress it points to. incrementing a pointer to a ASCII text string increases it by one to the next byte and the next character. When incrementing a pointer to an integer array we increase by 4 bytes to the start of the next integer. In C you increase by 1 for both and the compiler automatically increments by the correct number of bytes. In C you need to do it manually.



3. What is the difference between incrementing a pointer and incrementing a variable that a pointer points to? Explain how your code is incrementing the count variable.

incrementing a pointer changes where in memory it points to. Incrementing a variable a pointer points to changes the value of that variable.

The code increments the code using the line "(*counter)++;" where counter is the pointer to the counter variable, and the '*' means we are accessing the value it points to.



4. Explain a statement in your code where you are dereferencing a pointer. What does this mean? Explain by comparing it with the corresponding assembler code.

in the first line of the while loop of copycodes(), "(*list) = (int) (*text);". the list and text pointers are dereferenced so we access the values they point to and can put the value pointed to by text into the memory address pointed to by list.



5. Is your computer using big-endian or little-endian? How did you come to your conclusion? Is there any benefit of using either of the two alternatives?

It uses little-endian. We can see it by looking at the endian-proof() function. It prints the number in counter through a pointer. It first prints the value in the lowest adress within the pointer itself then prints the value in the adress offset by 1 then by 2 and so on. And as we see the first byte to be printed is 0x23. 


## Assignment 5

1. Consider AM18, AM19, and AF1. Explain why gv ends up with the incremented value, but m
does not.

The function fun() takes a value as an argument, the internal "param" has no connection to the passed variable. When param is modified m stays unchanged. Then the value of param is given to gv which is a global variable.



2. Pointer cp is a character pointer that points to a sequence of bytes. What is the size of the cp pointer itself?

cp cointains the adress of the first byte in the sequence, which is 32 bits or 4 bytes.



3. Explain how a C string is laid out in memory. Why does the character string that cp points to have to be 9 bytes?

A C string is stored as s series of bytes, each containing the ASCII code for one character + a nullbyte to mark the end of the string. 



4. Which addresses have fun and main? Which sections are they located in? What kind of
memory are they stored in? What is the meaning of the data that these symbols points to?

The adress are:
fun: 0x00000570
main: 0x00000684

They are stored in the text section of the RAM, along with the rest of the code. The data is the first machine code instruction of the functions.