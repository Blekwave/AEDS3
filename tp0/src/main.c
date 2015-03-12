#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "treap.h"

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]){
    Treap *treap = NULL;

    int instructions, key, pri;
    scanf("%d\n", &instructions);

    while (instructions > 0){
        char buffer[BUFFER_SIZE];
        fgets(buffer, BUFFER_SIZE, stdin);

        char *pch = strtok(buffer, " ");
        if (!strcmp(pch, "INSERT")){
            key = atoi(pch = strtok(NULL, " "));
            pri = atoi(pch = strtok(NULL, " "));
            treap = Tr_Insert(treap, key, pri);
        }
        else if (!strcmp(pch, "REMOVE")){
            key = atoi(pch = strtok(NULL, " "));
            treap = Tr_Remove(treap, key);
        }
        else if (!strcmp(pch, "LOCATE")){
            key = atoi(pch = strtok(NULL, " "));
            char directions[BUFFER_SIZE];
            int depth = -1;
            if (Tr_Locate(treap, key, directions, 0, &depth)){
                directions[depth] = '\0';
                printf("%s\n", directions);
            }
            else
                printf("-1\n");
        }
        else {
            fprintf(stderr, "%s is not a valid command.\n", pch);
        }
        instructions--;
    }

    Tr_Delete(treap);
    return 0;
}