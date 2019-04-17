#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "interface.h"
#include "linghaijun.h"
//#include "wusiyuan.h"
//#include "liluchen.h"

/*程序operating_system开设三个文件，三条链表
 * 一个文件储存进货信息，对应一条链表，其头指针作为主函数中的局部变量，以参数形式传递给接口
 * 一个文件储存销售信息，对应一条链表，其头指针作为主函数中的局部变量，以参数形式传递给接口
 * 一个文件储存库存信息，对应一条链表，其头指针作为全局变量及时库存中的配件信息
 * 程序缺点：
 * 1）文件操作（读取，修改）可能引起错误
 * 2）将数据块作为链表节点直接写入，占据冗余空间（链表项中的指针是无效数据）
 * 程序优点：
 * 1）结构化特征明显，数据组织明确，错误控制表现好，界面美观
 * 数据在文件中的组织形式为：
 *
 * */
void printChoice();


int main()
{
    /*goods_head        -------------> 保存库存信息的链表头结点
     * sale_header      -------------> 保存销售信息的链表头结点
     * purchase_header  -------------> 保存进货信息的链表头结点
     * choice_one       -------------> 用户二重选择的外层选择
     * choice_two       -------------> 用户二重选择的内层选择
     * */
    Purchase *purchase_header = NULL;
    Sale     *sale_header     = NULL;
    char      choice_one      = '\0';
    char      choice_two      = '\0';


    printChoice();
    if (1 != scanf("%c%*c", &choice_one))
        { puts("非法操作，退出程序"); exit(0); }//读取数据并处理换行符
    while (choice_one)
    {

       /* switch (choice) {
            case 'b':
            case 'B': {
                printf("0：录入销售信息\t1：录入进货信息");
                switch(choice2)
                    check_in_purchase_records(linkedlist_header); }
                break;
            case 'c':
            case 'C':linkedlist_header = *check_in_sale_records(linkedlist_header);
                break;
            case 'd':
            case 'D':linkedlist_header = revise_records(linkedlist_header);
                break;
            case 'e':
            case 'E':linkedlist_header = delete_records(linkedlist_header);
                break;
            case 'f':
            case 'F':search_records(linkedlist_header);
                break;
            case 'g':
            case 'G':assess_trade_records(linkedlist_header);
                break;
            case 'h':
            case 'H':assess_presents_records(linkedlist_header);
                break;
            case 'i':
            case 'I':print_records_information(linkedlist_header);
                break;
            case 'j':
            case 'J':save_records_to_file(linkedlist_header);
                break;
            case 'k':
            case 'K':   puts("成功退出程序，再见"); exit(0);
            default:    puts("Sorry, no such choice, please select again:");
        }
        /*这里还要考虑每次更新数据之后是否需要及时更新数据结构体中的原有数据
         * （此为默认操作，即伴随每一次进货/出货，修改，删除等会对原有数据
         * 造成影响的操作之后都需要更新数据。这里使用一个全局结构体数组来实现*/
        printChoice();
        if (1 != scanf("%c%*c", &choice_one)) { puts("非法操作，退出程序"); exit(0); }
    }

    return 0;
}

void printChoice()
{
    printf("A.买进商品\n");
    printf("C.录入进货/销售信息\n");
    printf("D.修改进货/销售记录\n");
    printf("E.删除进货/销售记录\n");
    printf("F.查询进货/销售记录\n");
    printf("G.统计交易信息\n");
    printf("H.统计赠品情况\n");
    printf("I.打印进货/销售信息\n");
    printf("J.保存当前交易信息至文件\n");
    printf("K.退出程序\n");
    /*此处可以插入认为必要且能够添加的新功能，如
     * 1.完整输出当前进货/销售记录(必要)
     * 2.完整输出文件中所有进货/销售记录(非必要)
     * 2.清除所有信息（必要）
     * 3.……
     */
}
