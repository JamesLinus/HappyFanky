extern gdt_ptr
extern idt_ptr
	
global loadgdt
global loadidt
global out_byte
global in_byte
global udd

[section .text]
[bits 32]
align 4




loadgdt:
	lgdt[gdt_ptr]
	ret
loadidt:
	lidt[idt_ptr]
	ret


;void out_byte(u16 port, u8 value)

out_byte:
	mov	edx, [esp + 4]		; port
	mov	al, [esp +8]		; value
	out	dx, al
	nop	
	nop
	ret

; u8 in_byte(u16 port);
in_byte:
	mov	edx, [esp + 4]		; port
	xor	eax, eax
	in	al, dx
	nop	; 一点延迟
	nop
	ret
	

