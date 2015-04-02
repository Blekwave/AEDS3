#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define STR_SIZE 201
#define TAG_SIZE 10
#define VAL_SIZE 4

char is_pattern(char *text, char *tag){
    while (*text && *tag && toupper(*text) == toupper(*tag)){
        text++;
        tag++;
    }
    if (*tag == '\0')
        return 1;
    return 0;
}

void replace(char *text, char *tag, char *replace_val){
    char in_tag = 0;
    int i;
    int text_len = strlen(text);
    int tag_len = strlen(tag);
    int val_len = strlen(replace_val);
    char *pointer = text;
    while (*pointer){
        if (in_tag && is_pattern(pointer, tag)){
            memmove(pointer + val_len, pointer + tag_len,
                text_len - (pointer - text) + 1); // '\0' is copied as well!
            for (i = 0; i < val_len; i++)
                pointer[i] = replace_val[i];
            pointer += val_len;
        }
        else {
            if (*pointer == '<')
                in_tag = 1;
            if (*pointer == '>')
                in_tag = 0;
            pointer++;
        }
    }
}

int main(int argc, char const *argv[])
{
    char tag[TAG_SIZE + 1], text[STR_SIZE + 1], replace_val[VAL_SIZE + 1];
    while (gets(tag)){
        gets(replace_val);
        gets(text);
        replace(text, tag, replace_val);
        printf("%s\n", text);
    }
    return 0;
}