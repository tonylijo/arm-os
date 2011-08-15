@;;	startup code for initialising the arm processor.

	.equ RAM_BASE, 0x40000000
	.equ RAM_SIZE, 32*1024
	.equ RAM_TOP, (RAM_BASE + RAM_SIZE)
	.equ UNDEF_STACK_SIZE, 0x00000004
	.equ ABT_STACK_SIZE, 0x00000004
	.equ FIQ_STACK_SIZE, 0x00000004
	.equ SVC_STACK_SIZE, 0x00000400
	.equ IRQ_STACK_SIZE, 0x00000400
	
	
	.equ I_BIT, 0x80
	.equ F_BIT, 0x40
	.equ T_BIT, 0x20

	.equ MOD_USR, 0x10
	.equ MOD_IRQ, 0x12
	.equ MOD_FIQ, 0x11
	.equ MOD_SVC, 0x13
	.equ MOD_ABT, 0x17
	.equ MOD_UNDEF, 0x1B
	.equ MOD_SYS, 0x1F
	
	.text
	.arm
	.global _start_boot
	.func _start_boot
_start_boot:
_vetors:	
	ldr pc, reset_addr
	ldr pc, undef_addr
	ldr pc, swi_addr
	ldr pc,	prefechabt_addr
	ldr pc, dataabt_addr
	nop
	ldr pc,  [pc,#-0xFF0]
	ldr pc, fiq_addr

reset_addr:
	.word reset_init

undef_addr:
	.word undef_addr

swi_addr:
	.word swi_addr

prefechabt_addr:
	.word prefechabt_addr

dataabt_addr:
	.word dataabt_addr

fiq_addr:
	.word fiq_addr
	.word 0
reset_init:	
setup_stacks:
	ldr r0, =_stack_end

@;;	initialize undefined mode stack

	msr cpsr_c,#MOD_UNDEF|I_BIT|F_BIT
	mov sp,r0
	sub r0,r0,#UNDEF_STACK_SIZE

@;;	initialize abort mode stack

	msr cpsr_c,#MOD_ABT|I_BIT|F_BIT
	mov sp,r0
	sub r0,r0,#ABT_STACK_SIZE

@;;	initialize fiq mode stack

	msr cpsr_c,#MOD_FIQ|F_BIT|I_BIT
	mov sp,r0
	sub r0,r0,#FIQ_STACK_SIZE

@;;	initalize irq mode stack

	msr cpsr_c,#MOD_IRQ|F_BIT|I_BIT
	mov sp,r0
	sub r0,r0,#IRQ_STACK_SIZE

@;;	initalize supervisory mode stack

	msr cpsr_c,#MOD_SVC|F_BIT|I_BIT
	mov sp,r0
	sub r0,r0,#SVC_STACK_SIZE

@;;	initialize system mode stack

	msr cpsr_c,#MOD_SYS|F_BIT|I_BIT
	mov sp,r0

@;;	back to supervisory mode.

	msr cpsr_c,#MOD_SVC|F_BIT|I_BIT
	
init_data_seg:	
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_etext


@;;-------------------------------
@;;	 function copy := the data segment code from flash to ram as ram is volatile.
@;;
@;;	 r0 - start of data segment in ram
@;;	 r1 - end of data segment in ram 
@;;	 r2 - start of data segment in flash 
@;;--------------------------------

copy:	cmp r0,r1	
	ldrltb r3, [r2],#1
	strltb r3, [r0],#1
	
	blt	copy 


init_bss_seg:
	ldr r0, =_sbss
	ldr r1, =_ebss
	mov r3, #0
@;;---------------------------------
@;;	 function zero := make all variable which ar declared in un initialized data segment is made zero
@;;	 r0 start of bss segment in ram. 
@;;	 r1 end of bss segment in ram.
@;;	 r3 value zero.
@;;--------------------------------- 
zero:	cmp r0,r1
	strltb r3,[r0],#1
	
	blt zero


	b main
.endfunc
.end
