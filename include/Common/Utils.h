#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdbool.h>

static inline bool all(void* begin, void* end, bool (*pred)(const void*)) {
    for (
        void* iter = begin;
        iter && iter!=end;
        iter++
    ) {
        if (!pred(iter)) {
            return false;
        }
    } return true;
}

#endif /* COMMON_UTILS_H */