#include <iostream>
using namespace std;

int main() {
    int a1, b2s, ar, bb;
    a1 = 3;
    bb = 23;
    b2s = 4;
    ar = 5;
    a1 = 2 * a1 + ar * (b2s + bb);
    cout << a1 << '\n';
    bb = a1 * (b2s + 2 * ar);
    cout << "value=" << bb;
    return 0;
}