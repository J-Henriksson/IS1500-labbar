# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro

#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
#.macro	PUSH reg
#	addi sp,sp,-4
#	sw \reg,0(sp) 
#.endm

#.macro	POP reg
#	lw	\reg,0(sp)
#	addi	sp,sp,4
#.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, delay, time2string, display_string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000	# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################

time2string:
	addi    sp,sp,-12
	sw      ra,0(sp)
	sw	s0, 4(sp)
	sw	s1, 8(sp)
	
	mv 	s0, a0		# get adress
	mv	s1, a1		# get time
	
	li	t1, 0xF000
	and	t0, s1, t1	# isolate fourth digit
	mv 	a0, t0
	srli	a0, a0, 0xC
	jal 	hexasc		# get ASCII for digit 
	sb	a0, 0(s0)	# add digit to string
	
	li	t1, 0xF00
	and	t0, s1, t1	# isolate third digit
	mv 	a0, t0
	srli	a0, a0, 0x8
	jal 	hexasc		# get ASCII for digit
	sb	a0, 1(s0)	# add digit to string
	
	li 	t0, 0x3A
	sb	t0, 2(s0)	#add colon to string
	
	andi	t0, s1, 0xF0	# isolate second digit
	mv 	a0, t0
	srli	a0, a0, 0x4
	jal 	hexasc		# get ASCII fordigit
	sb	a0, 3(s0)	# add digit to string
	
	andi	t0, s1, 0xF	# isolate first digit
	mv 	a0, t0
	jal 	hexasc		# get ASCII for second digit
	sb	a0, 4(s0)	# add digit to string
	
	li 	t0, 0x00
	sb	t0, 5(s0)	#add null byte buffer
		
	lw      ra,0(sp)
	lw	s0, 4(sp)
	lw	s1, 8(sp)
	addi    sp,sp,12
	
	jr      ra
	

hex2asc:
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	


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
  	
 
delay:
	mv 	t0, a0		# get ms to deley
	li 	t2, 1000000	# inner loop counter threshold

outerloop:
	ble    	t0, zero, exit	# loop if ms counter (t0) is >0
	addi 	t0, t0, -1	# decrement ms counter
	li 	t1, 0		# reset inner counter

innerloop:
	addi	t1, t1, 1		#increment counter
	ble    	t1, t2, innerloop	#loop if inner counter (t1) is within threshold
	
	j	outerloop

exit:						
	jr	ra		# exit delay
	
