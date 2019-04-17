
/***************************************************
 * @author:Linghaijun
 * @date:2019/3/13
 * @time:21:37
 * @description:创建头文件，设定宏，设定链表节点
 * 2019/3/15 向头文件中添加部分数据项，创建函数声明
 * 2019/3/16 完善头文件中数据组织，补充添加数据项，完善链表节点，完善函数接口
 * 2019/3/19 大幅修改头文件，包括其中数据组织形式，以及函数接口的实现
 * 2019/3/22 进一步完善头文件
 ***************************************************/

#ifndef _INFORMATION_NEEDED_H
#define _INFORMATION_NEEDED_H


#define SETBUYAMOUNTLIMIT   200                 //设定赠送配件的购买数量
#define SETSUMLIMITS        10000000.00         //设定程序中出现金额最高额度
#define MAX_INT             2147483647          //设定int类型的最大值，防止溢出
#define STRLENLIM			31                  //设定单个字符串所能容纳有效字符个数最大值为30


/*为销售链表中数据组织需要而开放的枚举数组*/
enum sell_on_bulk_or_tail { bulk = 0, tail = 1 };
enum Any_presents { no_presents = 0, is_presents = 1 };



/*使用结构体Date记录每次交易的时间，（年份默认为当年）
 * 其中数据项包括月份，天数，小时数，分钟数，*/
typedef struct Date
{
    int trade_month;
    int trade_hour;
    int trade_minute;
    int trade_day;
}Date;


/*使用结构体记录每个赠品的信息*/
typedef struct Present
{
    char name[STRLENLIM];
    char module_name[STRLENLIM];
    char producer[STRLENLIM];
    char supplier[STRLENLIM];
}Present;



/*进货链表节点数据组织情况：
*purchase_time:Date结构体类型，用于记录买入时间信息
*name：字符数组类型，用于记录买入商品名称
*module_name：字符数组类型，用于记录买入商品型号名称
*producer:字符数组类型，用于记录制造商
*price：double类型，用于记录商品单价
*number：unsigned int类型，用于记录商品数量
*money：double类型，用于记录商品总价（程序计算）
*supplier：字符数组类型，用于记录供货商名称
*succ：Purchase *类型，用于标记链表后继节点
*prod：Purchase *类型，用于标记链表前驱节点
*/
typedef struct Purchase
{
    Date purchase_time;
    char name[STRLENLIM];
    char module_name[STRLENLIM];
    char producer[STRLENLIM];
    double price;
    unsigned int number;
    double money;
    char supplier[STRLENLIM];
    struct Purchase *succ;
}Purchase;


/*销售链表节点数据组织情况：
 * sell_method:sell_on_bulk_or_tail枚举类型，用于记录操作是批发还是零售
*sale_time:Date结构体类型，用于记录销售时间信息
*name：字符数组类型，用于记录销售商品名称
*module_name：字符数组类型，用于记录买入商品型号名称
*producer:字符数组类型，用于记录制造商名称
*price：double类型，用于记录商品单价
*number：unsigned int类型，用于记录商品数量
*money：double类型，用于记录商品总价（程序计算）
*client：字符数组类型，用于记录用户名称
 * present_method:Any_present枚举类型，用于记录是否有赠品
 * present_name:字符数组类型，用于记录赠品名称（如果有）
*succ：Purchase *类型，用于标记链表后继节点
*prod：Purchase *类型，用于标记链表前驱节点
*/
typedef struct Sale
{
    enum  sell_on_bulk_or_tail sell_method;
    Date sale_time;
    char name[STRLENLIM];
    char module_name[STRLENLIM];
    char producer[STRLENLIM];
    double price;
    unsigned int number;
    double money;
    char client[STRLENLIM];
    enum Any_presents present_method;
    Present present;
    struct Sale *succ;
}Sale;

typedef struct Goods
{
    char name[STRLENLIM];
    char module_name[STRLENLIM];
    char producer[STRLENLIM];
    char supplier[STRLENLIM];
    unsigned int number;
    struct Goods *succ;
    struct Goods *pred;
}Goods;


/*******************************全局变量区***************************************
 * sum             -----------> double类型，表示初始持有总金额，默认为￥5000,000
 * sales_sum       -----------> double类型，表示出售总金额，默认为0
 * purchase_sum    -----------> double类型，表示进货总金额，默认为0
 * goods_head      -----------> Goods*类型，用于指向存储库存信息的链表头结点
 * ******************************************************************************/
double sum = 5000000.00;
double sale_sum = 0;
double purchase_sum = 0;
Goods *goods_header;


/*****************************************************
 * 以下开放接口为main函数中的主要函数，由小组成员分别完成：
 * 1.录入进货/销售信息
 * 2.修改进货/销售记录
 * 3.删除进货/销售记录
 * 4.查询进货/销售记录
 * 5.统计交易信息
 * 6.统计赠品情况
 * 7.打印进货/销售信息
 * 8.保存当前交易信息至文件
 ******************************************************/


/*传入参数：Purchase *链表头结点指针
 *返回参数：Purchase *修改后的链表头结点指针
 *操作：从文件中录入进货信息
 * */
Purchase *check_in_purchase_records(Purchase *);


/*传入参数：Sale *链表头结点指针
 *返回参数：Sale *修改后的链表头结点指针
 *操作：从文件中录入销售信息
 * */
Sale *check_in_sale_records(Sale *);


/*传入参数：Purchase *链表头结点指针
 *返回参数：Purchase *修改后的链表头结点指针
 *操作：修改Purchase链表中单条进货信息
 * */
Purchase *revise_purchase_records(Purchase *);


/*传入参数：Sale *链表头结点指针
 *返回参数：Sale *修改后的链表头结点指针
 *操作：修改Sale链表中单条销售信息
 * */
Sale *revise_sale_records(Sale *);


/*传入参数：Purchase *链表头结点指针
 *返回参数：Purchase *修改后的链表头结点指针
 *操作：删除Purchase链表中单条进货信息
* */
Purchase *delete_purchase_records(Purchase *);


/*传入参数：Sale *链表头结点指针
 *返回参数：Sale *修改后的链表头结点指针
 *操作：删除Sale链表中单条进货信息
* */
Sale *delete_sale_records(Sale *);


/*传入参数：Purchase，Sale类型*链表头结点指针
 *返回参数：无返回值
 *操作：打印进货/销售操作记录
 * */
void print_records(Purchase *, Sale *);


/*传入参数：Purchase Sale *链表头结点指针
 * 返回参数：无返回值
 * 操作：统计进货信息*/
void assess_purchase_records(Purchase *);


/*传入参数：Purchase Sale *链表头结点指针
 * 返回参数：无返回值
 * 操作：统计销售信息*/
void assess_sale_records(Sale *);


/*传入参数：Sale *链表头结点指针
 *返回参数：无返回值
 *操作：统计赠品信息
 * */
void assess_presents_records(Sale *);

/************************************************
 * 待定
 * 操作：打印库存报表
 * void store_records(Sale *,Purchase *);
 * 如库存信息显示如下：（需要考虑库存有哪些数据，需要打印哪些数据）
 * **********************************************/



/*传入参数：Purchase Sale *链表头结点指针
 *返回参数：无返回值
 *操作：打印一段时间内的进货/销售信息（含）
 * */
void print_time_period_records(Purchase *, Sale *);


/*传入参数：Purchase Sale *链表头结点指针
 *返回值：无返回值
 *操作：将链表中进货/销售信息储存至文件
 * */
void save_records_to_file(Purchase *, Sale *);


#endif //_INFORMATION_NEEDED_H
