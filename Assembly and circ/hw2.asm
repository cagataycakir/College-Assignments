.data
	myArr: .space 200	#4 byte * 50
	mes1: .asciiz "Possible!\n"
	mes2: .asciiz "Not possible!\n"
	mes3: .asciiz "\n Array size should be 0 to 50."

.text
main:
	jal getUserIn
	
	la $a2, myArr		#set array pointer head
	#a0 has array size
	#a1 has search value
	#a2 is array pointer
	#v0 and v1 are used in for loop of the recursive func
	#myArr stores values in heap (max 50)
	
	jal checkSumPoss

	li $v0, 4	#print matching set is not found
	la $a0, mes2
	syscall

	li $v0, 10 	#set system call func 10: exit
	syscall

checkSumPoss:
	addi $v0, $zero, 1	#set loop incr (starts with 1)
	addi $v1, $a0, 0	#set loop end (current size)
	
	#push my values to stack for recursive func
	#a0 is size, a1 is num, a2 is array pointer
	addi $sp, $sp, -16
	sw $ra, 0($sp)
	sw $a0, 4($sp)
	sw $a1, 8($sp)
	sw $a2, 12($sp)
	
	blez $a0, returnNeg	#if (size =< 0) return 0;
	
	lw $t0, 0($a2)		#if (arr[0] == num) return 1;
	beq $t0, $a1, returnPosi
	
	beq $a0, 1, returnNeg	#if (size == 1) return 0;

	sub $t0, $a1, $t0	#arr[0]-num (t0 currently shows array)
	bgezal $t0, forLoop	#if(num > arr[0]) <for loop iteration>
	
	lw $ra, 0($sp)		#pop branch int
	lw $a0, 4($sp)
	lw $a1, 8($sp)
	lw $a2, 12($sp)
	addiu $sp,$sp,16
	
	#CheckSumPossibility(num,&arr[1],size-1)   <a1><a2><a0>
	sub $a0, $a0, 1		#size-1
	addi $a2, $a2, 4	#array[1]
	j checkSumPoss
	
#if match is not found in a branch, there still could be matches
returnNeg:

	lw $ra, 0($sp)		#pop branch int
	lw $a0, 4($sp)
	lw $a1, 8($sp)
	lw $a2, 12($sp)
	addiu $sp,$sp,16

	jr $ra			#return from branch

#if match is found we can just cut to the case and end the program
returnPosi:
	li $v0, 4	#print matching set is found
	la $a0, mes1
	syscall
	
	li $v0, 10 	#set system call func 10: exit
	syscall
	
getUserIn:
	li $v0, 5	#get user integer input for array size
	syscall
	
	#check array size
	bltz $v0, inArr	#arrS<0
	addi $t0, $v0 -50 #v0-50
	bgtz $t0, inArr	#arrS>50
	
	addiu $a0,$v0,0	#reserve array size for return
	addiu $s0,$v0,0	#reserve array size for loop
	
	li $v0, 5	#get user integer input for searched integer
	syscall
	addiu $a1,$v0,0
	
	##
	
	la $s1, myArr		#temp arr pointer for loop
	addiu $s2,$ra,0 	#reserve function call for loop
	
	beqz $s0, userInLoopSkip
	
   userInLoop:
	subiu $s0,$s0,1
	
	li $v0, 5		#get user input for array elements
	syscall
			
	sw $v0, 0($s1)		#add to array
	addi $s1, $s1, 4
	
	bnez $s0 userInLoop

   userInLoopSkip:
	
	##
	
	addiu $ra,$s2,0 	#restore function call
	
	jr $ra			#return from branch
	
forLoop:
	#push branch int
	addi $sp, $sp, -12	#move stack pointer
	sw $ra, 0($sp)		#save return adress to stack
	sw $v0, 4($sp)		#save loop increment to stack
	sw $v1, 8($sp)		#save loop size
	##for (v0 to v1)

	sub $t1, $v1, $v0
	blez $t1, loopOut
	#########################
	#CheckSumPossibility(num-arr[0],&arr[i],size-i)   <a1><a2><a0>
	sub $a0, $a0, $v0	#size-i
	
	lw $t0, ($a2)		#arr[0]
	sub $a1, $a1, $t0	#num-arr[0]
	
	addi $t0, $zero, 4	#4byte array size
	mult $v0, $t0		#multiple array size by loop pointer
	mflo $t0		#get pointer location relative to array start (i)
	add $a2, $a2, $t0	#current pointer + relative location = arr[i] 
	
	##call task with address
	jal checkSumPoss	#call task with address
	
	#########################
	#pop branch int
	lw $ra, 0($sp)		#load return adress to stack
	lw $v0, 4($sp)		#load loop increment from stack
	lw $v1, 8($sp)		#load loop size
	addiu $sp,$sp,12	#move stack pointer
	
	addi $v0, $v0, 1	#++i
	
	j forLoop
	loopOut:
	
	lw $ra, 0($sp)		#load return adress to stack
	lw $v0, 4($sp)		#load loop increment from stack
	lw $v1, 8($sp)		#load loop size
	addiu $sp,$sp,12	#move stack pointer
	
	addi $v0, $zero 1	#i=1
	jr $ra

#if invalid array size end prog
inArr:
	li $v0, 4	#print error message
	la $a0, mes3
	syscall
	
	li $v0, 10 	#set system call func 10: exit
	syscall