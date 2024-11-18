#ifndef POLYNOM_H
#define POLYNOM_H

#include <iostream>
#include <array.h>
#include <math.h>
#include <QString>
#include <QRegularExpression>

enum EPrintMode {
    EPrintModeClassic,
    EPrintModeCanonical
};

template <class number>
class TPolynom {
private:
    EPrintMode printMode;

    number canonicCoef;
    TArray<number>* arrCoef;
    TArray<number>* arrRoot;
public:
    TPolynom(number canonicCoef = 1);
    TPolynom(const QString& qstrPolynom); // Новый метод, для извлечения полинома из QString
    ~TPolynom();
    void addCoef(number coef);
    void addRoot(number root);
    void flushMemory();
    void setPrintMode(EPrintMode mode);
    void setCanonicCoef(number coef);
    unsigned getRootsCount();
    number value(number val);
    template <class T>
    friend ostream& operator<<(ostream&, TPolynom<T>&);
    template <class T>
    friend QString& operator<<(QString&, TPolynom<T>&);
    void calcCoefFromRoots();
    int changeArrRootSize(unsigned newSize); // Возврщает int - кол-во добавленных эл-тов
    bool changeRootByIndex(unsigned index, number newRoot);
    void printRoots();
    number getCanonicCoef();
};

template <class number>
TPolynom<number>::TPolynom(number canonicCoef) {
    this->canonicCoef = canonicCoef;
    this->printMode = EPrintMode::EPrintModeClassic;
    this->arrCoef = new TArray<number>();
    this->arrRoot = new TArray<number>();
}

template <class number>
TPolynom<number>::TPolynom(const QString& qstrPolynom) {
    this->printMode = EPrintMode::EPrintModeClassic;
    this->arrCoef = new TArray<number>();
    this->arrRoot = new TArray<number>();

    // Удаляем пробелы из строки
    QString polynomStr = qstrPolynom.simplified();

    // Проверяем, начинается ли строка с "P(x)="
    const QString prefix = "P(x)=";
    if (polynomStr.startsWith(prefix)) {
        polynomStr.remove(0, prefix.length()); // Убираем префикс
    }

    // Регулярное выражение для извлечения коэффициентов и корней
    QRegularExpression regex(R"(\(([^)]+)\))");
    QRegularExpressionMatchIterator it = regex.globalMatch(polynomStr);
    it.next();

    // Извлечение коэффициента
    if (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString coefStr = match.captured(1);
        number coef;
        coefStr >> coef;
        this->setCanonicCoef(coef);
    }

    // Извлечение корней
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString rootStr = match.captured(1);

        // Удаляем "x - " если присутствует
        if (rootStr.startsWith("x-")) {
            rootStr.remove(0, 2); // Убираем "x - "
        }
        if (!rootStr.isEmpty()) {
            number root; // Функция для парсинга комплексного числа
            rootStr >> root;
            arrRoot->appendElement(root); // Добавляем корень в массив
        }

    }

    this->calcCoefFromRoots();
}

template <class number>
TPolynom<number>::~TPolynom() {
    this->flushMemory();
}

template <class number>
unsigned TPolynom<number>::getRootsCount() {
    return this->arrRoot->getSize();
}

template <class number>
void TPolynom<number>::addCoef(number coef) {
    this->arrCoef->appendElement(coef);
}

template <class number>
void TPolynom<number>::addRoot(number root) {
    this->arrRoot->appendElement(root);
    this->calcCoefFromRoots();
}


template <class number>
number TPolynom<number>::value (number val) {
    number result = 0;
    for (unsigned i = 0; i < this->arrCoef->getSize(); i++) {
        int rootPow = (this->arrCoef->getSize() - i - 1);
        result += this->arrCoef->get(i) * pow(val, rootPow);
    }

    return result;
}

template <class number>
ostream& operator<<(ostream& os, TPolynom<number>& polynom) {
    cout << "P(x) = ";

    if (!polynom.arrRoot->getSize()) {
        cout << polynom.canonicCoef;
        return os;
    }

    if (polynom.printMode == EPrintMode::EPrintModeCanonical) {
        for (unsigned i = 0; i < polynom.arrCoef->getSize() - 1; i++) {
            int pow = (polynom.arrCoef->getSize() - i - 1);
            os << polynom.arrCoef->get(i);
            if (pow != 0) os << "x";
            if (pow > 1) os << "^" << pow;
            os << " + ";
        }
        cout << polynom.arrCoef->get(polynom.arrCoef->getSize() - 1);

    } else {
        cout << polynom.canonicCoef;
        for (unsigned i = 0; i < polynom.arrRoot->getSize(); ++i)
        {
            cout << "(x - "  << polynom.arrRoot->get(i) << ")";
        }

    }

    os << "\n";

    return os;
}

template <class number>
QString& operator<<(QString& s, TPolynom<number>& polynom) {
    s += "P(x) = ";

    if (!polynom.arrRoot->getSize()) {
        s << polynom.canonicCoef;
        return s;
    }

    if (polynom.printMode == EPrintMode::EPrintModeCanonical) {
        for (unsigned i = 0; i < polynom.arrCoef->getSize() - 1; i++) {
            int pow = (polynom.arrCoef->getSize() - i - 1);
            s << polynom.arrCoef->get(i);
            if (pow != 0) s += "x";
            if (pow > 1) {
                s += "^";
                s += QString().setNum(pow);
            }
            s += " + ";
        }
        s << polynom.arrCoef->get(polynom.arrCoef->getSize() - 1);

    } else {
        s << polynom.canonicCoef;
        for (unsigned i = 0; i < polynom.arrRoot->getSize(); ++i)
        {
            s += "(x - ";
            s << polynom.arrRoot->get(i);
            s += ")";
        }

    }

    // s += " "; // проверить - нужен ли

    return s;
}

template <class number>
void TPolynom<number>::flushMemory() {
    this->arrCoef->flushMemory();
    this->arrRoot->flushMemory();
}

template <class number>
void TPolynom<number>::setPrintMode(EPrintMode mode) {
    this->printMode = mode;
}

template <class number>
void TPolynom<number>::setCanonicCoef(number coef) {
    this->canonicCoef = coef;
}

template <class number>
void TPolynom<number>::calcCoefFromRoots() {
    TArray<number> classicalCoeffs; // Начинаем с единичного полинома
    classicalCoeffs.appendElement(1); // Начальный коэффициент для x^0

    for (unsigned i = 0; i < this->arrRoot->getSize(); i++) {
        TArray<number> newCoeffs;
        newCoeffs.fillArray(classicalCoeffs.getSize() + 1);

        for (unsigned j = 0; j < classicalCoeffs.getSize(); j++) {
            newCoeffs.replaceElement(j, newCoeffs.get(j) + classicalCoeffs.get(j)); // Сохраняем старые коэффициенты
            newCoeffs.replaceElement(j + 1, newCoeffs.get(j + 1) + (-this->arrRoot->get(i) * classicalCoeffs.get(j)) ); // Умножаем на (x - root)
        }

        classicalCoeffs.fillArray(newCoeffs.getSize());
        for (unsigned t = 0; t < newCoeffs.getSize(); t++) { // Обновляем коэффициенты
            classicalCoeffs.replaceElement(t, newCoeffs.get(t));
        }
    }

    // Умножаем на канонический коэффициент
    for (int i = 0; i < (int)classicalCoeffs.getSize(); i++) {
        classicalCoeffs.replaceElement(i, classicalCoeffs.get(i) * this->canonicCoef);
    }

    // Сохраняем классические коэффициенты
    this->arrCoef->flushMemory();
    for (int i = 0; i < (int)classicalCoeffs.getSize(); i++) {
        this->arrCoef->appendElement(classicalCoeffs.get(i));
    }

}

template <class number>
int TPolynom<number>::changeArrRootSize(unsigned newSize) {
    int addedCount = newSize - this->arrRoot->getSize();

    this->arrRoot->resizeArray(newSize);
    this->calcCoefFromRoots();

    return addedCount;
}

template <class number>
bool TPolynom<number>::changeRootByIndex(unsigned index, number newRoot)
{
    return arrRoot->replaceElement(index, newRoot);
}

template <class number>
void TPolynom<number>::printRoots()
{
    arrRoot->print();
}

template <class number>
number TPolynom<number>::getCanonicCoef()
{
    return canonicCoef;
}

#endif // POLYNOM_H
