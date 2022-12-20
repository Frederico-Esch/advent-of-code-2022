#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SET_SIZE 1000000
#define OFFSET 1500
char set[SET_SIZE];

struct {
    int Hx, Hy, Tx[9], Ty[9];
} ponte;

void initialize(){
    for (int i = 0; i < SET_SIZE; i++){
        set[i] = 0;
    }

    ponte.Hx = 0;
    ponte.Hy = 0;
    for (int i = 0; i < 9; i++) {
        ponte.Tx[i] = 0;
        ponte.Ty[i] = 0;
    }
}

void adjust_ponte() {
    for (int i = 0; i < 9; i++) {
        if(i == 0) {
            if (abs(ponte.Hx - ponte.Tx[0]) > 1) {
                if (ponte.Hy - ponte.Ty[0] != 0) { //coluna diferente
                    ponte.Ty[0] = ponte.Hy;
                }
                ponte.Tx[0] += (ponte.Hx-ponte.Tx[0])/2;
            }
            if (abs(ponte.Hy - ponte.Ty[0]) > 1) {
                if(ponte.Hx - ponte.Tx[0] != 0) { //linha diferente
                    ponte.Tx[0] = ponte.Hx;
                }
                ponte.Ty[0] += (ponte.Hy - ponte.Ty[0])/2;
            }
        } else {
            if (abs(ponte.Tx[i-1] - ponte.Tx[i]) > 1 && abs(ponte.Ty[i-1] - ponte.Ty[i]) > 1) {
                ponte.Tx[i] += (ponte.Tx[i-1]-ponte.Tx[i])/2;
                ponte.Ty[i] += (ponte.Ty[i-1] - ponte.Ty[i])/2;
            }
            else if (abs(ponte.Tx[i-1] - ponte.Tx[i]) > 1) {
                if (ponte.Ty[i-1] - ponte.Ty[i] != 0) { //coluna diferente
                    ponte.Ty[i] = ponte.Ty[i-1];
                }
                ponte.Tx[i] += (ponte.Tx[i-1]-ponte.Tx[i])/2;
            }
            else if (abs(ponte.Ty[i-1] - ponte.Ty[i]) > 1) {
                if(ponte.Tx[i-1] - ponte.Tx[i] != 0) { //linha diferente
                    ponte.Tx[i] = ponte.Tx[i-1];
                }
                ponte.Ty[i] += (ponte.Ty[i-1] - ponte.Ty[i])/2;
            }
        }
    }
    int index = (ponte.Ty[8] + OFFSET)*OFFSET + (ponte.Tx[8] + OFFSET + 500);
    set[index/8] |= (1 << index%8);
    printf("Index %d\n", index/8);

}

int main() {
    FILE* file = fopen("input", "r");
    char cmd;
    int amount;

    initialize();
    while (!feof(file)) {
        fscanf(file, "%c %d", &cmd, &amount);

        while (amount) {
            switch (cmd) {
                case 'D':
                    ponte.Hy--;
                    break;
                case 'U':
                    ponte.Hy++;
                    break;
                case 'R':
                    ponte.Hx++;
                    break;
                case 'L':
                    ponte.Hx--;
                    break;
            }
            adjust_ponte();
            amount--;
        }

        if(feof(file)) break;
        fseek(file, 1, SEEK_CUR);
    }

    int sum = 0;
    for (int i = 0; i < SET_SIZE; i++){
        for (int j = 0; j < 8; j++){
            if(((set[i] >> j) & 1) == 1) sum++;
        }
    }

    printf("%d\n", sum);
    fclose(file);
    return 0;
}
