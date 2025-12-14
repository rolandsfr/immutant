#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

#define ARR_DEFINE(T, Name)                                                    \
	typedef struct Name {                                                      \
		size_t len;                                                            \
		size_t cap;                                                            \
		T* data;                                                               \
	} Name;                                                                    \
                                                                               \
	static inline void Name##_init(Name* v)                                    \
	{                                                                          \
		v->len = 0;                                                            \
		v->cap = 8;                                                            \
		v->data = malloc(v->cap * sizeof(T));                                  \
		if (!v->data) {                                                        \
			printf("Failed to allocate dynamic array\n");                      \
		}                                                                      \
	}                                                                          \
                                                                               \
	static inline void Name##_free(Name* v)                                    \
	{                                                                          \
		free(v->data);                                                         \
		v->data = NULL;                                                        \
		v->len = v->cap = 0;                                                   \
	}                                                                          \
                                                                               \
	static inline void Name##_push(Name* v, T value)                           \
	{                                                                          \
		if (v->len >= v->cap) {                                                \
			v->cap *= 2;                                                       \
			v->data = realloc(v->data, v->cap * sizeof(T));                    \
			if (!v->data) {                                                    \
				printf("Failed to realloc dynamic array\n");                   \
			}                                                                  \
		}                                                                      \
		v->data[v->len++] = value;                                             \
	}

#endif
