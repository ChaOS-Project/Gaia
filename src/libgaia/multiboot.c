/*
 * multiboot.c
 *
 *  Created on: 17/12/2011
 *      Author: piranna
 */


#include "multiboot.h"

#include <stdio.h>


/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)	((flags) & (bit))


void multiboot_print_info(unsigned long magic, unsigned long addr)
{
    multiboot_info_t* mbi;

    /* Am I booted by a Multiboot-compliant boot loader? */
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        printf("Invalid magic number: 0x%x\n", (unsigned)magic);
        return;
    }

    /* Set MBI to the address of the Multiboot information structure. */
    mbi = (multiboot_info_t*)addr;

    /* Print out the flags. */
    printf("flags = 0x%x\n", (unsigned) mbi->flags);

    /* Are mem_* valid? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MEMORY))
        printf("mem_lower = %uKB, mem_upper = %uKB\n",
           (unsigned)mbi->mem_lower, (unsigned)mbi->mem_upper);

    /* Is boot_device valid? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_BOOTDEV))
        printf("boot_device = 0x%x\n", (unsigned)mbi->boot_device);

    /* Is the command line passed? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_CMDLINE))
        printf("cmdline = %s\n", (char*)mbi->cmdline);

    /* Are mods_* valid? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MODS))
    {
        multiboot_module_t* mod;
        int i;

        printf("mods_count = %d, mods_addr = 0x%x\n",
               (int)mbi->mods_count, (int) mbi->mods_addr);
        for(i = 0, mod = (multiboot_module_t*)mbi->mods_addr;
            i < mbi->mods_count;
            i++, mod++)
            printf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                   (unsigned)mod->mod_start,
                   (unsigned)mod->mod_end,
                   (char*)mod->cmdline);
    }

    /* Bits 4 and 5 are mutually exclusive! */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_AOUT_SYMS)
    && CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_ELF_SHDR))
    {
        printf("Both bits 4 and 5 are set.\n");
        return;
    }

    /* Is the symbol table of a.out valid? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_AOUT_SYMS))
    {
        multiboot_aout_symbol_table_t* multiboot_aout_sym = &(mbi->u.aout_sym);

        printf("multiboot_aout_symbol_table: tabsize = 0x%0x, "
               "strsize = 0x%x, addr = 0x%x\n",
               (unsigned)multiboot_aout_sym->tabsize,
               (unsigned)multiboot_aout_sym->strsize,
               (unsigned)multiboot_aout_sym->addr);
    }

    /* Is the section header table of ELF valid? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_ELF_SHDR))
    {
        multiboot_elf_section_header_table_t* multiboot_elf_sec;
        multiboot_elf_sec = &(mbi->u.elf_sec);

        printf("multiboot_elf_sec: num = %u, size = 0x%x,"
               " addr = 0x%x, shndx = 0x%x\n",
               (unsigned)multiboot_elf_sec->num,
               (unsigned)multiboot_elf_sec->size,
               (unsigned)multiboot_elf_sec->addr,
               (unsigned)multiboot_elf_sec->shndx);
    }

    /* Are mmap_* valid? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MEM_MAP))
    {
        multiboot_memory_map_t* mmap;

        printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
               (unsigned)mbi->mmap_addr, (unsigned)mbi->mmap_length);

        for(mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
            (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
            mmap = (multiboot_memory_map_t*)((unsigned long)mmap
                                             + mmap->size + sizeof(mmap->size)))
            printf(" size = 0x%x, base_addr = 0x%x%x,"
                   " length = 0x%x%x, type = 0x%x\n",
                   (unsigned)mmap->size,
                   mmap->addr >> 32,
                   mmap->addr & 0xffffffff,
                   mmap->len >> 32,
                   mmap->len & 0xffffffff,
                   (unsigned)mmap->type);
    }

//    /* Is there drive info? */
//    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_DRIVE_INFO))
//    {
//    }

//    /* Is there a config table? */
//    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_CONFIG_TABLE))
//    {
//    }

    /* Is there a boot loader name? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_BOOT_LOADER_NAME))
        printf("bootloader = %s\n", (char*)mbi->boot_loader_name);

//    /* Is there a APM table? */
//    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_APM_TABLE))
//    {
//    }

    /* Is there video information? */
    if(CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_VIDEO_INFO))
    {
    	printf("vbe_control_info = 0x%x, vbe_mode_info = 0x%x\n",
    			mbi->vbe_control_info, mbi->vbe_mode_info);
    	printf("vbe_mode = 0x%x\n", mbi->vbe_mode);
    }
}
