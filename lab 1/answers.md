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
