#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<math.h>

//GAVE UP

char* read_line(FILE* file){
    int start = ftell(file);
    //find the end of the line or file
    while(!feof(file) && fgetc(file) != '\n' );
    int end = ftell(file); //position after '\n'
    if (!feof(file)) end -= 2;
    else if (end-start < 2) return NULL;

    char* line = malloc(end-start + 1); // 5 - 1 -> 4
    fseek(file, start, SEEK_SET);
    fread(line, end-start, 1, file); // 5
    if(!feof(file)) fgetc(file); //skip next '\n'
    line[end-start] = '\0';
    return line;
}

int next_element(char* line){
    int index = 0;
    while(line[index] != ']' && line[index] != ',' && line[index] && line[index] != '[') index++;
    return index;
}

typedef struct {

    int size;
    int end;

} LIST_SIZE;

LIST_SIZE size_list(char* list){
    if(list[1] == ']') {
        LIST_SIZE result = {
            .size = 0,
            .end = 2,
        };
        return result;
    }

    int size = 1;
    int index = 0;
    int depth = 0;
    do {
        if(list[index] == ',') size++;
        if(list[index] == ']') depth--;
        if(list[index] == '[') depth++;
        index++;
    }while(depth > 0);

    LIST_SIZE result = {
        .size = size,
        .end = index
    };
    return result;
}

int compare_items(char* lhs, char* rhs){
    //only comparing numbers right now
    int right_index = 1, left_index = 1, index;
    while(rhs[right_index] != '\0' && lhs[left_index] != '\0'){
        int left_value = -1, right_value = -1;
        int left_size = 0, right_size = 0;

        #pragma region left dealing
        index = next_element(&lhs[left_index]) + left_index;
        if(lhs[index] == '[' || lhs[index+1] == '[') {
            printf("ELEMENT: LIST ");
            LIST_SIZE size = size_list(&lhs[index]);
            left_size = size.size;
            left_index += size.end;
            printf("SIZE %d\n", left_size);
        }
        else if(lhs[index] == ','){
            printf("ELEMENT: NUMBER = ");
            left_value = 0;
            for(int i = index-1; i >= left_index; i--) left_value += (lhs[i] - '0')*(pow(10, index-left_index-1));
            printf("%d\n", left_value);
            left_index = index + 1;
        }else if(lhs[index] == ']'){
            printf("LEFT RAN OUT OF ITEMS: INPUT IS CORRECT\n");
            return 1; //left run out so it's correct
        }
        #pragma endregion

        #pragma region right dealing
        index = next_element(&rhs[right_index]) + right_index;
        if(rhs[index] == '[' || rhs[index+1] == '['){
            printf("ELEMENT: LIST ");
            LIST_SIZE size = size_list(&rhs[index]);
            right_size = size.size;
            right_index += size.end;
            printf("SIZE %d\n", right_size);
        }
        else if(rhs[index] == ','){
            printf("ELEMENT: NUMBER = ");
            right_value = 0;
            for(int i = index-1; i >= right_index; i--) right_value += (rhs[i] - '0')*(pow(10, index-right_index-1));
            printf("%d\n", right_value);
            right_index = index + 1;
        }
        else if(rhs[index] == ']'){
            printf("RIGHT RAN OUT OF ITEMS: INPUT IS INCORRECT\n");
            return 0;
        }
        #pragma endregion

        if(right_value > -1 && left_value > -1){
            if(left_index > right_index)
                return 0;
        }
        else {
            if(left_value > -1 || right_value > -1){
                if (left_value > -1)
                    left_size = 1;
                else
                    right_size = 1;
            }

            if (left_size > right_size){
                printf("RIGHT SUB LIST RAN OUT OF ELEMENTS FIRST: INPUT IS INCORRECT\n");
                return 0;
            }
        }
    }
    return 1;
}

int main(void){
    FILE* file = fopen("test.txt", "r"); //"abc" -> 'a' + 'b' + 'c' + '\0'
    char* line[2];
    int pair = 0, sum = 0;

    while(pair++ || 1) {
        line[0] = read_line(file);
        if(line[0] == NULL) break;
        line[1] = read_line(file);
        fgetc(file); //throw end of pair

        //print
        printf("PAIR: %d\n", pair);
        printf("LINE 0: %s\n", line[0]);
        printf("LINE 1: %s\n\n", line[1]);
        if(compare_items(line[0], line[1])){
            sum += pair;
        }
        printf("SUM = %d\n\n", sum);
        free(line[0]);
        free(line[1]);
    }

    fclose(file);
    return 0;
}