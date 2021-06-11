#pragma once

using std::complex;
using std::vector;

/**
* Word transformation for Schonhage Strassen multiply algorithm
*/
void Long_Word_transform(vector<complex<double> >& array, bool back);

/**
* Long word multiplication using Schonhage Strassen algorithm
*/
vector<int> Long_Word_mul(vector<int> l, vector<int> r);
