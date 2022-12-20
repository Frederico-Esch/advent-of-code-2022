#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int64_t cycles;
    int64_t reg;
} CPU;

CPU createCPU(){
    CPU cpu = {
        .reg = 1,
        .cycles = 0,
    };

    return cpu;
}

void cycle_render(CPU* cpu) {
    int pos = cpu->cycles%40 -1;
    if (pos == -1) pos += 40;

    if (cpu->reg == pos | cpu->reg + 1 == pos | cpu->reg - 1 == pos)
        printf("#");
    else
        printf(" ");

    if (pos == 39) printf("\n");
}

int main() {
    CPU cpu = createCPU();

    FILE* file = fopen("input", "r");

    char* cmd = malloc(4);
    int numero;
    while (!feof(file)) {
        fread(cmd, 1, 4, file);
        if (strcmp(cmd, "noop") == 0) {
            cpu.cycles++;
            cycle_render(&cpu);
            if (feof(file)) break;
            fseek(file, 1, SEEK_CUR);
        } else {
            fscanf(file, " %d", &numero);
            cpu.cycles ++;
            cycle_render(&cpu);
            cpu.cycles ++;
            cycle_render(&cpu); //alterar aqui
            cpu.reg += numero;
            if (feof(file)) break;
            fseek(file, 1, SEEK_CUR);
        }
    }

    free(cmd);
    fclose(file);
    return 0;
}
