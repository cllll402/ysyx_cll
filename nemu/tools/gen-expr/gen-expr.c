/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

typedef uint32_t word_t;

static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; 

static char *code_format =
"#include <stdio.h>\n\n"
"int main() { "
"  int result = 0; "
"  result = %s; "
"  printf(\"%%d\", result); "
"  return 0; "
"}";

static uint32_t choose(int max) {
	return rand() % max;
}

static void gen(char c) {
	char str[2] = {c, '\0'};
	strcat(buf, str);  
}

static void gen_rand_num() {
	int num = choose(100);
	char str[12];
	snprintf(str,sizeof(str),"%d",num);
	strcat(buf, str);
}

static void gen_rand_op() {
	switch (choose(4)) {
    case 0: 
	gen('+');
    break;
    
    case 1: 
	gen('-');
    break;
    
    case 2:
    gen('*');
    break;
    
	default:
	gen('/');
    break;
  }
}

static void gen_rand_expr() {

    switch (choose(3)) {
		case 0: 
		gen_rand_num(); 
		break;
		
		case 1: 
		gen('('); 
		gen_rand_expr(); 
		gen(')'); 
		break;
		
		default: 
		gen_rand_expr(); 
		gen_rand_op(); 
		gen_rand_expr(); 
		break;
    }
}

void gen_expr_result(){
	FILE *fp = fopen("/home/cll/ysyx/ysyx-workbench/nemu/tools/gen-expr/build/input","r");
	
	if (fp == NULL) {
    printf("\033[31mFailed to open file\033[0m\n");
    return;
    }
    
    char *e = NULL;
    size_t len_e = 0;
    word_t result;
    ssize_t read;
    int test_result; 

    read = getline(&e, &len_e, fp);
    if (read == -1) {
        perror("Failed to read expression");
        fclose(fp);
        if (e) free(e);
        return;
    }
    
    if (read > 0 && e[read - 1] == '\n') {
        e[read - 1] = '\0';
    }

    test_result = fscanf(fp, "%u", &result);
    /*if (test_result != 1) {
        perror("Failed to read test result");
        fclose(fp);
        if (e) free(e);
        return;
    }*/
	
	printf("Expression: %s\n", e);
    printf("Expected Result: %d\n", test_result);
	fclose(fp);
	
    if (e) free(e);
}
	
int main(int argc, char *argv[]) {
    
    int ret;
    int result;
    int loop = 1;
    int seed = time(0);
    srand(seed);
    
    if (argv[1] == NULL){
    loop = 10;
    } 
    
    if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
    }
    
    for (int i = 0; i < loop; i ++) {
    buf[0] = '\0';
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);
    FILE *fp = fopen("/tmp/.code.c", "w");
    fputs(code_buf, fp);
    fclose(fp);    //将模板写入缓存区，再将缓存区写入文件code
    
    ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) {
    assert(0);
    }
    
    fp = popen("/tmp/.expr", "r");
    ret = fscanf(fp, "%d", &result);
    pclose(fp);    //如果执行编译失败，则跳过执行后面的命令
    
    printf("Expr[%d]:%s=%d\n\n",i,buf,result);
    
    remove("/tmp/.code.c");
	remove("/tmp/.expr");
	gen_expr_result();
	
    }
    return 0;
}
