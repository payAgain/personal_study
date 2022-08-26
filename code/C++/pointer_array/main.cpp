#include <iostream>

using namespace std;

int main()
{
    int arr[10]={0};
    printf("%p\n",arr);//首元素的地址
    printf("%p\n",arr+1);
    // 这两种表达是等价的
    printf("%p\n",&arr[0]);//首元素的地址
    printf("%p\n",&arr[0]+1);

    printf("%p\n",&arr);//整个数组元素的地址
    printf("%p\n",&arr+1); // 输出为 arr + 10

    cout << "=============" << endl;
    char a[] = "你好，全民制作人";
    char *b, tp[] = "我是蔡徐坤";
    char e = 'a', f = 'k';

    // b可以是指向数组的指针，也可以是指向变量的指针
    b = &e;
    printf("b可以是一个字符指针:%c \n", *b);
    // 在C++中，数组给指针赋值的时候，会降级为指向第一个元素的指针
    // 这个行为在反过来的时候是错误的
    // a = b; wrong!
    b = a;  // 当b指向a 他们直接没有任何区别
    printf("b指向数组: %s \n", b);
    printf("b也可以像a一样指向某个元素 %c \n", b[2]);
    

    char *c[2] = {a, tp}; // []的优先级大于* 因此这是一个指针数组
    char **d; // d同样有2种理解的方式，一个是二级指针，另一个是指向指针数组的指针 \
    也就是d和c是等价的

    b = &e; // b 再次指向一个字符
    d = &b;
    printf("修改之前 %c \n", **d);
    *d = &f; //让b指向f
    printf("修改之后 %c \n", *b);

    printf("这是指针数组中的内容: %s, %s \n", c[0], c[1]);
    d = c;
    printf("d 和 c是等价的 %s, %s \n", d[0], d[1]);

    char x[2][10] = {"丁真", "理塘"};
    char (*y)[10] = x; // 我们修改*的优先级，这样y首先是一个指针 \
    然后它指向了一个10个元素的数组，这就是二维数组
    printf("这是一个二维数组的指针: %s, %s \n", y[1], y[0]);

}