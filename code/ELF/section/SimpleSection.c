int printf(const char *format, ...);

int global_init_var = 84;
// int global_uninit_var;

void func1(int i)
{
    printf("%d\n", i);
}

int main(void)
{
    static int static_var = 85;
    // static int static_var1;
    int a = 1;
    int b;
    // func1(static_var1 + static_var);
    return a;
}
