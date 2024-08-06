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

#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

#define NR_WP 32

typedef struct watchpoint {
    int NO;
    char expr[256];
    uint32_t result;
    uint32_t old_result;  
    bool flag; 
    struct watchpoint *next;
} WP;

extern WP wp_pool[NR_WP];

void init_wp_pool();
WP *new_wp(char *expr,uint32_t result);
void free_wp(WP *wp);
void create_wp(char *expr,uint32_t result);
void remove_wp(int N);
void isa_watchpoint_display();


#endif
