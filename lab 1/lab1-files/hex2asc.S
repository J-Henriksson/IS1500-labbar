  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # edited 2025-09-02 by Viktor Eriksson
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 15	# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  
  hexasc:
  	
  	mv 	t0, a0		#move input to t0 to free up a0		
  	
  	andi 	t0, t0, 0xF	#isolate 4 LSBs
  	
  	li 	t1, 9
  	bgt 	t0, t1, letter	#branch if >9
  	
digit:
  	li 	a0, 0x30	#set a0 to ASCII '0'
  	add 	a0, a0, t0
  	
  	jr	ra
  	
letter:
  	addi 	t0, t0, -10	#shift so 10 -> 0
  	li 	a0, 0x41	#set ao to ACII 'A'
  	add 	a0, a0, t0
  	
  	jr	ra
  	
