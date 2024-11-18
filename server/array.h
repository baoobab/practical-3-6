#ifndef ARRAY_H
#define ARRAY_H

#include <cmath>
#include <iostream>

using namespace std;


template <class number>
class TArray {
private:
    unsigned size;
    number* arr;

    void quickSortHelper(int low, int high);
    void quickSortReverseHelper(int low, int high);
    bool checkIndex(unsigned int index);
public:
    TArray();
    ~TArray();
    unsigned getSize();
    void appendElement(number el);
    void print();
    void resizeArray(unsigned newSize);
    number mediumValue();
    number standardDeviation();
    void sort();
    void reverseSort();
    void flushMemory();
    bool replaceElement(unsigned int index, number value);
    void removeElementByIndex(unsigned int index);
    void fillArray(unsigned newSize, number el = 0);
    number get(unsigned idx);
};

template <class number>
TArray<number>::TArray() {
    this->size = 0;
    this->arr = 0;
}

template <class number>
TArray<number>::~TArray() {
    this->flushMemory();
}

template <class number>
unsigned TArray<number>::getSize() {
    return this->size;
}

template <class number>
bool TArray<number>::checkIndex(unsigned int index) {
    return (index < this->size);
}

template <class number>
void TArray<number>::flushMemory() {
    delete[] this->arr;
    this->size = 0;
    this->arr = 0;
}

template <class number>
void TArray<number>::fillArray(unsigned newSize, number el) {
    number* newArr = new number[newSize];

    for (number* curr = newArr; curr != (newArr + newSize); curr++) {
        *curr = el;
    }
    delete[] this->arr;

    this->arr = newArr;
    this->size = newSize;
}

template <class number>
void TArray<number>::appendElement(number el) {
    number* newArr = new number[this->size + 1];
    unsigned cnt = 0;

    for (number* curr = this->arr; curr != (this->arr + this->size); curr++) {
        *(newArr + cnt++) = *curr;
    }
    *(newArr + cnt) = el;

    delete[] this->arr;

    this->arr = newArr;
    this->size++;
}

template <class number>
void TArray<number>::removeElementByIndex(unsigned index) {
    if (!checkIndex(index) || !this->size) return; // Если такого индекса нет, или массив пустой - выходим

    number* newArr = new number[this->size - 1];
    unsigned cnt = 0;
    for (number* curr = this->arr; curr != (this->arr + this->size); curr++) {
        if (index-- == 0) continue; // скипаем удаляемый элемент
        *(newArr + cnt++) = *curr;
    }

    delete[] this->arr;

    this->arr = newArr;
    this->size--;
}

template <class number>
void TArray<number>::print() {
    if (!this->size) {
        cout << "Array is empty" << "\n";
        return;
    }
    cout << "Array (size is " << this->size << "): ";
    for (number* curr = this->arr; curr != (this->arr + this->size); curr++) {
        cout << *curr << " ";
    }
    cout << "\n";
}

template <class number>
number TArray<number>::mediumValue() {
    if (!this->size) return 0;

    number summ = 0;
    for (number* curr = this->arr; curr != (this->arr + this->size); curr++) {
        summ += *curr;
    }
    return summ/this->size;

}

template <class number>
number TArray<number>::standardDeviation() {
    if (!this->size || this->size == 1) return 0;

    number mediumValue = this->mediumValue();
    number standardSumm = 0;

    for (number* curr = this->arr; curr != (this->arr + this->size); curr++) {
        standardSumm += pow(*curr - mediumValue, 2);
    }

    return sqrt(standardSumm / (this->size - 1));
}

template <class number>
void TArray<number>::quickSortHelper(int low, int high) {
    if (this->size <= 1) return;

    int i = low;
    int j = high;
    number pivot = *(this->arr+((i + j) / 2));
    number temp;

    while (i <= j) {
        while (*(this->arr+i) < pivot)
            i++;
        while (*(this->arr+j) > pivot)
            j--;
        if (i <= j) {
            temp = *(this->arr+i);
            *(this->arr+i) = *(this->arr+j);
            *(this->arr+j) = temp;
            i++;
            j--;
        }
    }
    if (j > low)
        this->quickSortHelper(low, j);
    if (i < high)
        this->quickSortHelper(i, high);
}

template <class number>
void TArray<number>::quickSortReverseHelper(int low, int high) {
    if (this->size <= 1) return;

    int i = low;
    int j = high;
    number pivot = *(this->arr + ((i + j) / 2));
    number temp;

    while (i <= j) {
        while (*(this->arr + i) > pivot)
            i++;
        while (*(this->arr + j) < pivot)
            j--;
        if (i <= j) {
            temp = *(this->arr + i);
            *(this->arr + i) = *(this->arr + j);
            *(this->arr + j) = temp;
            i++;
            j--;
        }
    }

    if (j > low)
        this->quickSortReverseHelper(low, j);
    if (i < high)
        this->quickSortReverseHelper(i, high);
}

template <class number>
void TArray<number>::sort() {
    this->quickSortHelper(0, this->size - 1);
}

template <class number>
void TArray<number>::reverseSort() {
    this->quickSortReverseHelper(0, this->size - 1);
}

template <class number>
bool TArray<number>::replaceElement(unsigned int index, number value) {
    if (!checkIndex(index)) {
        cout << "\nIndex out of range\n";
        return false;
    }

    *(this->arr + index) = value;
    return true;
}

template <class number>
void TArray<number>::resizeArray(unsigned int newSize) {
    int elementsToAppend = newSize - this->size;

    if (elementsToAppend < 0) {
        while (elementsToAppend++) {
            this->removeElementByIndex(this->size - 1);
        }
    } else if (elementsToAppend > 0) {
        while (elementsToAppend--) {
            this->appendElement(0);
        }
    }

    cout << "Current dimension is: " << newSize << "\n";
    return;
}

template <class number>
number TArray<number>::get(unsigned index) {
    if (!checkIndex(index)) {
        throw out_of_range("out");
    }

    return *(this->arr + index);
}

#endif // ARRAY_H
