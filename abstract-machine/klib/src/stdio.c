#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void putch(char c); 

void write(const char *str) {
    while (*str != '\0') {
        putch(*str++); // 逐个字符输出
    }
}

static void itoa(int num, char *str) {
    char *ptr = str; 
    bool is_negative = (num < 0); 

    if (is_negative) {
        num = -num; 
    }

    do {
        int digit = num % 10;
        *ptr++ = digit + '0'; 
        num /= 10;
    } while (num > 0); 

    if (is_negative) {
        *ptr++ = '-';
    }
    *ptr = '\0'; 

    // 反转字符串中的字符，使其变成正确的顺序
    for (char *start = str, *end = ptr - 1; start < end; ++start, --end) {
        char temp = *start;
        *start = *end;
        *end = temp;
    }
}

static int format_output(char *out, const char *fmt, va_list ap) {
    char *ptr = out; 

    while (*fmt != '\0') {
        if (*fmt == '%') { 
            fmt++; 
            if (*fmt == 'd') { 
                int num = va_arg(ap, int); 
                char buffer[32]; 
                itoa(num, buffer); 
                for (char *buf_ptr = buffer; *buf_ptr != '\0'; ++buf_ptr) {
                    *ptr++ = *buf_ptr; 
                }
            } 
            else if (*fmt == 's') { 
                const char *str = va_arg(ap, const char *); 
                while (*str != '\0') {
                    *ptr++ = *str++; 
                }
            }
            else if (*fmt == 'c') { // 处理字符
                char ch = (char)va_arg(ap, int);
                *ptr++ = ch;
            }
        } else { 
            *ptr++ = *fmt; 
        }
        fmt++; 
    }
    *ptr = '\0'; 
    return ptr - out; 
}

int sprintf(char *out, const char *fmt, ...) {
    va_list args;   
    va_start(args, fmt); 
    int len = format_output(out, fmt, args); 
    va_end(args);
    return len; 
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    return format_output(out, fmt, ap);
}

int printf(const char *fmt, ...) {
    char buffer[1024]; // 临时缓冲区
    va_list args;
    va_start(args, fmt); 
    int len = vsprintf(buffer, fmt, args); 
    va_end(args);
    
    write(buffer); 
    return len; 
}

/*
int snprintf(char *out, size_t n, const char *fmt, ...) {
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
}
*/

#endif
