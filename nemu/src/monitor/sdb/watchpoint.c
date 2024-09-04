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
#include "/home/cll/ysyx/ysyx-workbench/nemu/src/monitor/sdb/watchpoint.h"

#define NR_WP 32

WP wp_pool[NR_WP] = {};
WP *head = NULL, *free_list = NULL;

void init_wp_pool() {

    for (int i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    wp_pool[i].flag = false;
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
    wp->next = head; 
    wp->flag = true;  //一个存expr，一个存result,一个存下一个head
    head = wp;
    return wp;
	}
}

void free_wp(WP *wp){
	if (head == wp){
	head = head->next;
	}  
	
	else {
  		WP *pre = head;
        while (pre != NULL && pre->next != wp) {
        pre = pre->next;
        }
        
        if (pre != NULL) {
        pre->next = wp->next;
        printf("Free the watchpoint\n");
        }
    }
	wp->expr[0]='\0';
	wp->result = 0;
    wp->next = free_list;
    free_list = wp;
}

void create_wp(char *expr,uint32_t result){
	WP *wp = new_wp(expr,result);
	strcpy(wp->expr,expr);
	wp->old_result = result;
	printf("The NO.%d watchpoint has been created at address:%p \n",wp->NO, (void*)wp);
}

void remove_wp(int N){
    if(N < 0 || N >= NR_WP){
    printf("0 <= N <= 32\n");
    assert(0);
    }
	WP *wp = &wp_pool[N];
    if (wp->flag) { 
    free_wp(wp);
    printf("The NO.%d watchpoint has been deleted\n", N);
    } 
    else {
    printf("The NO.%d watchpoint is not active\n", N);
    }
}

void isa_watchpoint_display(){
 	bool set_flag = false;
	for (int i = 0; i < NR_WP; i ++){
		if (wp_pool[i].flag) { 
		printf("NO.%d   %s   %p   result:%x\n", wp_pool[i].NO, wp_pool[i].expr, (void *)&wp_pool[i], wp_pool[i].old_result);
		set_flag = true;
		} 
	}		
	if (set_flag == false) {
	printf("\033[1;031mThere is no watchpoint\033[0m\n");
	}
}
