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

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "expr.h"
#include "/home/cll/ysyx/ysyx-workbench/nemu/src/isa/riscv32/local-include/reg.h"
#include "/home/cll/ysyx/ysyx-workbench/nemu/include/memory/vaddr.h"
#include "/home/cll/ysyx/ysyx-workbench/nemu/tools/gen-expr/gen_expr.h"
#include "watchpoint.h"

#define NR_CMD ARRLEN(cmd_table)

#define PMEM_START 0x80000000
#define PMEM_END   0x87FFFFFF
//明确一下pmen的地址范围

typedef uint32_t vaddr_t;
typedef uint32_t word_t;

void init_regex();
void init_wp_pool();
void isa_reg_display();
void test_comparison();

static int is_batch_mode = false;
static int cmd_c(char *args);
static int cmd_q(char *args);
static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);

struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
}

cmd_table [] = {
  { "help", "Information", cmd_help },
  { "c", "Continue", cmd_c },
  { "q", "Exit", cmd_q },
  { "si", "Step", cmd_si },
  { "info", "Print", cmd_info},
  { "x", "Scan", cmd_x },
  { "p", "Caculate", cmd_p },
  { "w", "Watch", cmd_w },
  { "d", "Delite", cmd_d }
};

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args){
	char *arg = strtok(NULL, " ");
	int N = 0;
	
	if (arg == NULL) {
		N=1;
		cpu_exec(N);
		printf("Command excuted %d time",N);
	}
	
	else {
		N = atoi(arg);
		cpu_exec(N);
		printf("Command excuted %d times",N);
	} 
	return 0;
}

static int cmd_info(char *args){
	char *arg = strtok(NULL, " ");

    if (arg == NULL) {
    printf("You need to input r/w\n");
    }
    
    else if (strcmp(arg, "r") == 0) {
    printf("Display the reg information\n");
    isa_reg_display();
    }
    
    else if (strcmp(arg, "w") == 0) {
    printf("Display the watchpoint information\n");
    trace_and_difftest();
    }
    
    else {
        printf("You need to input r/w\n");
    }
	return 0;
}

static int cmd_x(char *args){

	char *arg1 = strtok(NULL, " ");
	char *arg2 = strtok(NULL, " ");
    
	if (arg1 == NULL || arg2 == NULL){
		printf("Args number is not enough\n");
		printf("Example:x N EXPR\n\n");
		
		printf("PS:PMEM_START 0x80000000\n   PMEM_END   0x87FFFFFF\n");
		return 0;
	}

	int N = strtol(arg1,NULL,10);
    vaddr_t EXPR = strtol(arg2,NULL,16);
    //这里的strtol将字符串转换为长整数型
    //第一个参数为转换目标，第二个参数默认为NULL，第三个参数为进制
    
	for (int i = 0; i < N;) {
			printf("%#010x: ", EXPR);
			
			for (int j = 0; i < N && j < 4; i++, j++) {
				word_t w = vaddr_read(EXPR, 4);
				EXPR += 4;
				printf("%#010x ", w);
			}
		puts("");
	}
	return 0;
}

static int cmd_p(char *args){
	
	init_regex();
	
	char e[512];
	bool success = false;
	char *arg = strtok(NULL, " ");
	
	if (arg == NULL) {
    printf("You need to input the args\n");
    }
    
    else {
    strncpy(e, arg, sizeof(e)-1);
	e[sizeof(e)-1]= '\0';
    }
    word_t result = expr(e,&success);      
    if (success) {
    printf("\033[1;31mThe final result is :%d \033[0m \n",result);
    }
    else {
    printf("\033[1;31mThe fomula is illegality\033[0m \n");
    printf("\033[1;31mAnd if you want to type the minus nad pointer, you need to take the parens!\033[0m \n");
    }
	return 0;
}

static int cmd_w(char *args){

	char *arg = strtok(NULL, " ");

	if (arg == NULL) {
	printf("You need to input args EXPR\n");
	}

	else {
	bool success;
	word_t result = expr(args,&success);
	create_wp(args,result);
	} 
	
	return 0;
}

static int cmd_d(char *args){

	char *arg = strtok(NULL, " ");

	if (arg == NULL) {
	printf("You need to input args N\n");
	}
	
	else {
	int N = atoi(arg);
	remove_wp(N);
	} 
	return 0;
}
/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { 
        return; 
        }
        break;
      }
    }
	
    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void test_comparison() {
    FILE *fp = fopen("/home/cll/ysyx/ysyx-workbench/nemu/tools/gen-expr/build/input", "r");

    if (fp == NULL) {
        printf("\033[31mFailed to open file\033[0m\n");
        return;
    }

    char *e = NULL;
    size_t len = 0;
    int32_t result;
    ssize_t read;
    bool success = false;

    while ((read = getline(&e, &len, fp)) != -1) {
        if (read > 0 && e[read - 1] == '\n') {
            e[read - 1] = '\0';
        }

        char *equal_pos = strchr(e, '=');
        char *colon_pos = strchr(e, ':');

        *equal_pos = '\0';
        result = strtoull(equal_pos + 1, NULL, 10);

        // 计算表达式的长度并分配内存
        size_t expr_length = equal_pos - colon_pos - 1;
        char *expr_str = (char *)malloc(expr_length + 1);

        // 复制表达式
        strncpy(expr_str, colon_pos + 1, expr_length);
        expr_str[expr_length] = '\0';

        // 计算表达式结果
        int32_t expr_result = expr(expr_str, &success);
        if (result != expr_result) {
        printf("\033[31mExpected: %d, Got: %d\n\033[0m", result, expr_result);
        } 
        else {
        printf("\033[32mThe test is right. The final value is %d\n\033[0m", result);
        }
        free(expr_str);
    }
    fclose(fp);
    if (e) free(e);
}


void init_sdb() {
    /* Compile the regular expressions. */
    init_regex();
    /* Initialize the watchpoint pool. */
    init_wp_pool();
    //test_comparison();
}
