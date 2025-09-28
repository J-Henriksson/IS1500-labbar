Assignment 1:
Changed row 16 "addi	s0, s0, 0x01" to addi	"s0, s0, 0x03"

why: The instruction adds 1 (0x01) to s0 which contains the value that is printed as ASCII. By changing the added value 
to 3 (0x03) we increment by 3 instead only print every 3rd character. 


Assignment 2:


Your subroutine hex2asc is called with an integer-value as an argument in register a0 and
returns a return-value in register a0. If the argument is 17, what is the return-value? Why?

the output would be 1.
17 in binary is 10001, the subroutine ignores all bites but the 4 least significant, meaning we get 0001 which is 
1 in decimal. 



If your solution contains a conditional-branch instruction: which input values cause the
instruction to branch to another location? This is called a taken branch.

10 - 15



Assingment 3:


Which registers are saved and restored by your subroutine? Why?

s0 and s1 are saved and restored so they can be used temporarily to store the NBCD time and the address for where to 
save the ASCII time string. s0 and s1 is used because they are calle-saved, meaning they are safe across the function
calls to "hexasc", since if it uses them, the function also has to restore them. The content of a0 is also saved
when it contains an ASCII value returned by "hexasc".



Which registers are used but not saved? Why are these not saved?

t0 and t1 are used for temporarily in the process of isolating parts of the NBCD time. They are not saved as they 
are caller-saved, meaning whatever called the subroutine is responsible for saving them if it needs them after. 
 


Assume the time is 16:53. Which lines of your code handle the '5'?

line 142 - 146 (28 - 32 in subroutine)



Assignment 4:


If the argument value in register a0 is zero, which instructions in your subroutine are
executed? How many times each? Why?

The subroutine will execute down to the 5th row that has the condition for the outer loop (outer counter >=0).



Repeat the previous question for a negative number: -1.
Same thing will happen, since the condition is if the counter is less than or equal 0).


