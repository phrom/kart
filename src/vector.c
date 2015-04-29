#include "vector.h"

#include <stdlib.h>
#include <string.h>

struct vector {
    int elem_size;
    int size;
    int capacity;
    unsigned char begin[1];
};

struct vector* vector_new(int elem_size, int initial_capacity)
{
    struct vector* vector = (struct vector*)malloc(
        sizeof(struct vector) + initial_capacity * elem_size);
    vector->elem_size = elem_size;
    vector->size = 0;
    vector->capacity = initial_capacity;
    return vector;
}

void vector_free(struct vector* vector) { free(vector); }

static void vector_realloc(struct vector** vector, int new_capacity)
{
    *vector = (struct vector*)realloc(
        *vector,
        sizeof(struct vector) + new_capacity * (*vector)->elem_size);
    (*vector)->capacity = new_capacity;
}

static void vector_guarantee_space(struct vector** vector)
{
    if ((*vector)->size == (*vector)->capacity) {
        int new_capacity = (*vector)->capacity * 2;
        vector_realloc(vector, new_capacity);
    }
}

void vector_push_back(struct vector** vector, void* value)
{
    vector_guarantee_space(vector);
    memcpy((*vector)->begin + (*vector)->size * (*vector)->elem_size,
           value,
           (*vector)->elem_size);
    (*vector)->size += 1;
}

void vector_insert(struct vector** vector, int index, void* value)
{
    vector_guarantee_space(vector);
    memmove((*vector)->begin + (index + 1) * (*vector)->elem_size,
            (*vector)->begin + index * (*vector)->elem_size,
            ((*vector)->size - index) * (*vector)->elem_size);
    memcpy((*vector)->begin + index * (*vector)->elem_size,
           value,
           (*vector)->elem_size);
    (*vector)->size += 1;
}

void* vector_get(struct vector* vector, int index)
{
    return vector->begin + index * vector->elem_size;
}

int vector_size(const struct vector* vector) { return vector->size; }

int vector_capacity(const struct vector* vector)
{
    return vector->capacity;
}

void vector_sort(struct vector* vector,
                 int (*comp)(const void*, const void*))
{
    qsort(vector->begin, vector->size, vector->elem_size, comp);
}

void* vector_begin(struct vector* vector) { return vector->begin; }

void* vector_last(struct vector* vector)
{
    return vector->begin + (vector->size - 1) * vector->elem_size;
}

void* vector_end(struct vector* vector)
{
    return vector->begin + vector->size * vector->elem_size;
}

void* vector_grow(struct vector** vector)
{
    vector_guarantee_space(vector);
    (*vector)->size += 1;
    return vector_last(*vector);
}

int vector_bsearch(struct vector* vector,
                   const void* value,
                   int (*comp)(const void*, const void*))
{
    const int length = vector_size(vector);
    int low = 0, high = length, i = 0;
    while (low < high) {
        i = low + (high - low) / 2;
        if (comp(vector_get(vector, i), value) < 0) {
            low = i + 1;
        }
        else {
            high = i;
        }
    }
    if (comp(vector_get(vector, i), value) > 0) {
        i--;
    }
    return i;
}

void vector_foreach(struct vector* vector, void (*fn)(void*))
{
    int i;
    for (i = 0; i < vector->size; ++i) {
        void* value = vector_get(vector, i);
        fn(value);
    }
}

void vector_memcpy(struct vector** vector, const void* values, int size)
{
    if ((*vector)->capacity < size) {
        vector_realloc(vector, size);
    }
    memcpy((*vector)->begin, values, size * (*vector)->elem_size);
    (*vector)->size = size;
}
