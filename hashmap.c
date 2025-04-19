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
    /*
        if (map->size + 1 > map->capacity * 0.70){
         enlarge(map);
         //al parecer no es necesario en este caso como en la actividad en clases
    }
    */
    long posicion = hash(key, map->capacity); //posicion sera el resultado de la funcion hash, lo que nos dara donde se ubica en el arreglo
    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key!= NULL){ //mientras que la casilla sea distinta de NULL o que no este vacia
        if (is_equal(map->buckets[posicion]->key, key)) { //si ya existe la clave, no hagas nada
            map->buckets[posicion]->value = value;
            return;
        }
        posicion = (posicion + 1) % map->capacity; //aumenta la posicion para seguir avanzando
    }
    Pair * par =createPair(key, value); //se crean 
    map->size++;
    map->buckets[posicion] = par;
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
    long index = hash(key, map->capacity);
    while (map->buckets[index] != NULL) {
        if (is_equal(map->buckets[index]->key, key)){
            map->buckets[index]->key = NULL;
            map->buckets[index]->value = NULL;
            map->size--;
            return;
        }
        index = (index + 1) % map->capacity;
    }

}

Pair * searchMap(HashMap * map,  char * key) {   
    long index = hash(key, map->capacity);
    while (map->buckets[index] != NULL) {
        if (map->buckets[index]->key != NULL && is_equal(map->buckets[index]->key, key)) {
            map->current = index;
            return map->buckets[index];
        }
        index = (index + 1) % map->capacity;
    }

    return NULL;
}

Pair * firstMap(HashMap * map) {
    long index = 0;
    while (map->buckets[index] != NULL) {
        if (map->buckets[index]->key != NULL) {
            map->current = index;
            return map->buckets[index];
        }
        index++;
    }

    return NULL;
}

Pair * nextMap(HashMap * map) {
    long index = map->current + 1;

    while (index < map->capacity) {
        if (map->buckets[index] != NULL && map->buckets[index]->key != NULL) {
            map->current = index; // guardar el nuevo índice
            return map->buckets[index];
        }
        index++;
    }
    return NULL;
}
