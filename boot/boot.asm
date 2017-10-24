
global multiboot_entry	 ;这个是boot的入口，即ELF文件入口
global	start, _start	
 
extern 	gdt_ptr		
extern  cinit            ; 这是初始化
extern 	cheak		 ;这是保存multiboot 信息的函数
extern  hh		 ;打印HLT
extern  k		

%include	"./boot/pm.inc"
MODULEALIGN 	equ  1<<0                       ; align loaded modules on page boundaries
MEMINFO   	equ  1<<1                   	; provide memory map
FLAGS     	equ  MODULEALIGN | MEMINFO  	; this is the Multiboot 'flag' field
MAGIC       	equ   0x1BADB002           	; 'magic number' lets bootloader find the header
CHECKSUM    	equ -(MAGIC + FLAGS)        	; checksum required
TEMPSTACKSIZE	equ	0x4000			;16k
STACKTOP 	equ 0x100000  -4            

	
[section .text]
[BITS 32]
align	4
;Align 32 bits boundary.

multiboot_header:
	 dd MAGIC	
   	 dd FLAGS
   	 dd CHECKSUM
;貌似不需要ELF头,规范上有说明ELF头不是必须的。so,不写。

multiboot_entry:

	;LGDT会丢失所有信息，临时堆栈来个然后调用一个C写的函数保存此信息
	mov esp, stack+TEMPSTACKSIZE	           ; set up the temp stack
	finit 
	push ebx                           ; pass Multiboot info structure
	push eax                           ; pass Multiboot magic number
   	call  cheak                        ; call kernel proper
	
	;切换GDT,丢弃所有信息，当然所需要的信息已经全部保存到内存中
	lgdt	[GdtPtr]
	
	mov	ax, Selector_DATA
	mov	ds, ax			; 数据段选择子
	mov	es, ax
	mov	fs, ax
	mov	ax, Selector_VIDEO
	mov	gs, ax			; 视频段选择子
	mov	ax, Selector_STATCK		
	mov	ss, ax			; 堆栈段选择子
	mov	esp, STACKTOP

	sgdt[gdt_ptr]
	call	cinit			;初始化GDT IDT	
	jmp	Selector_CODE:go	;这里的远跳转是为了让代码段信息更新
go:	
	sti				;开中断
					;这里尚未 分页。。不太清楚做法如何
	call 	k			;这里应该算初始化真正完毕了，该进入内核 实现进程调度

	call hh				;打印HLT结束信息	
loop:	hlt
	jmp	loop

[section .data]
[BITS 32]

; GDT
;  	                                           段基址               段界限,   属性
GDT_NULL:		Descriptor             0,                    0,  0						
GDT_FLAT_C:		Descriptor             0,              0fffffh,  DA_CR  | DA_32 | DA_LIMIT_4K			
GDT_FLAT_RW:		Descriptor             0,              0fffffh,  DA_DRW | DA_32 | DA_LIMIT_4K		
GDT_VIDEO:		Descriptor	 0B8000h,              0ffffh,   DA_DRW | DA_DPL3	; 显存
GDT_STACK:		Descriptor       100000h,              0ffh,   DA_DRW | DA_32 | DA_LIMIT_4K

GdtLen		equ	$ - GDT_NULL
GdtPtr		dw	GdtLen - 1				; 段界限
		dd	GDT_NULL				; 基地址

; GDT 选择子 
Selector_CODE		equ	GDT_FLAT_C	- GDT_NULL
Selector_DATA		equ	GDT_FLAT_RW	- GDT_NULL
Selector_VIDEO		equ	GDT_VIDEO	- GDT_NULL+ SA_RPL3
Selector_STATCK		equ	GDT_STACK	- GDT_NULL

[section .bss]
[BITS 32]
;我只是临时用的堆栈而已，切换GDT后会被抛弃
stack:
   resb TEMPSTACKSIZE      ; reserve 16k stack on a quadword boundary

	

