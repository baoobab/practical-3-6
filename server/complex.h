#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include "cmath"
#include "QString"
using namespace std;

class TComplex {
private:
    double re, im;
public:
    TComplex();
    TComplex(const double&);
    TComplex(const double&, const double&);
    double modulus(); // Модуль к.ч. - вещ. число
    double argument(); // Аргумент к.ч. - вещ. число
    bool operator==(TComplex);
    bool operator!=(TComplex);
    bool operator<(TComplex);
    bool operator>(TComplex);
    bool operator>=(TComplex c);
    bool operator<=(TComplex c);
    TComplex operator*(TComplex);
    TComplex operator/(TComplex);
    TComplex operator+(TComplex);
    TComplex& operator-(); // унарный минус
    TComplex operator-(TComplex); // бинарный минус
    TComplex& operator+=(TComplex);
    TComplex& operator-=(TComplex);
    friend QString& operator<<(QString&, TComplex);
    friend QString& operator>>(QString&, TComplex&);
    friend ostream& operator<<(ostream&, TComplex);
    friend istream& operator>>(istream&, TComplex&);
    friend TComplex pow(TComplex, int);
    friend TComplex sqrt(TComplex num);
};

#endif // COMPLEX_H
