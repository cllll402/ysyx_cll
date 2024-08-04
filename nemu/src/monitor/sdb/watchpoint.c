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

#include "expr.h"

#define NR_WP 32

typedef struct watchpoint {
    int NO;
    char expr[256];
    uint32_t result;
    struct watchpoint *next;
} WP;

WP wp_pool[NR_WP] = {};
WP *head = NULL, *free_list = NULL;

void init_wp_pool() {

    for (int i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    } //单向链表，一直从1-->32，最后一个指向指针为空
    head = NULL;
    free_list = wp_pool;
}

WP *new_wp(char *expr,uint32_t result){
	if (free_list == NULL){
	printf("There is no free watchpoint\n");
	assert(0);
	}
	
	else {
	WP *wp = free_list;
	free_list = free_list -> next;  //插入节点，替换节点
	strncpy(wp->expr, expr, sizeof(wp->expr) - 1);
    wp->expr[sizeof(wp->expr) - 1] = '\0';
    wp->result = result;
    wp->next = head; //一个指向expr，一个指向result,一个指向空余head
    head = wp;
    return wp;
	}
}

void free_wp(WP *wp){
	if (head == wp){
	head = head->next;
	}  
	
	else {
  		WP *prev = head;
        while (prev != NULL && prev->next != wp) {
        prev = prev->next;
        }
        
        if (prev != NULL) {
        prev->next = wp->next;
        }
    }
	wp->expr[0]='\0';
	wp->result = 0;
    wp->next = free_list;
    free_list = wp;
}

void remove_wp(int N){
	WP *wp = &wp_pool[N];
	free_wp(wp);
	printf("The NO.%d watchpoint has been deleted\n",N);
}

void create_wp(char *expr,uint32_t result){
	WP *wp = new_wp(expr,result);
	strcpy(wp->expr,expr);
	wp->result = result;
	printf("The NO.%d watchpoint has been created\n",wp->NO);
}

void trace_and_difftest(){

	for (int i = 0; i < NR_WP; i ++) {
	//	if (wp_pool[i].expr){
	//	printf("The NO.%d watchpoint is NUll\n",wp_pool[i].NO);
	//	}
	//	else {
		printf("The NO.%d watchpoint expr:%s   result：%d\n",wp_pool[i].NO, wp_pool[i].expr, wp_pool[i].result);
	//	}
    }
}
