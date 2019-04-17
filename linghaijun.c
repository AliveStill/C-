
//
// Created by 绘新 on 2019/3/29.
//


#ifndef LINGHAIJUN_H_INCLUDED
#define LINGHAIJUN_H_INCLUDED

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"
#include "linghaijun.h"

/*统计所有赠品情况*/
void all_present(Sale *head)
{
    printf("所有赠品信息如下：\n");
    printf("名称：\t型号\t制造商\t供应商\n");
    for( Sale *current = head; current ; current = current->succ )
    {
        if ( !current ->  present_method ){ continue; }
        else{
            printf("%s\t", current -> present . name);
            printf("%s\t", current -> present . module_name);
            printf("%s\t", current -> present . producer);
            printf("%s\n", current -> present . supplier);
        }
    }
}


void search_in_out_records(Purchase *pur_head, Sale *sale_head)
{
    char name[STRLENLIM];
    char choice = '\0';

    /*这里还可以添加功能，让用户选择是否提前打印所有进货/销售信息*/
    printf("请输入要查询的配件名称：\n");
    if ( 1 != scanf("%s", name) ){ puts("读取失败，返回主界面"); return ; }
    printf("请选择要读入的记录类型：(按回车键确认)");
    printf("A.进货记录\t");
    printf("B.销售记录\n");
    if ( 1 != scanf("%c%*c", &choice) ){ puts("读取失败，返回主界面"); return ; };
    switch( choice )
    {
        case 'A':case 'a':search_in_records(pur_head,name);break;
        case 'B':case 'b':search_out_records(sale_head,name);break;
        default:{ puts("操作失败，返回主界面。"); return ; }
    }
}


void search_in_records(Purchase * head, char *str)
{
    if ( NULL == head ) { puts("无进货记录"); return ; }

    unsigned int i = 0;

    printf("商品中%s配件的所有信息记录如下：\n", str);
    printf("编号\t交易时间\t名称\t型号\t制造商\t供应商\t单价\t数量\t总价\n");
    for ( Purchase *current = head; current; current = current -> succ )
    {
        if ( 0 != strcmp(str, current -> name)  ) { continue; }
        else {
            printf("%d\t", i++);
            printf("%02d/%02d/%02d/%02d\t", current -> purchase_time . trade_month,
                    current -> purchase_time . trade_day,
                    current -> purchase_time . trade_hour,
                    current -> purchase_time . trade_minute);
            printf("%s\t", current -> name);
            printf("%s\t", current -> module_name);
            printf("%s\t", current -> producer);
            printf("%s\t", current -> supplier);
            printf("%lf\t", current -> price);
            printf("%d\t", current -> number);
            printf("%lf", ( current -> number ) * ( current -> price ));
            puts("");
        }
    }
    printf("按回车键返回主界面");
}


void search_out_records(Sale *head, char *str)
{
    if ( NULL == head ) { puts("无销售记录"); return ; }

    unsigned int i = 0;

    printf("商品中%s配件的所有信息记录如下：\n", str);
    printf("编号\t交易时间\t名称\t型号\t制造商\t买家\t单价\t数量\t总价\t赠品\n");
    for ( Sale *current = head; current; current = current -> succ )
    {
        if ( 0 != strcmp(str,current->name)  ) { continue; }
        else {
            printf("%d\t", i++);
            printf("%02d/%02d/%02d/%02d\t", current -> sale_time . trade_month,
                   current -> sale_time . trade_day,
                   current -> sale_time . trade_hour,
                   current -> sale_time . trade_minute);
            printf("%s\t", current -> name);
            printf("%s\t", current -> module_name);
            printf("%s\t", current -> producer);
            printf("%s\t", current -> client);
            printf("%lf\t", current -> price);
            printf("%d\t", current -> number);
            printf("%lf\t", ( current -> number ) * ( current -> price ));
            if( 1 == current -> present_method ) { printf("More information will displayed.\n"); }
            else { printf("/"); }
            puts("");
        }
    }
    printf("按回车键返回主界面\n");
}


Purchase *revise_purchase_records(Purchase *head)
{
    if ( NULL == head ) { puts("进货记录为空"); return head; }

    /*choice        ------------> 用户选择需要修改的节点的编号
     * limit        ------------> 链表尾的编号，即用户能够选择的最大编号
     * print_in_info------------> 打印链表信息，并返回链表的最大长度
     * */
    int choice   = 0;
    int order    = 0;
    int limit    = print_in_info(head);

    printf("请选择需要修改的记录前的编号：\n");
    while ( 1 != scanf("%d%*c",&choice) || choice > limit || choice <= 0 ){}

    /*定位要修改的节点,同时确定其前驱*/
    Purchase *position = head;
    Purchase *preview = NULL;
    for ( order = 1; position && order != choice ; ++order,preview = position, position = position-> succ);
    if ( !position ) { puts("该节点不存在,操作失败，返回主界面"); return head; }

    //根据要修改的进货信息修改库存以及同步全局变量
    remove_goods(position);

    /*申请新的节点,若返回值为空，打印错误信息，返回主函数，保证在此之后的函数部分不会出现异常*/
    Purchase *new_node = build_pur_node();
    if ( !new_node ) { puts("操作失败，返回主界面"); return head; }


    /*在进货链表中插入新链表项*/
    /*如果preview为空，则要删除的是头结点,否则按一般情况处理*/
    if ( preview == NULL ){
        new_node -> succ = head;
        head = new_node;
    }
    else {
        new_node -> succ = position -> succ;
        preview  -> succ = new_node;
    }


    /*删除原有链表项*/
    free( position );


    /*返回修改后的头指针*/
    return head;
}

Sale *revise_sale_records(Sale *head)
{
    if ( NULL == head ) { puts("销售记录为空"); return head; }

    /*choice        ------------> 用户选择需要修改的节点的编号
     * limit        ------------> 链表尾的编号，即用户能够选择的最大编号
     * print_in_info------------> 打印链表信息，并返回链表的最大长度
     * */
    int choice   = 0;
    int order    = 0;
    int limit    = print_out_info(head);

    printf("请选择需要修改的记录前的编号：\n");
    while ( 1 != scanf("%d%*c",&choice) || choice > limit || choice <= 0 ){}

    /*定位要修改的节点,同时确定其前驱*/
    Sale *position = head;
    Sale *preview = NULL;
    for ( order = 1; position && order != choice ; ++order,preview = position, position = position-> succ);
    if ( !position ) { puts("该节点不存在,操作失败，返回主界面"); return head; }

    //根据要修改的销售信息退还商品
    add_goods(position);

    /*申请新的节点,若返回值为空，打印错误信息，返回主函数*/
    Sale *new_node = build_sale_node();
    if ( !new_node ) { puts("操作失败，返回主界面"); return head; }


    /*在进货链表中插入新链表项*/
    /*如果preview为空，则要删除的是头结点,否则按一般情况处理*/
    if ( !preview  ){
        new_node -> succ = head;
        head = new_node;
    }
    else {
        new_node -> succ = position -> succ;
        preview  -> succ = new_node;
    }

    /*删除原有链表项*/
    free( position );

    /*返回修改后的头指针*/
    return head;
}


Purchase *build_pur_node(void)
{
    int choice;
    Purchase *current = (Purchase *)malloc(sizeof(Sale));
    assert ( current );
    if ( NULL == current ) { return NULL; }

    /*录入销售信息,使用while循环控制错误输入*/
    printf("***************************录入进货信息*******************************\n");
    printf("请输入交易时间，依次包括月，日，时，分,以空格分隔:(按回车键确认)\n");
    while ( 4 != scanf("%d%d%d%d%*c",&current -> purchase_time .trade_day,
                    &current -> purchase_time . trade_day,
                    &current -> purchase_time . trade_hour,
                    &current -> purchase_time . trade_minute) ){}
    printf("请输入配件名称：（按回车键确认）\n");
    while ( 1 != scanf("%s",current -> name) ){}
    printf("请输入配件型号：(按回车键确认）\n");
    while ( 1 != scanf("%s",current -> module_name) ){}
    printf("请输入制造商名称：（按回车键确认）\n");
    while ( 1 != scanf("%s",current ->producer) ){}
    printf("请输入供货商名称: (按回车键确认）\n");
    while ( 1 != scanf("%s",current -> supplier)){}
    printf("请输入进货单价：(按回车键确认）\n");
    while ( 1 != scanf("%lf%*c", &current -> price) ){}
    printf("请输入销售数量：（按回车键确认)\n");
    while ( 1 != scanf("%d", &current -> number) || current -> number <= 0){}


    /*询问用户是否保存信息，否，释放内存，退出函数并返回主函数*/
    printf("是否保存信息？输入0确定，输入1退出并返回主程序\n");
    if ( 1 != scanf("%d%*c",&choice) || choice != 0 )
        { free(current); return NULL; }
    /*这里选择忽略错误信息，将报告错误信息的权利留给上层主调函数*/
    else {
        puts("处理中，请稍后...");
        time_t begin_time = clock();
        time_t end_time = begin_time;
        //提供2500ms的延时，提供更好的用户体验
        while ( end_time - begin_time <= 2500 )
        {
            end_time = clock();
        }
    }

    //在此处理附加情况，如交易额超过最大限额，进货金额多于库存中剩余的钱
    double cur_num = current -> price * current -> number;
    if ( SETSUMLIMITS < cur_num )
    {
        printf("超出交易最高限额！\n");
        free ( current );
        return NULL;
    }
    if ( sum < cur_num )
    {
        printf("库存中的钱不够了!\n");
        printf("本次进货需要金额为%.2lf\n", cur_num );
        printf("库存中剩余金额为%.2lf\n",sum);
        free ( current );
        return NULL;
    }

    /***************************************************************************************************
     * 将信息加到全局库存链表中，代码在此实现，注意错误控制，有异常立即返回NULL中断操作并返回主程序
     * *************************************************************************************************/

    extern Goods *goods_header;

    /*创建链表节点,如果动态内存分配失败，则此操作失败，释放内存，返回NULL*/
    Goods *temp_goods = (Goods *)malloc(sizeof(Goods));
    assert ( NULL != temp_goods );
    if ( NULL == temp_goods )
    { free(current); return NULL; }

    if ( 0 != strcpy( temp_goods -> name,        current -> name) ||
         0 != strcpy( temp_goods -> module_name, current -> module_name) ||
         0 != strcpy( temp_goods -> producer,    current -> producer ) ||
         0 != strcpy( temp_goods -> supplier,    current -> supplier) )
    { free ( current ); free ( temp_goods ); return NULL; }

    /* 将库存信息存入链表，描述如下
     * 1>如果头结点为空，直接插入节点使之成为头结点，结束，否则转2
     * 2>对比关键字，若符合，链表节点成员number+(*current).number，结束，否则转3
     * 3>检测succ域是否为空，是，添加当前节点至succ域，结束，否则转4
     * 4>令当前指针指向下移，若为空，结束，否则转2
     * */
    if ( NULL == goods_header )
    {
        goods_header = temp_goods;
    } else {
        for ( Goods *go_node = goods_header; NULL != go_node; go_node = go_node ->succ )
        {
            if ( 0 != strcmp( go_node -> name,        temp_goods -> name) ||
                 0 != strcmp( go_node -> module_name, temp_goods -> module_name) ||
                 0 != strcmp( go_node -> producer,    temp_goods -> producer ) ||
                 0 != strcmp( go_node -> supplier,    temp_goods -> supplier) )
            {
                if ( NULL == go_node -> succ)
                {
                    go_node -> succ = temp_goods;
                    break;
                }
            }else {
                go_node -> number += current -> number;
                break;
            }
        }
    }

    //同步全局变量,sum减少，purchase_sum增加
    double cur_sum = current -> number * current -> price;
    if ( cur_sum > sum ) { puts("余额不足"); }
    sum -= cur_sum;
    if ( cur_sum > MAX_INT - purchase_sum ) { puts("发生数值上溢!"); }
    purchase_sum += cur_sum;

    /*将库存信息同步到文件*/
    synch_info();

    return current;
}


Sale *buile_sale_node(void)
{
    int choice;
    Sale *current = (Purchase *)malloc(sizeof(Sale));
    assert ( current );
    if ( NULL == current ) { return NULL; }

    /*录入销售信息,使用while循环控制错误输入*/
    printf("***************************录入销售信息*******************************\n");
    printf("请选择交易类型：0.批发 1.销售(按回车键确定)\n");
    while ( 1 != scanf("%d%*c",&choice) && choice >= 0 && choice < 2 ){}
    current -> sell_method = choice;
    printf("请输入交易时间，依次包括月，日，时，分,以空格分隔:(按回车键确认)\n");
    while ( 4 != scanf("%d%d%d%d%*c",&current -> sale_time .trade_day,
                    &current -> sale_time . trade_day,
                    &current -> sale_time . trade_hour,
                    &current -> sale_time . trade_minute) ){}
    printf("请输入配件名称：（按回车键确认）\n");
    while ( 1 != scanf("%s",current -> name) ){}
    printf("请输入配件型号：(按回车键确认）\n");
    while ( 1 != scanf("%s",current -> module_name) ){}
    printf("请输入制造商名称：（按回车键确认）\n");
    while ( 1 != scanf("%s",current ->producer) ){}
    printf("请输入客户名称: (按回车键确认）\n");
    while ( 1 != scanf("%s",current -> client) ){}
    printf("请输入销售单价：(按回车键确认）\n");
    while ( 1 != scanf("%lf%*c", &current -> price) ){}
    printf("请输入销售数量：（按回车键确认)\n");
    while ( 1 != scanf("%d", &current -> number) || current -> number <= 0){}
    double cur_num = current -> price * current -> number;

    printf("是否有赠品？有请按1，无请按0.(按回车键确认)\n");
    while ( 1 != scanf("%d%*c", &current -> present_method) ||
            current -> present_method > 1 || current -> present_method < 0 ) {}


    printf("是否保存信息？输入0确定，输入1退出并返回主程序\n");
    if ( 1 != scanf("%d%*c",&choice) || choice != 0 )
        { free(current); return NULL; }
    /*这里选择忽略错误信息，将报告错误信息的权利留给上层主调函数*/
    else {
        puts("处理中，请稍后...");
        time_t begin_time = clock();
        time_t end_time = begin_time;
        //提供2500ms的延时，提供更好的用户体验
        while ( end_time - begin_time <= 2500 )
        {
            end_time = clock();
        }
    }


    /******************************************************************************************************
     * 将交易信息与库存进行校对，若是符合要求，修改相关链表节点（可能包括删除），否则释放内存返回NULL
     * ****************************************************************************************************/

     /*每次更正销售信息之后，都要将原有货物退还到库存，再将新的货物从库存中拿出*/
     /*每次更正进货信息之后，都要将原有货物从库存中扣除，并加入新的库存*/
    /*搜寻链表中与销售信息相关的节点，确保只有一个或零个匹配,同时声明赠品节点*/
    extern Goods *goods_header;
    Goods *temp_goods = goods_header;
    Goods *preview = NULL;
    Goods *gift_node = NULL;

    for ( ; temp_goods; preview = temp_goods, temp_goods = temp_goods -> succ )
    {
        //寻找匹配的节点,匹配项包括名称，型号，制造商
        if ( !strcmp( temp_goods -> name, current -> name ) &&
             !strcmp( temp_goods -> module_name, current -> module_name ) &&
             !strcmp( temp_goods -> producer, current -> producer ) ) { break; }
    }
    /*若库存中缺少该种货物则增加销售记录失败，释放内存，返回NULL*/
    if ( !temp_goods ) { free ( current ); return NULL; }
    /*若库存中有该种货物但数量不够，则摘除该节点*/
    else if ( current -> number > temp_goods -> number)
    {
        /*若该节点为头结点*/
        if ( !preview )
        {
            goods_header = current;
            free ( goods_header );
        }/*否则该节点不是头结点*/
        {
            preview -> succ = temp_goods -> succ;
            free ( temp_goods );
        }
    }


    //系统自动赠送赠品
    if ( SETBUYAMOUNTLIMIT < current -> number ||
            SETSUMLIMITS < current -> number * current -> price )
    {
        send_gift(current);
    }

    //同步全局变量，sum增加，sale_sum增加
    double cur_sum  = current -> price * current -> number;
    if ( cur_sum > MAX_INT - sum ) { puts("发生数值上溢"); }
    sum += cur_sum;
    if ( cur_sum > MAX_INT - purchase_sum ) { puts("发生数值上溢"); }
    purchase_sum += cur_sum;

    /*将库存信息同步到文件*/
    synch_info();

    return current;
}


int print_in_info(Purchase *head)
{
    if ( NULL == head )
        { puts("*********************--进货记录为空--***********************"); return 0; }

    unsigned int i = 0;
    printf("*********************进货信息罗列如下**************************\n");
    printf("编号\t交易时间\t名称\t型号\t制造商\t供应商\t单价\t数量\t总价\n");

    for ( Purchase *current = head; NULL != current; current = current -> succ, ++i )
    {
        printf("%d\t", i);
        printf("%02d/%02d/%02d/%02d\t" ,
               current -> purchase_time . trade_month,
               current -> purchase_time . trade_day,
               current -> purchase_time . trade_hour,
               current -> purchase_time . trade_minute);
        printf("%s\t", current -> name);
        printf("%s\t", current -> module_name);
        printf("%s\t", current -> producer);
        printf("%s\t", current -> supplier);
        printf("%lf\t", current -> price);
        printf("%d\t", current -> number);
        printf("%lf", ( current -> number ) * ( current -> price ));
        puts("");
    }

    return i;
}


int print_out_info(Sale *head)
{
    if ( NULL == head )
        { puts("************************销售记录为空****************************"); return 0; }

    int i = 1;
    printf("***********************销售信息罗列如下*****************************\n");
    printf("编号\t交易时间\t名称\t型号\t制造商\t买家\t单价\t数量\t总价\t赠品\n");

    for ( Sale *current = head; current; current = current -> succ, ++i )
    {
        printf("%d\t", i);
        printf("%02d/%02d/%02d/%02d\t", current -> sale_time . trade_month,
               current -> sale_time . trade_day,
               current -> sale_time . trade_hour,
               current -> sale_time . trade_minute);
        printf("%s\t", current -> name);
        printf("%s\t", current -> module_name);
        printf("%s\t", current -> producer);
        printf("%s\t", current -> client);
        printf("%lf\t", current -> price);
        printf("%d\t", current -> number);
        printf("%lf\t", ( current -> number ) * ( current -> price ));
        if( 1 == current -> present_method ) { printf("There will be more information"); }
        else { printf("/"); }
        puts("");
    }

    return i - 1;
}

/*随机从库存中取出一件商品送给客户作为赠品*/
void send_gift(Sale *pos_node)
{
    extern Goods* goods_header;
    int list_length = get_repo_list_length();

    /*如果链表长度为0，返回NULL*/
    if ( 0 == list_length )
    {
        return NULL;
    } /*如果链表长度为1，则只打印第一个变量，提供一个选择*/
    else if ( 1 == list_length ){
        gift_mode(1);
    }/*如果链表长度为2，则只打印前两个变量，提供两个选择*/
    else if ( 2 == list_length ){
        gift_mode(2);
    } /*否则处理更一般的情况，随机打印三个变量,提供三个选择*/
    else if ( 3 <= list_length )
    {
        gift_mode(3);
    }
}

/*获取库存链表的长度*/
int get_repo_list_length()
{
    extern Goods* goods_header;
    int numb = 0;
    for ( Goods *current = goods_header; current; ++numb, current = current -> succ );

    return numb;
}

/*默认提供的随机选取方法：从链表头开始依次序打印前面的库存*/
void gift_mode(int mode)
{
    extern Goods *goods_header;
    Goods *current = goods_header;
    Goods *preview = NULL;
    int len = mode >= 3 ? 3 : mode;
    int choice = 0;
    printf("****************请选择一个赠品********************\n");
    printf(" 名称\t型号\t制造商\t供应商\n");

    for ( ; len > 0; --len, ++choice, current = current -> succ )
    {
        printf("%d.%s %s %s %s\n", choice + 1,
                current -> name, current -> module_name,
                current -> producer, current -> supplier);

    }
    while ( 1 != scanf("%d%*c", &choice) || choice <= 0 || choice > len ){}

    //根据用户选择定位节点并进行相关操作
    current = goods_header;
    for ( int i = 0; current && i < len; preview = current, current = current -> succ, ++i ){}

    //如果库存数量为1，则摘除该节点，否则将数量减1
    if ( current -> number <= 1 )
    {
        if ( !preview )
        {
            goods_header = current -> succ;
            free ( current );
        } else {
            preview -> succ = current -> succ;
            free ( current );
        }

    } else {
        current -> number -= 1;
    }


    /*将链表信息同步至文件*/
    synch_info();
}

/*将链表中信息同步到文件，如果打开文件失败则引发异常，直接退出程序*/
void synch_info(void)
{
    extern Goods * goods_header;
    FILE *fp = fopen("goods.txt","w");
    assert ( fp );
    /*打开文件失败，退出程序*/
    if ( !fp ) { exit(0); }

    /*将金额写入库存文件*/
    fwrite(&sum,sizeof(double),1,fp);
    /*将库存中配件*/
    for ( Goods *current = goods_header; current; current = current -> succ )
    {
        fwrite(current, sizeof(Goods),1,fp);
    }

    fclose(fp);
}

//根据要修改的节点退还商品，从库存中取出，并同步全局变量
//若是找到该商品且该商品库存容量足够退还，则正常退还
//若是找到该库存但容量不够，则将该商品从库存中移除
//若是未找到该商品，则不进行任何操作
void remove_goods(const Purchase *pos_node)
{

    extern Goods* goods_header;
    extern double sum;
    if ( !goods_header ) { return ; }

    Goods *current = goods_header;
    Goods *preview = NULL;
    for ( ; current; current = current -> succ )
    {
        if ( !strcmp(current -> name, pos_node -> name) &&
             !strcmp(current -> module_name, pos_node -> module_name) &&
             !strcmp(current -> supplier, pos_node -> supplier) &&
             !strcmp(current -> producer, pos_node -> producer) ) { break; }
    }

    //没有找到该节点
    if ( !current ) { return ; }
    //数量足够正常退还，则减少数量即可
    else if ( pos_node -> number < current -> number )
    {
        double cur_sum = pos_node -> number * pos_node -> price;
        //检查数值越界，若是，sum置0，下同
        if ( SETSUMLIMITS < cur_sum + sum )
        { puts("发生数值上溢"); }
        else { sum += cur_sum; }
    }
    //否则需要摘除该节点
    //检测是否为头结点
    else if ( !preview )
    {
        double cur_sum = pos_node -> number * pos_node -> price;
        if ( cur_sum > MAX_INT - sum ) { puts("发生数值上溢"); }
        sum += cur_sum;
        goods_header = current -> succ;
        free ( current );
    } //否则不是头结点
    else {
        double cur_sum = pos_node -> number * pos_node -> price;
        if ( cur_sum > MAX_INT -sum ) { puts("发生数值上溢"); }
        sum += cur_sum;
        preview -> succ = current -> succ;
        free ( current );
    }

    //同步全局变量，purchase_sum减少，sum增加
    double cur_sum = pos_node -> number * pos_node -> price;
    if ( cur_sum > MAX_INT - sum ) { puts("发生数值上溢"); }
    sum += cur_sum;
    purchase_sum -= pos_node -> number;

    /*同步链表信息到文件*/
    synch_info();
}

//根据要修改的节点退还商品至库存，并同步全局变量
//若是找到该商品
//容量超过上限打印提示并继续操作
//否则如果容量未超过上限正常退还
//若是未找到该商品，则将该商品添加到链表末尾并同步到文件
void add_goods(const Sale *pos_node)
{
    extern Goods* goods_header;
    Goods *current = goods_header;
    Goods *preview = NULL;
    for ( ; current; preview = current,current = current -> succ )
    {
        if ( !strcmp(current -> name, pos_node -> name) &&
             !strcmp(current -> module_name, pos_node -> module_name) &&
             !strcmp(current -> producer, pos_node -> producer) ) { break; }
    }

    //如果该节点不在链表中
    if ( !current )
    {
        Goods *new_node = malloc(sizeof(Goods));
        assert ( new_node );
        /*分配内存失败，退出程序*/
        if ( !new_node ) { return ;}
        /*复制信息给新的库存节点，过程出错退出本函数*/
        if ( strcpy( new_node -> name, pos_node -> name ) ||
             strcpy( new_node -> module_name, pos_node -> module_name ) ||
             strcpy( new_node -> producer, pos_node -> producer ) ) { return ; }
        /*如果头结点为空*/
        if ( !goods_header )
        {
            goods_header = new_node;
        } //否则头结点不为空
        else {
            preview -> succ = new_node;
        }
    }//否则如果该节点在链表中
    //如果库存金额不足，将其置0,打印错误信息
    else if ( sum < pos_node -> price * pos_node -> number )
    {
        puts("库存金额不够了");
        sum = 0;
    } else {
        sum -= pos_node -> price * pos_node -> number;
    }


    //检查是否有赠品，如果有，将其加入到库存中,否则直接退出函数
    if ( !(int)pos_node -> present_method ) { return ; }
    current = goods_header;
    for ( ; current; preview = current, current = current -> succ )
    {
        if ( !strcmp(current -> name, pos_node -> name) &&
             !strcmp(current -> module_name, pos_node -> module_name) &&
             !strcmp(current -> producer, pos_node -> producer) ) { break; }
    }

    /*如果没有找到该节点，将其加入到链表末尾*/
    if ( !current )
    {
        Goods *new_node2 = malloc(sizeof(Goods));
        assert ( new_node2 );
        if ( !new_node2 ) { return ;}
        /*如果链表为空，将其作为头结点*/
        if ( !preview )
        {
            goods_header = new_node2;
        }//否则链表不为空
        else {
            preview -> succ = new_node2;
        }
    }/*否则正常增加*/
    else   {
        /*数值越界，打印错误提示*/
        if ( 1 > MAX_INT - current -> number )
        {
            puts("发生数值上溢！");
        }
        current -> number += 1;
    }

    /*同步全局变量,sale_sum减少，sum减少*/
    double cur_sum = pos_node -> price * pos_node -> number;
    if ( sum < cur_sum ) { puts("余额不足"); }
    sum -= cur_sum;
    sale_sum -= cur_sum;


    /*同步链表信息到文件*/
    synch_info();
}

#endif // LINGHAIJUN_H_INCLUDED
