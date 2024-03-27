/*
 * CS 261 PA4: Mini-ELF interpreter
 *
 * Name: Nick St. Amour
 */

#include "p4-interp.h"

/**********************************************************************
 *                         REQUIRED FUNCTIONS
 *********************************************************************/

y86_reg_t decode_execute (y86_t *cpu, y86_inst_t inst, bool *cnd, y86_reg_t *valA)
{
    y86_reg_t valE = 0;

    if (cnd == NULL || valA == NULL) {
        cpu->stat = INS;
        return 0;
    }
    
    

    int op = inst.icode;
    // cpu->pc = inst.valP;
    
    cpu->stat = 1;

    if (op == HALT) {
        cpu->stat = HLT;
    }

    

    return valE;
}

void memory_wb_pc (y86_t *cpu, y86_inst_t inst, byte_t *memory,
        bool cnd, y86_reg_t valA, y86_reg_t valE)
{
    switch (inst.icode) {
        case HLT:
            cpu->pc = inst.valP;
            break;
        default:
            cpu->pc = inst.valP;
            break;
    }
    
}

/**********************************************************************
 *                         OPTIONAL FUNCTIONS
 *********************************************************************/

void usage_p4 (char **argv)
{
    printf("Usage: %s <option(s)> mini-elf-file\n", argv[0]);
    printf(" Options are:\n");
    printf("  -h      Display usage\n");
    printf("  -H      Show the Mini-ELF header\n");
    printf("  -a      Show all with brief memory\n");
    printf("  -f      Show all with full memory\n");
    printf("  -s      Show the program headers\n");
    printf("  -m      Show the memory contents (brief)\n");
    printf("  -M      Show the memory contents (full)\n");
    printf("  -d      Disassemble code contents\n");
    printf("  -D      Disassemble data contents\n");
    printf("  -e      Execute program\n");
    printf("  -E      Execute program (trace mode)\n");
}

bool parse_command_line_p4 (int argc, char **argv,
        bool *header, bool *segments, bool *membrief, bool *memfull,
        bool *disas_code, bool *disas_data,
        bool *exec_normal, bool *exec_trace, char **filename)
{
    int c;
    while ((c = getopt(argc, argv, "hHafsmMdDeE")) != -1) {
        switch (c) {
            case 'h':
               
                usage_p4(argv);
                return true;
                
            case 'H':
                *header = true;
                break;
            case 'a':
                *membrief = true;
                *header = true;
                *segments = true;
                break;
            case 'f':
                if (*membrief) {
                    usage_p4(argv);
                    return false;
                }
                *memfull = true;
                *header = true;
                *segments = true;
                break;
            case 's':
                
                *segments = true;
                break;
            case 'm':
                // might need to delete print_segments == false
                if (*memfull == true) {
                    usage_p4(argv);
                    return false;
                }
                *membrief = true;
                break;
            case 'M':
                if (*membrief == true) {
                    usage_p4(argv);
                    return false;
                }
                *memfull = true;
                break;
            case 'd':
                *disas_code = true;
                break;
            case 'D':
                *disas_data = true;
                break; 
            case 'e':
                *exec_normal = true;
                break;
            case 'E':
                *exec_trace = true;
                break;
            /*
            default:
                return false;
            */  
        }
    }

    if (*exec_trace && *exec_normal) {
        usage_p4(argv);
        return false;
    }

    if (optind == argc - 1) {
        // how do we check if the filename is valid?
        *filename = argv[optind];
        return true;
    } else {
        usage_p4(argv);
        return false;
    }

    return true;

}

void dump_cpu_state (y86_t cpu)
{
    printf("Y86 CPU state:\n");
    printf("  %%rip: %016lx   flags: Z0 S0 O0     ", cpu.pc);
    if (cpu.stat == HLT) {
        printf("HLT\n");
    } else if (cpu.stat == AOK) {
        printf("AOK\n");
    }

    printf("  %%rax: %016lx    %%rcx: %016lx\n", 0, 0);
    printf("  %%rdx: %016lx    %%rbx: %016lx\n", 0, 0);
    printf("  %%rsp: %016lx    %%rbp: %016lx\n", 0, 0);
    printf("  %%rsi: %016lx    %%rdi: %016lx\n", 0, 0);
    printf("   %%r8: %016lx     %%r9: %016lx\n", 0, 0);
    printf("  %%r10: %016lx    %%r11: %016lx\n", 0, 0);
    printf("  %%r12: %016lx    %%r13: %016lx\n", 0, 0);
    printf("  %%r14: %016lx\n", 0);
}

