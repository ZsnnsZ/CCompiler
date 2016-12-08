struct struCH{
    char ch;
    struct struCH *next;
}struCH,*temp,*head,*shift,*top,*base;
/*head指向线性链表的头结点，shift指向动态建成的结点
 *top和base分别指向堆栈的顶和底
 */

/*出栈函数*/
void pop();
/*入栈函数*/
void push(char ch);
/*根据数组下标计算的值找对应的产生式，并入栈*/
void doforpush(int t);
/*总控程序*/
void identify();
/*判断当前字符是否为非终结符*/
int isN(char cur1);
/*根据cur1,cur2转为数字以判断是否有产生式*/
void checkforpush();
//判断当前栈顶字符和输入串字符是否对应相等
int curequal(char cur1, int cur2);