#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct lenc {
    int id;
    int size;
    struct lenc *next;
} Lenc;

Lenc *Lenc_InitNode(int id, int size){
    Lenc *new = malloc(sizeof(Lenc));
    new->id = id;
    new->size = size;
    new->next = NULL;
    return new;
}

void Lenc_DelNode(Lenc *node){
    free(node);
}

Lenc *Lenc_Init(){
    Lenc *new = malloc(sizeof(Lenc));
    new->id = -1;
    new->next = NULL;
    return new;
}

void Lenc_Destroy(Lenc *list){
    Lenc *prev;
    while (list){
        prev = list;
        free(list);
        list = prev->next;
    }
}

Lenc *Arrival(Lenc *lot, int id, int size){
    Lenc *prev = lot;
    while ((lot = lot->next)){
        if (lot->id == 0 && size <= lot->size){
            Lenc *new = Lenc_InitNode(id, size);
            prev->next = new;
            new->next = lot;
            lot->size -= size;
            lot = new;
            break;
        }
        prev = lot;
    }
    return lot;
}

void Departure(Lenc *lot, int id){
    Lenc *prev = lot;
    while ((lot = lot->next)){
        if (lot->id == id){
            lot->id = 0;
            if (lot->next->id == 0){
                Lenc *next = lot->next;
                lot->size += next->size;
                lot->next = next->next;
                Lenc_DelNode(next);
            }
            if (prev->id == 0){
                prev->size += lot->size;
                prev->next = lot->next;
                Lenc_DelNode(lot);
            }
            break;
        }
        prev = lot;
    }
}


#define PARK_PRICE 10
#define BUFFER_SIZE 128

int main(){
    int length, events;
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE - 1, stdin)){
        length = atoi(strtok(buffer, " "));
        events = atoi(strtok(NULL, " "));
        Lenc *lot = Lenc_Init();
        lot->next = Lenc_InitNode(0, length); // Empty space!
        int price = 0, id, size;
        char command;
        while (events--){
            fgets(buffer, BUFFER_SIZE - 1, stdin);
            command = *strtok(buffer, " ");
            id = atoi(strtok(NULL, " "));

            if (command == 'C'){
                size = atoi(strtok(NULL, " "));
                if (Arrival(lot, id, size))
                    price += PARK_PRICE;
            } else if (command == 'S') {
                Departure(lot, id);
            }
        }
        Lenc_Destroy(lot);
        printf("%d\n", price);
    }
    return 0;
}