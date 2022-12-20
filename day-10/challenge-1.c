#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int64_t cycles;
    int64_t reg;
    int64_t signals;

} CPU;

CPU createCPU(){
    CPU cpu = {
        .reg = 1,
        .cycles = 0,

        .signals = 0,
    };

    return cpu;
}

void verify_key_point(CPU* cpu) {
    int strength = -1;
    switch (cpu->cycles) {
        case 20:
        case 60:
        case 100:
        case 140:
        case 180:
        case 220:
            cpu->signals += cpu->reg*cpu->cycles;
            break;
    }
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
            verify_key_point(&cpu);
            if (feof(file)) break;
            fseek(file, 1, SEEK_CUR);
        } else {
            fscanf(file, " %d", &numero);
            printf("%d\n", numero);
            cpu.cycles ++;
            verify_key_point(&cpu);
            cpu.cycles ++;
            verify_key_point(&cpu); //alterar aqui
            cpu.reg += numero;
            if (feof(file)) break;
            fseek(file, 1, SEEK_CUR);
        }
    }

    printf("%ld\n", cpu.signals);
    free(cmd);
    fclose(file);
    return 0;
}
