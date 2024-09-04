#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
	size_t ret = 0;
	while (*s){
		s++;
		ret++;
	}
	return ret;
}

char *strcpy(char *dst, const char *src) {
	char *ret = dst;
	while (*src != '\0'){
		*dst = *src;
		dst++;
		src++;
	}
	*dst = '\0';
	return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
	char *ncpy = dst;
	while (0 < n && *src != '\0'){
		*dst = *src;
		dst++;
		src++;
		n--;
	}
	
	while (n > 0){
		*dst = '\0';
		dst++;
		n--;
	} 
	return ncpy;
}

char *strcat(char *dst, const char *src) {
	char *cat = dst;
	while (*dst != '\0'){
		dst++;
	}
	
	while (*src != '\0'){
		*dst = *src;
		dst++;
		src++;
	}
	*dst = '\0';
	return cat;
}

int strcmp(const char *s1, const char *s2) {
	while (*s1 != '\0' && *s2 != '\0'){
		if (*s1 != *s2){
			return(*s1 - *s2);
		}
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int strncmp(const char *s1, const char *s2, size_t n) {
	while (n > 0 && *s1 != '\0' && *s2 != '\0'){
		if (*s1 != *s2){
			return(*s1 - *s2);
		}
		s1++;
		s2++;
		n--;
	}
	if (n == 0){
		return 0;
	}
	return (*s1 - *s2);
}

void *memset(void *s, int c, size_t n) {
	unsigned char *byte_ptr = (unsigned char *)s;
	for (size_t i = 0; i < n; i++){
		*byte_ptr = (unsigned char)c;
		byte_ptr++;
	}
	return s;
}

void *memmove(void *dst, const void *src, size_t n) {
	unsigned char *dst_byte = (unsigned char *)dst;
	const unsigned char *src_byte = (const unsigned char *)src;
	if (dst > src && dst < src + n){
		dst_byte += n;
		src_byte += n;
		while (n > 0){
			*(--dst_byte) = *(--src_byte);
			n--;
		}
	}
	
	else {
		for (size_t i = 0; i < n; i++){
			*dst_byte = *src_byte;
			dst_byte++;
			src_byte++;
		}
	}
	return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
	unsigned char *out_byte = (unsigned char *)out;
	const unsigned char *in_byte = (const unsigned char*)in;
	for (size_t i = 0; i < n; i++){
		*out_byte = *in_byte;
		out_byte++;
		in_byte++;
	}
	return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const unsigned char *s1_byte = (const unsigned char *)s1;
	const unsigned char *s2_byte = (const unsigned char*)s2;
	for (size_t i = 0; i < n; i++){
		if (*s1_byte != *s2_byte){
			return (*s1_byte - *s2_byte);
		}
		s1_byte++;
		s2_byte++;
	}
	return 0;
}

#endif
