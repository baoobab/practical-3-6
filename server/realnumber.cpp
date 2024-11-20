#include "realnumber.h"
#include "cmath"
#include "QString"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

using namespace std;


TRealNumber::TRealNumber() {
    this->num = 0;
}

TRealNumber::TRealNumber(const double& n) {
    this->num = n;
}

TRealNumber::TRealNumber(const int& n) {
    this->num = (n * 1.0);
}

bool TRealNumber::operator==(TRealNumber rn) {
    return this->num == rn.num;
}

bool TRealNumber::operator!=(TRealNumber rn) {
    return !(*this == rn); // Инвертируем оператор равенства
}

ostream& operator<<(ostream& os, TRealNumber rn) {
    os << rn.num;
    return os;
}

QString& operator<<(QString& s, TRealNumber rn) {
    s += QString().setNum(rn.num);

    return s;
}

istream& operator>>(istream& is, TRealNumber& rn) {
    double num;
    if (is >> num) { // Проверяем успешность ввода
        rn.num = num;
    }
    return is;
}

QString& operator>>(QString& istr, TRealNumber& rn) {
    // Регулярное выражение для поиска чисел типа double
    static const QRegularExpression regexp("([-+]?[0-9]*\\.?[0-9]+)");

    // Поиск первого вхождения числа
    QRegularExpressionMatch match = regexp.match(istr);
    if (match.hasMatch()) {
        rn = TRealNumber(match.captured(0).toDouble());
    } else {
        return istr; // Если чисел нет вообще - выход
    }

    // Удаляем прочитанное число из строки
    istr.remove(0, match.capturedStart() + match.capturedLength());

    return istr;
}

TRealNumber pow(TRealNumber base, int exponent) {
    TRealNumber result(1.0); // Начальное значение (1.0)

    if (exponent == 0) {
        return result; // Любое число в степени 0 равно 1
    }

    // Работаем с положительным и отрицательным показателем степени
    bool isNegativeExponent = (exponent < 0);
    exponent = abs(exponent); // Берем абсолютное значение степени

    for (int i = 0; i < exponent; ++i) {
        result = result * base; // Умножаем результат на базу
    }

    if (isNegativeExponent) {
        // Если степень отрицательная, возвращаем обратное значение
        return TRealNumber(1.0 / result.num);
    }

    return result;
}

TRealNumber TRealNumber::operator*(TRealNumber rn) {
    TRealNumber resultRN; // результирующее вещ. число
    resultRN.num = this->num * rn.num;

    return resultRN;
}

TRealNumber TRealNumber::operator/(TRealNumber rn) {
    TRealNumber resultRN; // результирующее вещ. число
    resultRN.num = this->num / rn.num;

    return resultRN;
}

TRealNumber TRealNumber::operator+(TRealNumber rn) {
    TRealNumber resultRN; // результирующее вещ. число
    resultRN.num = this->num + rn.num;

    return resultRN;
}

TRealNumber& TRealNumber::operator+=(TRealNumber rn) {
    this->num = this->num + rn.num;

    return *this;
}

TRealNumber& TRealNumber::operator-=(TRealNumber rn) {
    this->num = this->num - rn.num;

    return *this;
}


// Унарный минус
TRealNumber& TRealNumber::operator-() {
    this->num = -this->num;
    return *this;
}

// Бинарный минус
TRealNumber TRealNumber::operator-(TRealNumber rn) {
    TRealNumber resultRN; // результирующее вещ. число
    resultRN.num = this->num - rn.num;

    return resultRN;
}

// Оператор больше
bool TRealNumber::operator>(TRealNumber rn) {
    return this->num > rn.num;
}

// Оператор меньше
bool TRealNumber::operator<(TRealNumber rn) {
    return this->num < rn.num;
}

// Оператор больше или равно
bool TRealNumber::operator>=(TRealNumber rn) {
    return !(*this < rn); // Если не меньше, то больше или равно
}

// Оператор меньше или равно
bool TRealNumber::operator<=(TRealNumber rn) {
    return !(*this > rn); // Если не больше, то меньше или равно
}

TRealNumber sqrt(TRealNumber rn) {
    return TRealNumber(sqrt(rn.num));
}
