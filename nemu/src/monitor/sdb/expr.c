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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
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
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_tokens(char *e) {
    int position = 0;
    int i;
    regmatch_t pmatch;
    
    nr_tokens = 0;
    while (e[position] != '\0') {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i ++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;
    
                printf("At position [%d] match the char \"%c\" \nwith rules[%d] \"%s\"\n\n", position, substr_start[0], i, rules[i].regex);
                position += substr_len;
                tokens[nr_tokens].type = rules[i].tokens_type;

                if (tokens[nr_tokens].type == TK_NOTYPE) {
                    break;
                }
    
                if (tokens[nr_tokens].type == TK_NUM || 
                    tokens[nr_tokens].type == TK_VAR || 
                    tokens[nr_tokens].type == TK_REG || 
                    tokens[nr_tokens].type == TK_EQ || 
                    tokens[nr_tokens].type == TK_NEQ || 
                    tokens[nr_tokens].type == TK_LT || 
                    tokens[nr_tokens].type == TK_GT || 
                    tokens[nr_tokens].type == TK_LE || 
                    tokens[nr_tokens].type == TK_GE || 
                    tokens[nr_tokens].type == TK_ASSIGN || 
                    tokens[nr_tokens].type == TK_PLUS || 
                    tokens[nr_tokens].type == TK_MINUS || 
                    tokens[nr_tokens].type == TK_MULTIPLY || 
                    tokens[nr_tokens].type == TK_DIVIDE || 
                    tokens[nr_tokens].type == TK_MOD || 
                    tokens[nr_tokens].type == TK_LPAREN || 
                    tokens[nr_tokens].type == TK_RPAREN) {
                    strncpy(tokens[nr_tokens].str, substr_start, substr_len);
                    tokens[nr_tokens].str[substr_len] = '\0';
                }
                nr_tokens++;
                break;
            }
        }
        if (i == NR_REGEX) {
            printf("There is something can not be match\n");
            return false;
        }
    }
    
    for (i = 0; i < nr_tokens; i++) {
        if (tokens[i].type == TK_MINUS &&
            (i == 0 || tokens[i - 1].type == TK_LPAREN || 
             tokens[i - 1].type == TK_PLUS || tokens[i - 1].type == TK_MINUS || 
             tokens[i - 1].type == TK_MULTIPLY || tokens[i - 1].type == TK_DIVIDE)) {
            tokens[i].type = TK_NEG;
        }
    }

    return true;
}



word_t eval(int p, int q, bool *check) {
  *check = true;
  if (p > q) {
  assert(0);
  } 
  
  else if (p == q) {
    if (tokens[p].type != TK_NUM) {
      *check = false;
      return 0;
    }
    word_t ret = strtol(tokens[p].str, NULL, 10);
    return ret;
  } 
  else if (check_parentheses(p, q)) {
  	if ( tokens[p + 1].type == TK_NEG){
  	word_t val2 = 
  	}
    return eval(p+1, q-1, check); //剥离最外层的括号再进行一次处理
  } 
  
  else {    
    int op = find_major(p, q);
    if (op < 0) {
    assert(0);
    }

    word_t val1 = eval(p, op-1, check);
    if (!*check) return 0;
    word_t val2 = eval(op+1, q, check);
    if (!*check) return 0;
    
    switch(tokens[op].type) {
      case TK_PLUS: {
      return val1 + val2;
      }
      
      case TK_MINUS: {
      return val1 - val2;
      }
      
      case TK_MULTIPLY: {
      return val1 * val2;
      }
      
      case TK_DIVIDE: {
		  if (val2 == 0) {
		  *check = false;
		  return 0;
		  } 
	  return (sword_t)val1 / (sword_t)val2; 
      }     
      default: assert(0);
    }
  }
}

bool check_parentheses(int p, int q) {
    if (tokens[p].type == TK_LPAREN && tokens[q].type == TK_RPAREN) {
    int count = 0;
    for (int i = p; i <= q; i++) {
        if (tokens[i].type == TK_LPAREN) {
        count++;
        } else if (tokens[i].type == TK_RPAREN) {
        count--;
        }

        if (count == 0) {
        return i == q;
        }
     }
  }
  return false;
}

int find_major(int p, int q) {
  int ret = -1, count = 0, op_type = 0;
  
  for (int i = p; i <= q; i++) {
    if (tokens[i].type == TK_NUM) {
      continue;  // Skip numbers
    }
    
    if (tokens[i].type == TK_LPAREN) {
      count++;   // Increase count for left parenthesis
    } 
    
    else if (tokens[i].type == TK_RPAREN) {
      if (count == 0) {
        return -1;  // Mismatched parentheses
      }
      count--;   // Decrease count for right parenthesis
    } 
    
    else if (count > 0) {
      continue;  // Skip operators inside parentheses
    } 
    
    else {
      int tmp_type = 0;
      switch (tokens[i].type) {
      case TK_MULTIPLY: 
      case TK_DIVIDE: 
        tmp_type = 1; break;
      case TK_PLUS: 
      case TK_MINUS: 
      case TK_NEG:    // Include TK_NEG in the same priority level as TK_MINUS
        tmp_type = 2; break;
      default: assert(0);
      }
      if (tmp_type >= op_type) {
        op_type = tmp_type;
        ret = i;
      }
    }
  }
  
  if (count != 0) return -1;  // Mismatched parentheses
  return ret;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  return eval(0, nr_token-1, success);;
}

