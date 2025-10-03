Assignment 1:
Changed row 16 "addi	s0, s0, 0x01" to "addi	s0, s0, 0x03"

why: The instruction adds 1 (0x01) to s0 which contains the value that is printed as ASCII. By changing the added value 
to 3 (0x03) we increment by 3 and instead only print every 3rd character. 

