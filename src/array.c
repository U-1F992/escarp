#include <escarp.h>

#include <stdlib.h>
#include <string.h>

struct escarp_array_t {
    size_t type;
    void *data;
    size_t len;
    size_t capacity;
};

escarp_array_error_t escarp_array_new(escarp_array_t **arr, size_t type,
                                      void *data, size_t len) {
    escarp_array_t *new_arr = NULL;
    size_t i = 0;

    if (arr == NULL || (data == NULL && len != 0)) {
        return ESCARP_ARRAY_ERROR_NULL_POINTER;
    } else if (type == 0) {
        return ESCARP_ARRAY_ERROR_INVALID_TYPE;
    }

    new_arr = (escarp_array_t *)malloc(sizeof(escarp_array_t));
    if (new_arr == NULL) {
        return ESCARP_ARRAY_ERROR_MEMORY_ALLOCATION_FAILURE;
    }
    if (len != 0) {
        new_arr->data = malloc(type * len);
        if (new_arr->data == NULL) {
            free(new_arr);
            return ESCARP_ARRAY_ERROR_MEMORY_ALLOCATION_FAILURE;
        }
    } else {
        new_arr->data = NULL;
    }

    for (i = 0; i < len; i++) {
        memcpy(new_arr->data + i * type, data + i * type, type);
    }
    new_arr->type = type;
    new_arr->len = len;
    new_arr->capacity = new_arr->len;

    *arr = new_arr;
    return ESCARP_ARRAY_SUCCESS;
}

void escarp_array_delete(escarp_array_t *arr) {
    if (arr == NULL) {
        return;
    }
    free(arr->data);
    free(arr);
}

escarp_array_error_t escarp_array_get_ptr(escarp_array_t *arr, void **out) {
    if (arr == NULL) {
        return ESCARP_ARRAY_ERROR_NULL_POINTER;
    }
    *out = arr->data;
    return ESCARP_ARRAY_SUCCESS;
}

escarp_array_error_t escarp_array_get_length(escarp_array_t *arr, size_t *out) {
    if (arr == NULL) {
        return ESCARP_ARRAY_ERROR_NULL_POINTER;
    }
    *out = arr->len;
    return ESCARP_ARRAY_SUCCESS;
}

escarp_array_error_t escarp_array_at(escarp_array_t *arr, size_t idx,
                                     void *out) {
    if (arr == NULL || out == NULL) {
        return ESCARP_ARRAY_ERROR_NULL_POINTER;
    } else if (idx >= arr->len) {
        return ESCARP_ARRAY_ERROR_OUT_OF_BOUNDS;
    }

    memcpy(out, arr->data + idx * arr->type, arr->type);
    return ESCARP_ARRAY_SUCCESS;
}

escarp_array_error_t escarp_array_push(escarp_array_t *arr, void *in) {
    void *new_data = NULL;

    if (arr == NULL || in == NULL) {
        return ESCARP_ARRAY_ERROR_NULL_POINTER;
    }

    if (arr->len == arr->capacity) {
        size_t new_capacity = arr->capacity == 0 ? 1 : arr->capacity * 2;
        new_data = realloc(arr->data, arr->type * new_capacity);
        if (new_data == NULL) {
            return ESCARP_ARRAY_ERROR_MEMORY_ALLOCATION_FAILURE;
        }
        arr->data = new_data;
        arr->capacity = new_capacity;
    }

    memcpy(arr->data + arr->type * arr->len, in, arr->type);
    arr->len++;

    return ESCARP_ARRAY_SUCCESS;
}

escarp_array_error_t escarp_array_pop(escarp_array_t *arr, void *out) {
    size_t new_len = 0;
    void *new_data = NULL;
    void *out_buf = NULL;

    if (arr == NULL) {
        return ESCARP_ARRAY_ERROR_NULL_POINTER;
    } else if (arr->len == 0) {
        return ESCARP_ARRAY_ERROR_OUT_OF_BOUNDS;
    }

    new_len = arr->len - 1;
    if (out != NULL) {
        out_buf = malloc(arr->type);
        if (out_buf == NULL) {
            return ESCARP_ARRAY_ERROR_MEMORY_ALLOCATION_FAILURE;
        }
        memcpy(out_buf, arr->data + arr->type * new_len, arr->type);
    }

    if (new_len == 0) {
        free(arr->data);
        arr->data = NULL;
        arr->capacity = 0;

    } else if (new_len < arr->capacity / 4) {
        size_t new_capacity = arr->capacity / 2;
        new_data = realloc(arr->data, arr->type * new_capacity);
        if (new_data == NULL) {
            /* Keep the original data and capacity */
        } else {
            arr->data = new_data;
            arr->capacity = new_capacity;
        }
    }
    arr->len = new_len;

    if (out != NULL) {
        memcpy(out, out_buf, arr->type);
    }
    return ESCARP_ARRAY_SUCCESS;
}

escarp_array_error_t escarp_array_concat(escarp_array_t *dest,
                                         escarp_array_t *src) {
    void *new_data;

    if (dest == NULL || src == NULL) {
        return ESCARP_ARRAY_ERROR_NULL_POINTER;
    }
    if (dest->type != src->type) {
        return ESCARP_ARRAY_ERROR_INVALID_TYPE;
    }

    new_data = realloc(dest->data, dest->type * (dest->len + src->len));
    if (new_data == NULL) {
        return ESCARP_ARRAY_ERROR_MEMORY_ALLOCATION_FAILURE;
    }

    memcpy(new_data + dest->type * dest->len, src->data, src->type * src->len);
    dest->data = new_data;
    dest->len += src->len;
    dest->capacity = dest->len;

    return ESCARP_ARRAY_SUCCESS;
}