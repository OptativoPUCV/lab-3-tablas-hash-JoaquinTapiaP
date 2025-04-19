#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair(char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash(char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    
    if (searchMap(map, key) != NULL) return;

    long index = hash(key, map->capacity);

    Pair * current = map->buckets[index];

    while (current != NULL && current->key != NULL) {
        current = map->buckets[index + 1];
    }

    if (current == NULL) {
        current = createPair(strdup(key), value);
    }
    else {
        current->key = strdup(key);
        current->value = value;
    }

    /*if ((float) (map->capacity / map->size >= 0.7)) {
        enlarge(map);
    }*/

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    long newCap = map->capacity * 2;
    Pair **newBuck = (Pair **)malloc(sizeof(Pair *) * newCap);
    
    for (long i = 0; i < newCap ; i++) {
        newBuck[i] = NULL;
    }

    for (long i = 0; i < map->capacity; i++) {
        Pair *current = map->buckets[i];
        if (current != NULL) {
    
            long index = hash(current->key, newCap);

            while (newBuck[index] != NULL) {
                index = (index + 1) % newCap;
            }

            newBuck[index] = createPair(current->key, current->value);
        }
    }

    free(map->buckets);

    map->buckets = newBuck;
    map->capacity = newCap;

}


HashMap * createMap(long capacity) {
    HashMap* nuevo = (HashMap*) malloc(sizeof(HashMap*));
    nuevo->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);

    nuevo->size = 0;
    nuevo->capacity = capacity;
    nuevo->current = -1;

    for (long i = 0; i < capacity; i++) {
        nuevo->buckets[i] = NULL;
    }

    return nuevo;
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {   


    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
