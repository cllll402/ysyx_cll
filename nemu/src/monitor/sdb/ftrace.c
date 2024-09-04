#include "ftrace.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <elf.h>
#include <memory/paddr.h>

static func_info elf_funcs[1024];
static int end = 0; // Initialize end variable
func_info* symbol_tbl = NULL; // Global variable initialization

void read_from_file(FILE *elf, size_t offset, size_t size, void* dest) {
    fseek(elf, offset, SEEK_SET);
    size_t flag = fread(dest, size, 1, elf);
    assert(flag == 1);
}

void get_str_from_file(FILE *elf, size_t offset, size_t n, void* dest) {
    fseek(elf, offset, SEEK_SET);
    char* flag = fgets(dest, n, elf);
    assert(flag != NULL);
}

void init_elf(const char* elf_file, paddr_t global_offset) {
    FILE *elf = fopen(elf_file, "rb");
    assert(elf != NULL); // Check if file opened successfully

    Elf32_Ehdr elf_header;
    read_from_file(elf, global_offset, sizeof(elf_header), &elf_header);

    // Print ELF header information
    printf("====== ELF Header ======\n");
    printf("Magic:	");
    for (int i = 0; i < EI_NIDENT; i++) {
        printf("%02x ", elf_header.e_ident[i]);
    }
    printf("\n");
    printf("\n");
        
    printf("Start of section headers:	%04u\n", elf_header.e_shoff);
    printf("Number of section headers: 	%04u\n", elf_header.e_shnum);
    printf("Size of section headers:  	%04u\n", elf_header.e_shentsize);
    printf("Start of program headers: 	%04u\n", elf_header.e_phoff);
    printf("Number of program headers: 	%04u\n", elf_header.e_phnum);
    printf("Size of program headers:  	%04u\n", elf_header.e_phentsize);
    printf("\n");
    
    Elf32_Off section_header_offset = elf_header.e_shoff;
    uint64_t headers_entry_size = elf_header.e_shentsize;
    uint64_t headers_entry_num = elf_header.e_shnum;

    assert(sizeof(Elf32_Shdr) == headers_entry_size);

    Elf32_Off symbol_table_offset = 0, string_table_offset = 0;
    size_t symbol_table_total_size = 0;
    size_t symbol_table_entry_size = 0;

    // Iterate through section headers
    for (int i = 0; i < headers_entry_num; ++i) {
        Elf32_Shdr section_entry;
        read_from_file(elf, global_offset + section_header_offset + i * headers_entry_size, headers_entry_size, &section_entry);
        
        switch (section_entry.sh_type) {
            case SHT_SYMTAB:
                symbol_table_offset = section_entry.sh_offset;
                symbol_table_total_size = section_entry.sh_size;
                symbol_table_entry_size = section_entry.sh_entsize;
                break;
            case SHT_STRTAB:
                if (i != elf_header.e_shstrndx) {
                    string_table_offset = section_entry.sh_offset;
                }
                break;
        }

        // Print section information
        printf("Section %d: Type: %010u | Offset: %04u | Size: %04u | Name: %#04x\n", 
               i, section_entry.sh_type, section_entry.sh_offset, section_entry.sh_size, section_entry.sh_name);
    }
    printf("\n");
    
    char function_name[64];
    assert(symbol_table_entry_size == sizeof(Elf32_Sym));

    // Read symbols from the symbol table
    for (int i = 0; i < symbol_table_total_size / symbol_table_entry_size; ++i) {
        Elf32_Sym symbol_section_entry;
        read_from_file(elf, global_offset + symbol_table_offset + i * symbol_table_entry_size, symbol_table_entry_size, &symbol_section_entry);
        
        if (ELF32_ST_TYPE(symbol_section_entry.st_info) == STT_FUNC) {
            get_str_from_file(elf, global_offset + string_table_offset + symbol_section_entry.st_name, sizeof(function_name), function_name);
            strncpy(elf_funcs[end].func_name, function_name, sizeof(elf_funcs[0].func_name));
            elf_funcs[end].start = symbol_section_entry.st_value;
            elf_funcs[end].size = symbol_section_entry.st_size;
            end++;
        }
    }
    fclose(elf);

    // Initialize symbol_tbl
    symbol_tbl = elf_funcs;

    // Print the symbol table
    printf("====== Symbol Table ======\n");
    for (int i = 0; i < end; ++i) {
        func_info *info = &elf_funcs[i];
        printf("Func: %12s | Start: %#" PRIx64 " | Size: %04zu\n", info->func_name, info->start, info->size);
    }
}

func_info* check_func(paddr_t addr) {
    for (int i = 0; i < end; ++i) {
        func_info *info = &elf_funcs[i];
        if (addr >= info->start && addr < info->start + info->size) {
            return info;
        }
    }
    return NULL;
}

void trace_func_call(paddr_t pc, paddr_t target) {
    if (symbol_tbl == NULL) return;

    func_info* func = check_func(target);
    if (func != NULL) {
        Log("Called function: %12s, PC: 0x%" PRIxPTR "\n", func->func_name, (uintptr_t)pc);
    } else {
        Log("\033[31mCalled unknown function: %12s, PC: 0x%" PRIxPTR "\033[0m\n", "UNKNOWN", (uintptr_t)pc);
    }
}

void trace_func_ret(paddr_t pc) {
    if (symbol_tbl == NULL) return;

    func_info* func = check_func(pc);
    if (func != NULL) {
        Log("Returned to function: %12s, PC: 0x%" PRIxPTR "\n", func->func_name, (uintptr_t)pc);
    } else {
        Log("\033[31mReturned from unknown function: %12s, PC: 0x%" PRIxPTR "\033[0m\n", "UNKNOWN", (uintptr_t)pc);
    }
}
