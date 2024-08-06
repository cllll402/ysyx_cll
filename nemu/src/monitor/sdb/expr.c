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
#include "/home/cll/ysyx/ysyx-workbench/nemu/include/memory/vaddr.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

int find_major(int p, int q);
bool check_parentheses (int p, int q);
int32_t eval(int p, int q, bool *check);

enum { 
    TK_NOTYPE = 256,     // 空格
    TK_NUM,              // 十进制
	
	TK_PLUS,             // 加号
	TK_MINUS,            // 减号
	TK_MULTIPLY,         // 乘号
	TK_DIVIDE,           // 除号
	
	TK_EQ,               // 等于号
	TK_NEQ,              // 不等于号
	TK_AND,              // 与
	TK_OR,               // 或

	TK_LT,               // 小于
	TK_GT,               // 大于
	TK_LE,               // 小于等于
	TK_GE,               // 大于等于
	
	TK_LPAREN,           // 左括号
	TK_RPAREN,           // 右括号
	TK_REG,              // 寄存器
	
	TK_NEG,              // 负号
	TK_POS,              // 正号
	
	TK_PTR,              // 指针       
	TK_HEX,              // 十六进制
    // ASCII 为 0-255，所以编码从 256 开始，其他类型顺序排列下来
};

static struct rule {
    const char *regex;
    int tokens_type;
} rules[] = {
    {" +", TK_NOTYPE},               // 空格
    {"(0x[0-9a-fA-F]+|[0-9]+)", TK_NUM},
                                     // 十进制或者十六进制
    {"\\+", TK_PLUS},                // 加号
    {"-", TK_MINUS},                 // 减号
    {"\\*", TK_MULTIPLY},            // 乘号
    {"/", TK_DIVIDE},                // 除号
    
    {"==", TK_EQ},                   // 等号
    {"!=", TK_NEQ},                  // 不等号
    {"&&", TK_AND},                  // 与
    {"\\|\\|", TK_OR},               // 或
    
    {"<=", TK_LE},                   // 小等于
    {">=", TK_GE},                   // 大等于
    {"<", TK_LT},                    // 小于
    {">", TK_GT},                    // 大于

    {"\\(", TK_LPAREN},              // 左括号 
    {"\\)", TK_RPAREN},              // 右括号
    {"\\$[a-zA-Z0-9]+", TK_REG},     // 寄存器
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
 	int i;
 	char error_msg[128];
	int ret;

    for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0) {
		  regerror(ret, &re[i], error_msg, 128);
		  panic("Regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}//ret=0时程序继续输出报错信息，regex为正则表达式提供了库
    }
}

typedef struct token {
	int type;
	char str[32];
} Token;

static int nr_tokens __attribute__((used))  = 0;
static Token tokens[32] __attribute__((used)) = {};
/*****************************************************************************************/
static bool make_tokens(char *e) {
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_tokens = 0;
    while (e[position] != '\0') {
        for (i = 0; i < NR_REGEX; i++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;
                position += substr_len;
                tokens[nr_tokens].type = rules[i].tokens_type;

                if (tokens[nr_tokens].type == TK_NOTYPE) {
                break;
                }

                if (tokens[nr_tokens].type == TK_NUM) {
                strncpy(tokens[nr_tokens].str, substr_start, substr_len);
                tokens[nr_tokens].str[substr_len] = '\0';
		            if (strncmp("0x", tokens[nr_tokens].str, 2) == 0) {
		            tokens[nr_tokens].type = TK_HEX;
		            }
                } 
                
                else {
                strncpy(tokens[nr_tokens].str, substr_start, substr_len);
                tokens[nr_tokens].str[substr_len] = '\0';
                }
                nr_tokens++;
                break;
            }
        }
        if (i == NR_REGEX) {
        printf("There is something that cannot be matched\n");
        return false;
        }
    }
	for (i = 0; i < nr_tokens; i++) {
		if (tokens[i].type == TK_MINUS && (i == 0 || (tokens[i - 1].type != TK_NUM && tokens[i - 1].type != TK_HEX && tokens[i - 1].type != TK_RPAREN))) {
	    tokens[i].type = TK_NEG;
  		} 
		else if (tokens[i].type == TK_PLUS && (i == 0 || (tokens[i - 1].type != TK_NUM && tokens[i - 1].type != TK_HEX && tokens[i - 1].type != TK_RPAREN))) {
	    tokens[i].type = TK_POS;
   		}
   		else if (tokens[i].type == TK_MULTIPLY && (i == 0 || (tokens[i - 1].type != TK_NUM && tokens[i - 1].type != TK_HEX && tokens[i - 1].type != TK_RPAREN))) {
	    tokens[i].type = TK_PTR;
   		}
	} 
    return true;
}


int32_t eval(int p, int q, bool *check) {
	*check = true;
	word_t ret = 0;
	if (p > q) {
	assert(0);
	} 
			
	else if (p == q ) {
		if (tokens[p].type == TK_NUM) {
		ret = strtol(tokens[p].str, NULL, 10);
		} 
		else if (tokens[p].type == TK_HEX) {
		ret = strtol(tokens[p].str, NULL, 16);
		} 
		else if (tokens[p].type == TK_REG) {
		bool success = false;
		isa_reg_str2val(tokens[p].str, &success);
		} 
		else {
		assert(0);
		}
		return ret;
	} 
	
    else if (check_parentheses(p, q)) {
		return eval(p+1, q-1, check); //剥离最外层的括号
	} 
		
	else {    
	    int op = find_major(p, q);
	    if (op < 0) {
	    assert(0);
	    }
			if (tokens[op].type == TK_NEG){
			return ret = -eval(p + 1, q, check);
			}
			if (tokens[op].type == TK_POS){
			return ret = eval(p + 1, q, check);
			}
			if (tokens[p].type == TK_PTR){
			vaddr_t addr = strtoul(tokens[p + 1].str, NULL,16); 
			printf("the addr 0x%x\n",addr);
			return vaddr_read(addr, 4);
			}
	    word_t val1 = eval(p, op-1, check);
	    if (!*check) return 0;
	    word_t val2 = eval(op+1, q, check);
	    if (!*check) return 0;
	  
	    switch(tokens[op].type) {
	    case TK_PLUS: return val1 + val2;
	    case TK_MINUS: return val1 - val2;
	    case TK_MULTIPLY: return val1 * val2;
	    case TK_DIVIDE: {
			if (val2 == 0) {
			*check = false;
			return 0;
			} 
		return (int32_t)val1 / (int32_t)val2; 
	    } 
	    		
		case TK_GT: return val1 > val2;
		case TK_LT: return val1 < val2;
		case TK_GE: return val1 >= val2;
		case TK_LE: return val1 <= val2;
		
		case TK_EQ: return val1 == val2;
		case TK_OR: return val1 || val2;
		case TK_NEQ: return val1 != val2;
		case TK_AND: return val1 && val2;
	
	    default: assert(0);
	    }
	}return 0;
}

int find_major(int p, int q) {
	int ret = -1, count = 0, op_type = 0;
  
	for (int i = p; i <= q; i++) {
		if (tokens[i].type == TK_NUM || tokens[i].type == TK_HEX || tokens[i].type == TK_REG) {
		continue;  
		}
    
		if (tokens[i].type == TK_LPAREN) {
		count++;   
		} 
		
		else if (tokens[i].type == TK_RPAREN) {
		count--;  
		} 
		
		else if (count > 0) {
		continue;  
		} 
		
		else {
		int tmp_type = 0;
		switch (tokens[i].type) {
		case TK_NEG: case TK_PTR: case TK_POS: tmp_type = 0; break;
		case TK_MULTIPLY: case TK_DIVIDE: tmp_type = 1; break;
		case TK_PLUS: case TK_MINUS: tmp_type = 2; break;
		case TK_EQ: case TK_NEQ: case TK_LT: case TK_GT: case TK_LE: case TK_GE: tmp_type = 3; break;
		case TK_AND: tmp_type = 4; break;
		case TK_OR: tmp_type = 5; break;
		default: assert(0);
		}
			if (tmp_type >= op_type) {
			op_type = tmp_type;
			ret = i;
			}
		}
	}
	if (count != 0) return -1;  
	return ret;
}
/*****************************************************************************************/
bool check_parentheses(int p, int q) {
    if (tokens[p].type == TK_LPAREN && tokens[q].type == TK_RPAREN) {
    int count = 0;
    for (int i = p; i <= q; i++) {
        if (tokens[i].type == TK_LPAREN) {
        count++;
        } 
        
        else if (tokens[i].type == TK_RPAREN) {
        count--;
        }

        if (count == 0) {
        return i == q;
        }
     }
  }
  return false;
}

int32_t expr(char *e, bool *success) {
  if (!make_tokens(e)) {
    *success = false;
    return 0;
  }
  return eval(0, nr_tokens-1, success);
}

