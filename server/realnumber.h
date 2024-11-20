#ifndef REALNUMBER_H
#define REALNUMBER_H

#include <iostream>
#include "cmath"
#include "QString"

using namespace std;


class TRealNumber
{
private:
    double num;
public:
    TRealNumber();
    TRealNumber(const double&);
    TRealNumber(const int&);
    bool operator==(TRealNumber);
    bool operator!=(TRealNumber);
    bool operator<(TRealNumber);
    bool operator>(TRealNumber);
    bool operator>=(TRealNumber rn);
    bool operator<=(TRealNumber rn);
    TRealNumber operator*(TRealNumber);
    TRealNumber operator/(TRealNumber);
    TRealNumber operator+(TRealNumber);
    TRealNumber& operator-(); // унарный минус
    TRealNumber operator-(TRealNumber); // бинарный минус
    TRealNumber& operator+=(TRealNumber);
    TRealNumber& operator-=(TRealNumber);
    friend QString& operator<<(QString&, TRealNumber);
    friend QString& operator>>(QString&, TRealNumber&);
    friend ostream& operator<<(ostream&, TRealNumber);
    friend istream& operator>>(istream&, TRealNumber&);
    friend TRealNumber pow(TRealNumber, int);
    friend TRealNumber sqrt(TRealNumber num);
};

#endif // REALNUMBER_H
