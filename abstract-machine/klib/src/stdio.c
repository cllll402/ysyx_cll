#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static void itoa(int num, char *str) {
    char *ptr = str; 
    bool is_negative = false; 

    if (num < 0) {
        is_negative = true; 
        num = -num; 
    }    

    do {
        int digit = num % 10;
        *ptr++ = digit + '0'; 
        num /= 10;
    }   while (num > 0); // 将数字的每一位转换为字符并放入字符串中
    
    if (is_negative) {
        *ptr++ = '-';
    }
    *ptr = '\0'; 

    for (char *start = str, *end = ptr - 1; start < end; ++start, --end) {
        char temp = *start;
        *start = *end;
        *end = temp;
    }    // 反转字符串中的字符，使其变成正确的顺序
}

int sprintf(char *out, const char *fmt, ...) {
    va_list args;   // 定义可变参数列表
    va_start(args, fmt); 
    char *ptr = out; 

    while (*fmt != '\0') {
        if (*fmt == '%') { 
            fmt++; 
            if (*fmt == 'd') { 
                int num = va_arg(args, int); 
                char buffer[32]; 
                itoa(num, buffer); 
                for (char *buf_ptr = buffer; *buf_ptr != '\0'; ++buf_ptr) {
                    *ptr++ = *buf_ptr; 
                }
            } 
            
            else if (*fmt == 's') { 
                const char *str = va_arg(args, const char *); 
                while (*str != '\0') {
                    *ptr++ = *str++; 
                }
            }
        } 
        else { 
            *ptr++ = *fmt; 
        }
        fmt++; 
    }
    *ptr = '\0'; 
    va_end(args); 
    return ptr - out; // 返回生成的字符串的长度
}

int printf(const char *fmt, ...) {
   panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
   panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
   panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
   panic("Not implemented");
}

#endif
