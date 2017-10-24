#include "type.h"
#include "protect.h"

//GDT  GDTR 这是全局变量
u8 		gdt_ptr[6];  //0-15 LIMIT 16-47 base
u8		idt_ptr[6];
DESCRIPTOR	gdt[GDT_SIZE];
GATE		idt[IDT_SIZE];

//汇编写的函数声明
void* memcopy(void* pDest, void* pSrc, int iSize);
void loadgdt();
void loadidt();
void init_8259A();


extern void init_prot();

void set_descriptor(DESCRIPTOR *p,u32 base,u32 limit,u16 attribute);
void init_gdt();
void reflash_gdtr();
void reflash_idtr();
void init_idt();
void init()
{
	printf1("init_GDT......");
	init_gdt();
	printf1("[ok]\n");
	//中断
	printf1("init_8259A.....");
	init_8259A();
	printf1("[ok]\n");
	printf1("init_prot......");
	init_prot();
	printf1("[ok]\n");
	printf1("init_IDT......");
	reflash_idtr();
	printf1("[OK]");


	
}



void set_descriptor(DESCRIPTOR *p,u32 base,u32 limit,u16 attribute)
{
	p->limit_low	= limit & 0x0FFFF;
	p->base_low	= base & 0x0FFFF;
	p->base_mid	= (base >> 16) & 0x0FF;
	p->attr1		= attribute & 0xFF;
	p->limit_high_attr2= ((limit>>16) & 0x0F) | (attribute>>8) & 0xF0;
	p->base_high	= (base >> 24) & 0x0FF;
}




void init_gdt()
{
	//这里我只用把内存中的gdt保存到数组中即可，如果有更改GDT需要重新设置一下lgdt
	memcopy(&gdt,  (void*)(*((u32*)(&gdt_ptr[2]))),    *((u16*)(&gdt_ptr[0])) + 1);
}


void reflash_idtr()
{
	u16* limit = (u16*)(&idt_ptr[0]);
	u32* base  = (u32*)(&idt_ptr[2]);
	*limit = IDT_SIZE * sizeof(GATE) - 1;
	*base  = (u32)&idt;
	loadidt();
}


void reflash_gdtr()
{
	/* gdt_ptr[6] 0~15:Limit  16~47:Base。*/
	u16* limit = (u16*)(&gdt_ptr[0]);
	u32* base  = (u32*)(&gdt_ptr[2]);
	*limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*base  = (u32)&gdt;
	loadgdt();
}











