/*
 * CS 261: Main driver
 *
 * Name: Nick St. Amour
 */

#include "p1-check.h"
#include "p2-load.h"
#include "p3-disas.h"
#include "p4-interp.h"



int main (int argc, char **argv)
{
  
   
   
    char *filename = NULL;

    bool print_header = false; 
    bool print_segments = false;
    bool print_membrief = false; 
    bool print_memfull = false;
    bool disas_code = false;
    bool disas_data = false;
    bool exec_normal = false;
    bool exec_trace = false;
    elf_hdr_t hdr;

    bool parse = parse_command_line_p4(argc, argv, &print_header, &print_segments, &print_membrief, &print_memfull, 
    &disas_code, &disas_data, &exec_normal, &exec_trace, &filename);

    if (parse && filename == NULL) {
        return EXIT_SUCCESS;
    }
    
    if (!parse) {
        return EXIT_FAILURE;
    }

    FILE *f = fopen(filename, "r");

    // might change this
    if (!read_header(f, &hdr)) {
        return EXIT_FAILURE;
    }

    byte_t* memory = (byte_t*)calloc(MEMSIZE, 1);


    elf_phdr_t phdr[hdr.e_num_phdr];

    for (int i = 0; i < hdr.e_num_phdr; i++) {
        if (!read_phdr(f, 16 + (i * 20), &phdr[i])) {
            printf("Failed to read file\n");
            free(memory);
            return EXIT_FAILURE;
        }
        
    }


    for (int i = 0; i < hdr.e_num_phdr; i++) {
       if (phdr[i].p_offset + phdr[i].p_filesz > 4096 || !load_segment(f, memory, phdr[i])) {
            printf("Failed to read file\n");
            free(memory);
            return EXIT_FAILURE;
        } 
    }
   

    if (print_header) {
        dump_header(hdr);
    }

    

    

    if (print_segments) {
        dump_phdrs(hdr.e_num_phdr, phdr);
    }

    if (print_memfull) {
        dump_memory(memory, 0, 4096);
    }

    if (print_membrief) {
        for (int i = 0; i < hdr.e_num_phdr; i++) {
            dump_memory(memory, phdr[i].p_vaddr, phdr[i].p_vaddr + phdr[i].p_filesz);
        }
    }

    if (disas_code) {

        printf("Disassembly of executable contents:\n");
        printf("  0x100:                               | .pos 0x100 code\n");
        int i = 0;
        while (i < hdr.e_num_phdr) {
            disassemble_code(memory, &phdr[i], &hdr);
            i++;
        }
    }

    
 
    if (exec_normal) {

        printf("Beginning execution at 0x0100\n");

        y86_t cpu;
        
        bool cnd;
        y86_reg_t valA;

        cpu.pc = hdr.e_entry;

        int i = 0;

        cpu.stat = AOK;


        while (cpu.stat == AOK) {

            y86_inst_t inst = fetch(&cpu, memory);
        
            // printf("%d", inst.icode);
        
            y86_reg_t valE = decode_execute(&cpu, inst, &cnd, &valA);
            memory_wb_pc(&cpu, inst, &memory, cnd, valA, valE);
            i++;
        }


        

        
        dump_cpu_state(cpu);
        printf("Total execution count: %d\n", i);
    }

    if (exec_trace) {

        printf("Beginning execution at 0x0100\n");

        y86_t cpu;
        
        bool cnd;
        y86_reg_t valA;

        cpu.pc = hdr.e_entry;

        int i = 0;

        cpu.stat = AOK;


        while (cpu.stat == AOK) {

            dump_cpu_state(cpu);

            y86_inst_t inst = fetch(&cpu, memory);
        
            // printf("%d", inst.icode);
        
            y86_reg_t valE = decode_execute(&cpu, inst, &cnd, &valA);
            memory_wb_pc(&cpu, inst, &memory, cnd, valA, valE);
            i++;
            printf("\n");
            printf("Executing: ");
            switch (inst.icode) {
                case 0:
                printf("halt\n");
                break;
                case 1:
                printf("nop\n");
                break;

            }
        } 
        dump_cpu_state(cpu);
        
        printf("Total execution count: %d\n", i);

        printf("\n");
        dump_memory(memory, 0, 4096);
    }
    

    free(memory);
    
    fclose(f);

    return EXIT_SUCCESS;
}

