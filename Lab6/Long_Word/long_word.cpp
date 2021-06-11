#include <complex>
#include <vector>
#include <cmath>

#include "long_word.h"
#include "../Utils/utils.h"

void Long_Word_transform(vector<complex<double> >& num, bool is_back) {
    Utils_swap_bits(num);
    int n = num.size();
    double t = (is_back ? -1 : 1);

    for (int i = 2; i <= n; i *= 2) {
        complex <double> wm(cos(t * 2 * M_PI / (double)i), sin(t * 2 * M_PI / (double)i));

        for (int k = 0; k < n; k += i) {
            complex <double> w(1);
            for (int j = 0; j < i / 2; ++j) {
                complex <double> a0 = num[k + j];
                complex <double> w_a1 = w * num[k + j + i / 2];
                num[k + j] = a0 + w_a1;
                num[k + j + i / 2] = a0 - w_a1;

                if (is_back) {
                    num[k + j] /= 2.0;
                    num[k + j + i / 2] /= 2.0;
                }
                w *= wm;
            }
        }
    }
}

vector<int> Long_Word_mul(vector<int> l, vector<int> r) {
    int n = Utils_power2(std::max(l.size(), r.size())) * 2;
    l.resize(n);
    r.resize(n);

    vector<complex<double> > temp_l(l.begin(), l.end());
    vector<complex<double> > temp_r(r.begin(), r.end());

    Long_Word_transform(temp_l, false);
    Long_Word_transform(temp_r, false);

    vector<complex<double> > temp(n);
    for (int i = 0; i < n; ++i)
        temp[i] = temp_l[i] * temp_r[i];
    Long_Word_transform(temp, true);

    vector<int> res(n, 0);
    for (int i = 0; i < n; ++i)
        res[i] += (int)(temp[i].real() + 0.5);
    for (int i = 0; i < n - 1; ++i)
        if (res[i] > 9) {
            res[i + 1] += res[i] / 10;
            res[i] %= 10;
        }
    while (res[res.size() - 1] > 9) {
        res.push_back(res[res.size() - 1] / 10);
        res[res.size() - 2] %= 10;
    }

    while (res.back() == 0 && res.size() > 1)
        res.pop_back();

    return res;
}
