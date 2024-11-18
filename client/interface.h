#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include "common.h"

class TInterface : public QWidget
{
    Q_OBJECT

public:
    TInterface(QWidget *parent = nullptr);
    ~TInterface();

public slots:
    void clearOutput(); // Очистить поле вывода
    void showCanonicalForm(); // Отправить запрос на вывод канонического вида полинома
    void showClassicalForm(); // Отправить запрос на вывод классического вида полинома
    void changeRootsCount(const QString& inputText); // Отправить запрос на изменение количества корней
    void calculateValueAtX(const QString& x); // Отправить запрос на вычисление значения в точке x
    void setNewPolynomial(QString& anText, QString& rootsText); // Отправить запрос на задание нового полинома
    void changeRootAndAN(QString& anText, QString& indexText); // Отправить запрос на изменение a_n и корня по индексу
    void exitApplication(); // Выход из приложения
    void answer(const QString& response); // Обработка ответа от сервера

signals:
    void request(QString);
private:
    QString strPolynom; // Строка для хранения текущего полинома

    QRadioButton *c_mode; // Режим полинома - комплексные числа
    QRadioButton *r_mode; // Режим полинома - вещественные числа
    QHBoxLayout *radioButtonsLayout;

    QLineEdit *outputField;
    QPushButton *clearButton;
    QHBoxLayout *outputLayout;
    QVBoxLayout *mainLayout;

    QLabel *canonicalFormLabel;
    QPushButton *canonicalFormButton;
    QHBoxLayout *canonicalFormLayout;

    QLabel *classicalFormLabel;
    QPushButton *classicalFormButton;
    QHBoxLayout *classicalFormLayout;

    QLabel *changeRootsCountLabel;
    QLineEdit *changeRootsCountInput;
    QPushButton *changeRootsCountButton;
    QHBoxLayout *changeRootsCountLayout;

    QLabel *newANAndRootsLabel;
    QLineEdit *newANInput;
    QLineEdit *newRootIndexInput;
    QPushButton *newANAndRootsButton;
    QHBoxLayout *newANAndRootsLayout;

    QLabel *calculateValueAtXLabel;
    QLineEdit *calculateValueAtXInput;
    QPushButton *calculateValueAtXButton;
    QHBoxLayout *calculateValueAtXLayout;

    QLabel *setNewPolynomialLabel;
    QLineEdit *setNewPolynomialANInput;
    QLineEdit *setNewPolynomialRootsInput;
    QPushButton *setNewPolynomialButton;
    QHBoxLayout *setNewPolynomialLayout;

    QPushButton *exitButton;

    void clearStrPolynom(); // Сброс текущего состояния полинома у клиента
    void formRequest(RequestType requestType, const QString& params = ""); // Метод для отправки запроса на сервер с параметрами
};

#endif // INTERFACE_H
