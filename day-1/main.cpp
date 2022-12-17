#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

#define SZ 3

int main(){
    int max[SZ] = { -1, -1, -1 };
    std::string input;
    std::ifstream file("./input", std::ios::in);
    int current = 0;
    while(std::getline(file, input, '\n')){
        if(input == ""){
            for( int i = 0; i < SZ; i++ ){
                if (current > max[i]){
                    std::swap(current, max[i]);
                }
            }
            current = 0;
            continue;
        }
        try {
            current += std::stoi(input);
        } catch (std::exception e) {
            printf("ERROR\n");
            return -1;
        }
    }

    int sum = 0;
    for(int i = 0; i < SZ; i++){
        printf("%d max: %d\n", i, max[i]);
        sum += max[i];
    }
    printf("SUM: %d\n", sum);
}
