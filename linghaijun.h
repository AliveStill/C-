

//
// Created by ??D? on 2019/3/29.
//

#ifndef OPERATINT_SYSTEM_LINGHAIJUN_H
#define OPERATINT_SYSTEM_LINGHAIJUN_H

#include "interface.h"

void search_in_out_records(Purchase *, Sale *);


void all_present(Sale *);


void search_in_records(Purchase *, char *);


void search_out_records(Sale *, char *);


int print_in_info(Purchase *);


int print_out_info(Sale *);


Sale *revise_sale_records(Sale *);


Purchase *revise_purchase_records(Purchase *);


Purchase *build_pur_node(void);


Sale *build_sale_node(void);


void send_gift(Sale *);


int get_repo_list_length();


void synch_info();


void remove_goods(const Purchase*);


void add_goods(const Sale*);


#endif //OPERATINT_SYSTEM_LINGHAIJUN_H
