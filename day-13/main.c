#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include<stdint.h>
#include<stdbool.h>

#define OUTFILE

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

typedef enum {

    ET_NUMBER,
    ET_SUBLIST,
    ET_END

} ElementType;


struct _ListElement {

    ElementType type;
    union
    {
        struct _ListElement* sublist;
        int number;
    };
    struct _ListElement* next;

};
typedef struct _ListElement ListElement;

void wrap_in_sublist(ListElement* element){
    int number = element->number;
    ListElement* subelement = malloc(sizeof(ListElement));

    element->type = ET_SUBLIST;
    element->sublist = subelement;

    subelement->type = ET_NUMBER;
    subelement->number = number;
    subelement->next = malloc(sizeof(ListElement));

    subelement->next->type = ET_END;
    subelement->next->next = NULL;

}

int compare(ListElement* left, ListElement* right){
    while (left->type != ET_END || right->type != ET_END)
    {
        if (left->type == ET_END) return true;
        if (right->type == ET_END) return false;

        if(left->type == ET_NUMBER && right->type == ET_NUMBER){
            if (left->number < right->number) return true;
            if (right->number < left->number) return false;
        }else {
            if (left->type == ET_NUMBER){
                wrap_in_sublist(left);
            }else if(right->type == ET_NUMBER){
                wrap_in_sublist(right);
            }
            int result = compare(left->sublist, right->sublist);
            if (result == true || result == false)
                return result;
        }

        left = left->next;
        right = right->next;
    }
    return 2;
}

int parse_int(char* string, size_t* offset){
    int index = 0;
    int value = 0;
    while(string[index] != ',' && string[index] != ']') {

        if(isalpha(string[index+1])){
            value += (string[index] - '0') * 10;
        }else {
            value += (string[index] - '0');
        }
        index++;
    }
    *offset += index;
    return value;
}

int isalpha(char n){
    return n <= '9' && n >= '0';
}

ListElement* parse_lista(char* line, size_t* offset){
    size_t index = 1;
    ListElement* root = malloc(sizeof(ListElement));

    ListElement* current = root;
    while (line[index] != ']' )
    {
        if(line[index] == ',') index++;

        if(isalpha(line[index])){
            current->number = parse_int(&line[index], &index);
            current->type = ET_NUMBER;
            current->next = malloc(sizeof(ListElement));
            current = current->next;
        }else if(line[index] == '[') {
            current->type = ET_SUBLIST;
            current->sublist = parse_lista(&line[index], &index);
            current->next = malloc(sizeof(ListElement));
            current = current->next;
        }
        else {
            index++;
        }
    }
    if(offset != NULL) *offset += index+1;
    current->type = ET_END;
    current->next = NULL;
    return root;
}

void print_lista(ListElement* lista){
    printf("[");
    while(lista->type != ET_END){
        if(lista->type == ET_NUMBER){
            printf("%d", lista->number);
        }else if(lista->type == ET_SUBLIST){
            print_lista(lista->sublist);
        }
        lista = lista->next;
        if(lista->type != ET_END) printf(",");
    }
    printf("]");
}

int main(void){
    FILE* file = fopen("input.txt", "r"); //"abc" -> 'a' + 'b' + 'c' + '\0'
    #ifdef OUTFILE
    FILE* saida = fopen("saida.txt", "w");
    #endif 
    char* line[2];
    ListElement* listas[2];
    int pair = 0, sum = 0;

    while(pair++ || 1) {
        line[0] = read_line(file);
        if(line[0] == NULL) break;
        line[1] = read_line(file);
        fgetc(file); //throw end of pair

        printf("pair %d\n", pair);
        //printf("STRING:\n");
        printf("LISTA 0: %s\n", line[0]);
        printf("LISTA 1: %s\n", line[1]);
        listas[0] = parse_lista(line[0], NULL);
        listas[1] = parse_lista(line[1], NULL);
        //printf("BUILT:\n");
        //printf("LISTA 0: ");
        //print_lista(listas[0]); putchar('\n');
        //printf("LISTA 1: ");
        //print_lista(listas[1]); putchar('\n'); 
        int comparison = compare(listas[0], listas[1]);
        if (comparison != 0){
            printf("RESULT = TRUE\n");
            sum += pair;
        }else {
            printf("RESULT = FALSE\n");
        }
        printf("SUM = %d\n", sum);

        putchar('\n');

        #ifdef OUTFILE
        fprintf(saida, "pair %d RESULT %d\n", pair, comparison);
        #endif
        free(line[0]);
        free(line[1]);
    }

    #ifdef OUTFILE
    fclose(saida);
    #endif OUTFILE
    fclose(file);
    return 0;
}
