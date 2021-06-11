#include <complex>
#include <vector>
#include <cmath>

#include "utils.h"

int Utils_reverse(int num, const int size) {
    int res = 0;
    for (int i = 0; i < size; ++i) {
        res <<= 1;
        res |= num & 1;
        num >>= 1;
    }
    return res;
}

int Utils_log(int num) {
    int res = 0;
    do {
        ++res;
        num >>= 1;
    } while (num);
    return res;
}

int Utils_power2(int num) {
    for (int i = 0; i < 32; ++i) if ((1 << i) >= num) return 1 << i;
    return -1;
}

void Utils_swap_bits(vector<complex<double> > &num) {
    int l = Utils_log(num.size()) - 1;
    vector<complex<double> > res(num.size());
    for (int i = 0; i < num.size(); ++i) res[i] = num[Utils_reverse(i, l)];
    num = res;
}
