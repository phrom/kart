#ifndef VECTOR_H_
#define VECTOR_H_

struct vector;

struct vector* vector_new(int elem_size, int initial_capacity);
void vector_free(struct vector* vector);
void vector_push_back(struct vector** vector, void* value);
void vector_insert(struct vector** vector, int index, void* value);
void* vector_get(struct vector* vector, int index);
int vector_size(const struct vector* vector);
int vector_capacity(const struct vector* vector);
void vector_sort(struct vector* vector,
                 int (*comp)(const void*, const void*));
void* vector_begin(struct vector* vector);
void* vector_last(struct vector* vector);
void* vector_end(struct vector* vector);
void* vector_grow(struct vector** vector);
int vector_bsearch(struct vector* vector,
                   const void* value,
                   int (*comp)(const void*, const void*));
void vector_foreach(struct vector* vector, void (*fn)(void*));
void vector_memcpy(struct vector** vector, const void* values, int size);

#define VECTOR_FOREACH(vector, fn)                                        \
    vector_foreach(vector, (void (*)(void*))fn)
#define VECTOR_BSEARCH(vector, value, comp)                               \
    vector_bsearch(vector,                                                \
                   (const void*)value,                                    \
                   (int (*)(const void*, const void*))comp)

#endif
