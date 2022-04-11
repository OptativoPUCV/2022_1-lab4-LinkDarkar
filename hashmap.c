#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap
{
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value)
{
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity)
{
    unsigned long hash = 0;
    char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++)
    {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2)
{
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value)
{
    long index = hash(key , map->capacity);
    while (map->buckets[index] != NULL && map->buckets[index]->key != NULL)
    {
        if (is_equal(key , map->buckets[index]->key) == 1)
        {
            return;
        }
        index = (index + 1) % map->capacity;
    }

    if (map->buckets[index] != NULL)
    {
        map->buckets[index]->key = key;
        map->buckets[index]->value = value;
    }
    else
    {
        map->buckets[index] = createPair(key , value);
    }
    map->size += 1;
}

void enlarge(HashMap * map)
{
    enlarge_called = 1;
    Pair ** oldBuckets;
    oldBuckets = map->buckets;
    long oldCapacity = map->capacity;
    //free(map);
    map = createMap(oldCapacity * 2);
    for (long index = 0 ; index < oldCapacity ; index += 1)
    {
        if (oldBuckets[index] != NULL && oldBuckets[index]->key != NULL)
        {
            insertMap(map , oldBuckets[index]->key , oldBuckets[index]->value);
        }
    }
}

HashMap * createMap(long capacity)
{
    HashMap* array = (HashMap *) malloc (sizeof(HashMap));
    array->buckets = (Pair **) calloc (capacity , sizeof(HashMap));
    array->current = -1;
    array->capacity = capacity;
    array->size = 0;

    return array;
}

void eraseMap(HashMap * map,  char * key)
{    
    long index = hash(key , map->capacity);
    while (map->buckets[index] != NULL && map->buckets[index]->key != NULL)
    {
        if (is_equal(key , map->buckets[index]->key) == 1)
        {
            map->buckets[index]->key = NULL;
            map->size -= 1;
            break;
        }
        index = (index + 1) % map->capacity;
    }
}

Pair * searchMap(HashMap * map,  char * key)
{   
    long index = hash(key , map->capacity);
    while (map->buckets[index] != NULL && map->buckets[index]->key != NULL)
    {
        if (is_equal(key , map->buckets[index]->key) == 1)
        {
            map->current = index;
            return map->buckets[index];
        }
        index = (index + 1) % map->capacity;
        if (map->buckets[index] == NULL)
        {
            return NULL;
        }
    }

    return NULL;
}

Pair * firstMap(HashMap * map)
{
    for (int cont = 0 ; cont < map->capacity - 1 ; cont = ((cont + 1) % map->capacity))
    {
        if (map->buckets[cont] != NULL && map->buckets[cont]->key != NULL)
        {
            map->current = cont;
            return map->buckets[cont];
        }
    }

    return NULL;
}

Pair * nextMap(HashMap * map)
{
    for (long index = map->current + 1 ; index < map->capacity - 1 ; index = ((index + 1) % map->capacity))
    {
        if (map->buckets[index] != NULL && map->buckets[index]->key != NULL)
        {
            map->current = index;
            return map->buckets[index];
        }
    }

    return NULL;
}
