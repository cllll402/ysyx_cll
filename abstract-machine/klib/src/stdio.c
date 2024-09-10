#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define INT_MIN (-2147483648)

void putch(char c); 

void write(const char *str) {
    while (*str != '\0') {
        putch(*str++); // 逐个字符输出
    }
}

static void itoa(int num, char *str) {
    char *ptr = str;
    bool is_negative = (num < 0);
    unsigned int abs_num = is_negative ? -num : num; // 统一处理正负数

    if (num == INT_MIN) {  // 处理 INT_MIN 特殊情况
        strcpy(str, "-2147483648");
        return;
    }

    do {
        int digit = abs_num % 10;
        *ptr++ = digit + '0';
        abs_num /= 10;
    } while (abs_num > 0);

    if (is_negative) {
        *ptr++ = '-';
    }
    *ptr = '\0';

    // 反转字符串
    for (char *start = str, *end = ptr - 1; start < end; ++start, --end) {
        char temp = *start;
        *start = *end;
        *end = temp;
    }
}


static int format_output(char *out, size_t out_size, const char *fmt, va_list ap) {
    char *ptr = out; 
    size_t remaining = out_size - 1; // 保留一个字符给 '\0'

    while (*fmt != '\0' && remaining > 0) {
        if (*fmt == '%') { 
            fmt++; 
            if (*fmt == 'd') { 
                int num = va_arg(ap, int); 
                char buffer[32]; 
                itoa(num, buffer); 
                for (char *buf_ptr = buffer; *buf_ptr != '\0' && remaining > 0; ++buf_ptr) {
                    *ptr++ = *buf_ptr; 
                    remaining--;
                }
            } 
            else if (*fmt == 'u') { // 处理无符号整数
                unsigned int num = va_arg(ap, unsigned int);
                char buffer[32];
                // 实现无符号整数转换
                // 例如：itoa_unsigned(num, buffer);
                unsigned int temp = num; // 临时变量用于处理
                int index = 0;
                
                // 处理 0 的特殊情况
                if (temp == 0) {
                    buffer[index++] = '0';
                } else {
                    while (temp > 0) {
                        buffer[index++] = (temp % 10) + '0';
                        temp /= 10;
                    }
                }
                // 反转字符串
                for (int i = 0; i < index / 2; i++) {
                    char tmp = buffer[i];
                    buffer[i] = buffer[index - 1 - i];
                    buffer[index - 1 - i] = tmp;
                }
                buffer[index] = '\0'; // 添加字符串结束符
                
                for (char *buf_ptr = buffer; *buf_ptr != '\0' && remaining > 0; ++buf_ptr) {
                    *ptr++ = *buf_ptr; 
                    remaining--;
                }
            }
            else if (*fmt == 'x') { // 处理十六进制无符号整数
                unsigned int num = va_arg(ap, unsigned int);
                char buffer[32];
                int index = 0;

                if (num == 0) {
                    buffer[index++] = '0';
                } else {
                    while (num > 0) {
                        int digit = num % 16;
                        buffer[index++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
                        num /= 16;
                    }
                }

                // 反转字符串
                for (int i = 0; i < index / 2; i++) {
                    char tmp = buffer[i];
                    buffer[i] = buffer[index - 1 - i];
                    buffer[index - 1 - i] = tmp;
                }
                buffer[index] = '\0'; // 添加字符串结束符

                for (char *buf_ptr = buffer; *buf_ptr != '\0' && remaining > 0; ++buf_ptr) {
                    *ptr++ = *buf_ptr; 
                    remaining--;
                }
            }
            else if (*fmt == 'X') { // 处理大写十六进制
                unsigned int num = va_arg(ap, unsigned int);
                char buffer[32];
                int index = 0;

                if (num == 0) {
                    buffer[index++] = '0';
                } else {
                    while (num > 0) {
                        int digit = num % 16;
                        buffer[index++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'A');
                        num /= 16;
                    }
                }

                // 反转字符串
                for (int i = 0; i < index / 2; i++) {
                    char tmp = buffer[i];
                    buffer[i] = buffer[index - 1 - i];
                    buffer[index - 1 - i] = tmp;
                }
                buffer[index] = '\0'; // 添加字符串结束符

                for (char *buf_ptr = buffer; *buf_ptr != '\0' && remaining > 0; ++buf_ptr) {
                    *ptr++ = *buf_ptr; 
                    remaining--;
                }
            }
            else if (*fmt == 's') { 
                const char *str = va_arg(ap, const char *); 
                while (*str != '\0' && remaining > 0) {
                    *ptr++ = *str++; 
                    remaining--;
                }
            }
            else if (*fmt == 'c') { // 处理字符
                if (remaining > 0) {
                    char ch = (char)va_arg(ap, int);
                    *ptr++ = ch;
                    remaining--;
                }
            }
        } else { 
            if (remaining > 0) {
                *ptr++ = *fmt; 
                remaining--;
            }
        }
        fmt++; 
    }
    *ptr = '\0'; 
    return ptr - out; 
}

int sprintf(char *out, size_t out_size, const char *fmt, ...) {
    va_list args;   
    va_start(args, fmt); 
    int len = format_output(out, out_size, fmt, args); 
    va_end(args);
    return len; 
}

int vsprintf(char *out, size_t out_size, const char *fmt, va_list ap) {
    return format_output(out, out_size, fmt, ap);
}

int printf(const char *fmt, ...) {
    char buffer[1024]; // 临时缓冲区
    va_list args;
    va_start(args, fmt); 
    int len = vsprintf(buffer, sizeof(buffer), fmt, args); 
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
