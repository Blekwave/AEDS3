#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define STR_SIZE 50
#define TAG_SIZE 10

void trim_newline(char *str){
    while (*str) str++;
    if (*(--str) == '\n')
        *str = '\0';
}

bool is_pattern(char *text, char *tag){
    while (*text && *tag && toupper(*text) == toupper(*tag)){
        text++;
        tag++;
    }
    if (*tag == '\0')
        return true;
    return false;
}

void print_replaced(char *text, char *tag, int replace_val){
    bool in_tag = false;
    int tag_len = strlen(tag);
    char *pointer = text;
    while (*pointer != '\0'){
        if (in_tag && is_pattern(pointer, tag)){
            printf("%d", replace_val);
            pointer += tag_len - 1; // pointer++ still executed
        }
        else {
            if (*pointer == '<')
                in_tag = true;
            if (*pointer == '>')
                in_tag = false;
            printf("%c", *pointer);
        }
        pointer++;
    }
}

int main(int argc, char const *argv[])
{
    char tag[TAG_SIZE + 1], text[STR_SIZE + 1];
    int replace_val;
    while (fgets(tag, TAG_SIZE + 1, stdin)){
        trim_newline(tag);
        scanf("%d", &replace_val);
        getc(stdin);
        fgets(text, STR_SIZE + 1, stdin);
        trim_newline(text);
        print_replaced(text, tag, replace_val);
        printf("\n");
    }
    return 0;
}