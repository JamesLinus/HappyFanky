#include "multiboot.h"
#include "x.h"
#include "type.h"
#include "protect.h"

//cheak函数需要保存硬盘和内存信息，还没做！！！！！！
/* Check if the bit BIT in FLAGS is set.  */
#define CHECK_FLAG(flags,bit)	((flags) & (1 << (bit)))

extern DESCRIPTOR gdt[128];
void cheak(unsigned long magic, unsigned long addr);
extern void init();

void k()
{
	printf1("                 i am k !\n");
	printf1("@#$%^&(*&^%$@#$%^*()(*&^%$##$^&*(%@#$%^&*(*&%$$%^&$%#@#$%^&*((*&^%$#$%^&\n");

}
void cinit()
{
	printf1("now init system!\n");
	init();
	printf1(" \n                       loading......\n\n");
	
}

void hh()
{
	printf1("*************** CPU HIT*************\n");
}




//最后需要这个函数保存 硬盘和内存信息！！！
void cheak(unsigned long magic, unsigned long addr)
{
  //清屏
  cls ();
  multiboot_info_t *mbi =(multiboot_info_t *) addr;;
  /* 引导装载器是否符合 Multiboot 规范？ */
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
     printf1 ("Invalid magic number: 0x%x\n", (unsigned) magic);
      return;
    }

  /* 输出标志。 */
  printf1 ("flags = 0x%x\n", (unsigned) mbi->flags);

  /* mem_* 是否有效？ */
  if (CHECK_FLAG (mbi->flags, 0))
    printf1 ("mem_lower = %uKB, mem_upper = %uKB\n",
            (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

  /* boot_device 是否有效？ */
  if (CHECK_FLAG (mbi->flags, 1))
    printf1 ("boot_device = 0x%x\n", (unsigned) mbi->boot_device);

  /* 是否有命令行参数？ */
  if (CHECK_FLAG (mbi->flags, 2))
    printf1 ("cmdline = %s\n", (char *) mbi->cmdline);

  /* mods_* 是否有效？ */
  if (CHECK_FLAG (mbi->flags, 3))
    {
      module_t *mod;
      int i;

      printf1("mods_count = %d, mods_addr = 0x%x\n",
              (int) mbi->mods_count, (int) mbi->mods_addr);
      for (i = 0, mod = (module_t *) mbi->mods_addr;
           i < mbi->mods_count;
           i++, mod += sizeof (module_t))
        printf1(" mod_start = 0x%x, mod_end = 0x%x, string = %s\n",
                (unsigned) mod->mod_start,
                (unsigned) mod->mod_end,
                (char *) mod->string);
    }

  /* 第 4 位和第 5 位是互斥的！ */
  if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
    {
      printf1 ("Both bits 4 and 5 are set.\n");
      return;
    }

  /* 是否有 a.out 符号表？ */
  if (CHECK_FLAG (mbi->flags, 4))
    {
      aout_symbol_table_t *aout_sym = &(mbi->u.aout_sym);

      printf1 ("aout_symbol_table: tabsize = 0x%0x, "
              "strsize = 0x%x, addr = 0x%x\n",
              (unsigned) aout_sym->tabsize,
              (unsigned) aout_sym->strsize,
              (unsigned) aout_sym->addr);
    }

  /* 是否有 ELF section header table？ */
  if (CHECK_FLAG (mbi->flags, 5))
    {
      elf_section_header_table_t *elf_sec = &(mbi->u.elf_sec);

      printf1 ("elf_sec: num = %u, size = 0x%x,"
              " addr = 0x%x, shndx = 0x%x\n",
              (unsigned) elf_sec->num, (unsigned) elf_sec->size,
              (unsigned) elf_sec->addr, (unsigned) elf_sec->shndx);
    }

  /* mmap_* 是否有效？ */
  if (CHECK_FLAG (mbi->flags, 6))
    {
      memory_map_t *mmap;

      printf1 ("mmap_addr = 0x%x, mmap_length = 0x%x\n",(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
      for (mmap = (memory_map_t *) mbi->mmap_addr; (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
           mmap = (memory_map_t *) ((unsigned long) mmap+ mmap->size + sizeof (mmap->size)))
      {
        printf1 (" size = 0x%x, base_addr = 0x%x%x,"
		" length = 0x%x%x, type = 0x%x\n",
                (unsigned) mmap->size,
                (unsigned) mmap->base_addr_high,
                (unsigned) mmap->base_addr_low,
                (unsigned) mmap->length_high,
                (unsigned) mmap->length_low,
                (unsigned) mmap->type);
	//保存内存信息
      }
    }
}
 







