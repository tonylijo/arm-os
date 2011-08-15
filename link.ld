ENTRY(_start_boot);
MEMORY {
       FLASH (rx): ORIGIN = 0, LENGTH = 512K
       ram_isp_low(A): ORIGIN = 0x40000120,LENGTH = 223
       RAM (rwx): ORIGIN = 0x40000200 , LENGTH = 32224
       ram_iso_high(A): ORIGIN = 0x40007FE0,LENGTH = 32
}
_stack_end = 0x40007EDC;
SECTIONS {
	 . = 0x00000000;
	 .text : {
	 	. = ALIGN(4);
		_stext = .;
		*(.text);
		*(.rodata);
		. = ALIGN(4);
		_etext = .;
		}>FLASH
	.data : AT (_etext) {
	       . = ALIGN(4);
		_sdata = .;
		*(.data);
		. = ALIGN(4);
		_edata = .;
		}>RAM
	.bss : {
	       . = ALIGN(4);
	       _sbss = .;
	       *(.bss);
	       *(COMMON)
	       . = ALIGN(4);
	       _ebss = .;
	       }>RAM
}
