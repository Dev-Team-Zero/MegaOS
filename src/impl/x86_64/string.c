#include "strings.h"
#include <stddef.h>

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n > 0) {
        unsigned char c1 = (unsigned char)*s1;
        unsigned char c2 = (unsigned char)*s2;

        if (c1 != c2 || c1 == '\0') {
            return c1 - c2;
        }

        s1++;
        s2++;
        n--;
    }

    return 0;
}

int strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

void* memcpy(void* dest, const void* src, size_t count) {
    char* dst = (char*)dest;
    const char* source = (const char*)src;
    for (size_t i = 0; i < count; i++) {
        dst[i] = source[i];
    }
    return dest;
}

char* strchr(const char* s, int c) {
    while (*s) {
        if (*s == (char)c) return (char*)s;
        s++;
    }
    return NULL;
}

void strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i]; i++) dest[i] = src[i];
    for (; i < n; i++) dest[i] = '\0';
}

char* strcpy(char* dest, const char* src) {
    char* ret = dest;
    while ((*dest++ = *src++));
    return ret;
}