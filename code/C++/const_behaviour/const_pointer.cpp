// #include <iostream>
// using namespace std;
int main() {
    const int a = 10;
    int *p = (int *)&a;
    *p = 20;
    int c = a;
    // cout << *p << ' ' << a << endl;
    // cout << p << ' ' << &a << endl;
    // cout << &(*p) << endl;
    return 0;
}