#include "interface.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRadioButton>
#include <QApplication>
#include "common.h"
#include <QtDebug>

TInterface::TInterface(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Многочлен на комплексных числах");
    setMaximumSize(600, 300);

    // Создаём метку и поле вывода
    outputField = new QLineEdit(this);
    outputField->setReadOnly(true);
    outputField->setPlaceholderText("Результат любого пункта будет здесь...");
    outputField->setMaximumWidth(350);

    // Кнопка "Очистить"
    QPushButton *clearButton = new QPushButton("Очистить", this);
    connect(clearButton, &QPushButton::clicked, this, &TInterface::clearOutput);

    // Создаем горизонтальный макет для метки и поля вывода
    QHBoxLayout *outputLayout = new QHBoxLayout();
    outputLayout->addWidget(outputField);
    outputLayout->addWidget(clearButton);

    // Создаем вертикальный макет для всего интерфейса
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(outputLayout);

    c_mode = new QRadioButton("Комплексные числа", this);
    r_mode = new QRadioButton("Вещественные числа", this);

    // Установка начального состояния радиокнопок
    c_mode->setChecked(true); // По умолчанию выбираем режим комплексных чисел
    r_mode->setChecked(false);

    // Добавление радиокнопок в макет
    QHBoxLayout *radioButtonsLayout = new QHBoxLayout();
    radioButtonsLayout->addWidget(c_mode);
    radioButtonsLayout->addWidget(r_mode);
    mainLayout->addLayout(radioButtonsLayout);

    // Пункт 1: Вывод канонического вида полинома
    QLabel *canonicalFormLabel = new QLabel("Вывод канонического вида полинома", this);
    QPushButton *canonicalFormButton = new QPushButton("Вывести", this);
    connect(canonicalFormButton, &QPushButton::clicked, this, &TInterface::showCanonicalForm);

    QHBoxLayout *canonicalFormLayout = new QHBoxLayout();
    canonicalFormLayout->addWidget(canonicalFormLabel);
    canonicalFormLayout->addWidget(canonicalFormButton);
    mainLayout->addLayout(canonicalFormLayout);

    // Пункт 2: Вывод классического вида полинома
    QLabel *classicalFormLabel = new QLabel("Вывод классического вида полинома", this);
    QPushButton *classicalFormButton = new QPushButton("Вывести", this);
    connect(classicalFormButton, &QPushButton::clicked, this, &TInterface::showClassicalForm);

    QHBoxLayout *classicalFormLayout = new QHBoxLayout();
    classicalFormLayout->addWidget(classicalFormLabel);
    classicalFormLayout->addWidget(classicalFormButton);
    mainLayout->addLayout(classicalFormLayout);

    // Пункт 3: Изменение количества корней
    QLabel *changeRootsCountLabel = new QLabel("Изменение количества корней", this);
    QLineEdit *changeRootsCountInput = new QLineEdit(this);

    changeRootsCountInput->setPlaceholderText("Новое количество корней");

    QPushButton *changeRootsCountButton = new QPushButton("Изменить", this);
    connect(changeRootsCountButton, &QPushButton::clicked, [this, changeRootsCountInput]() {
        QString inputText = changeRootsCountInput->text();
        changeRootsCount(inputText);
        changeRootsCountInput->clear();
    });

    QHBoxLayout *changeRootsCountLayout = new QHBoxLayout();
    changeRootsCountLayout->addWidget(changeRootsCountLabel);
    changeRootsCountLayout->addWidget(changeRootsCountInput);
    changeRootsCountLayout->addWidget(changeRootsCountButton);

    mainLayout->addLayout(changeRootsCountLayout);

    // Пункт 4: Изменить a_n и корень по индексу
    QLabel *newANAndRootsLabel = new QLabel("Изменить a_n и корень по индексу", this);

    QLineEdit *newANInput = new QLineEdit(this);
    newANInput->setPlaceholderText("Новый a_n");

    QLineEdit *newRootIndexInput = new QLineEdit(this);
    newRootIndexInput->setPlaceholderText("Индекс корня");

    QPushButton *newANAndRootsButton = new QPushButton("Изменить", this);

    connect(newANAndRootsButton, &QPushButton::clicked, [this, newANInput, newRootIndexInput]() {
        QString anText = newANInput->text();
        QString indexText = newRootIndexInput->text();
        changeRootAndAN(anText, indexText);
        newANInput->clear();
        newRootIndexInput->clear();
    });

    QHBoxLayout *newANAndRootsLayout = new QHBoxLayout();
    newANAndRootsLayout->addWidget(newANAndRootsLabel);
    newANAndRootsLayout->addWidget(newANInput);
    newANAndRootsLayout->addWidget(newRootIndexInput);
    newANAndRootsLayout->addWidget(newANAndRootsButton);

    mainLayout->addLayout(newANAndRootsLayout);

    // Пункт 5: Вычислить значение в точке x
    QLabel *calculateValueAtXLabel = new QLabel("Вычислить значение в точке x", this);
    QLineEdit *calculateValueAtXInput = new QLineEdit(this);

    calculateValueAtXInput->setPlaceholderText("Точка x");

    QPushButton *calculateValueAtXButton = new QPushButton("Вычислить", this);
    connect(calculateValueAtXButton, &QPushButton::clicked, [this, calculateValueAtXInput]() {
        QString inputText = calculateValueAtXInput->text();
        calculateValueAtX(inputText);
        calculateValueAtXInput->clear();
    });

    QHBoxLayout *calculateValueAtXLayout = new QHBoxLayout();
    calculateValueAtXLayout->addWidget(calculateValueAtXLabel);
    calculateValueAtXLayout->addWidget(calculateValueAtXInput);
    calculateValueAtXLayout->addWidget(calculateValueAtXButton);

    mainLayout->addLayout(calculateValueAtXLayout);

    // Пункт 6: Задать новый полином
    QLabel *setNewPolynomialLabel = new QLabel("Задать новый полином", this);
    QLineEdit *setNewPolynomialANInput = new QLineEdit(this);
    setNewPolynomialANInput->setPlaceholderText("a_n");

    QLineEdit *setNewPolynomialRootsInput = new QLineEdit(this);
    setNewPolynomialRootsInput->setPlaceholderText("корни (через пробел)");

    QPushButton *setNewPolynomialButton = new QPushButton("Задать", this);

    connect(setNewPolynomialButton, &QPushButton::clicked, [this, setNewPolynomialANInput, setNewPolynomialRootsInput]() {
        QString anText = setNewPolynomialANInput->text();
        QString rootsText = setNewPolynomialRootsInput->text();
        setNewPolynomial(anText, rootsText);
        setNewPolynomialANInput->clear();
        setNewPolynomialRootsInput->clear();
    });

    QHBoxLayout *setNewPolynomialLayout = new QHBoxLayout();
    setNewPolynomialLayout->addWidget(setNewPolynomialLabel);
    setNewPolynomialLayout->addWidget(setNewPolynomialANInput);
    setNewPolynomialLayout->addWidget(setNewPolynomialRootsInput);
    setNewPolynomialLayout->addWidget(setNewPolynomialButton);

    mainLayout->addLayout(setNewPolynomialLayout);

    // Добавляем кнопку "Выход"
    QPushButton *exitButton = new QPushButton("Выход", this);
    mainLayout->addWidget(exitButton);

    connect(exitButton, &QPushButton::clicked, this, &TInterface::exitApplication);

    // Установка основного макета
    setLayout(mainLayout);

    // strPolynom используется для сохранения текущего состояния полинома, очищаем её
    clearStrPolynom();

    tempOutputField = nullptr;
}

TInterface::~TInterface()
{
    delete tempOutputField;

    delete c_mode;
    delete r_mode;
    delete radioButtonsLayout;

    delete outputField;
    delete clearButton;
    delete outputLayout;
    delete mainLayout;

    delete canonicalFormLabel;
    delete canonicalFormButton;
    delete canonicalFormLayout;

    delete classicalFormLabel;
    delete classicalFormButton;
    delete classicalFormLayout;

    delete changeRootsCountLabel;
    delete changeRootsCountInput;
    delete changeRootsCountButton;
    delete changeRootsCountLayout;

    delete newANAndRootsLabel;
    delete newANInput;
    delete newRootIndexInput;
    delete newANAndRootsButton;
    delete newANAndRootsLayout;

    delete calculateValueAtXLabel;
    delete calculateValueAtXInput;
    delete calculateValueAtXButton;
    delete calculateValueAtXLayout;

    delete setNewPolynomialLabel;
    delete setNewPolynomialANInput;
    delete setNewPolynomialRootsInput;
    delete setNewPolynomialButton;
    delete setNewPolynomialLayout;

    delete exitButton;
}

void TInterface::clearStrPolynom() {
    strPolynom = "P(x) = 1";
}

void TInterface::showCanonicalForm()
{
    formRequest(CANONICAL_FORM_REQUEST); // Отправляем запрос на вывод канонического вида
}

void TInterface::showClassicalForm()
{
    formRequest(CLASSICAL_FORM_REQUEST); // Отправляем запрос на вывод классического вида
}

void TInterface::changeRootsCount(const QString& count)
{
    bool ok;
    int size = count.toInt(&ok);

    if (!ok || size <= 0)
    {
        QMessageBox::critical(this, "Ошибка", "Некорректный размер нового полинома");
        return;
    }

    // Создаем диалоговое окно для изменения размера массива
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Изменить количество корней");

    // Метка и readonly поле для старого полинома
    QLabel* oldRootLabel = new QLabel("Текущий полином (нумерация корней с нуля):", dialog);
    QLineEdit* oldRootField = new QLineEdit(dialog);

    formRequest(CLASSICAL_FORM_REQUEST);
    oldRootField->setText(strPolynom);
    oldRootField->setReadOnly(true);

    QString newRootLabelText = "Введите новые корни (через пробел): "  ;
    QLabel* newRootLabel = new QLabel(newRootLabelText, dialog);
    QLineEdit* newRootInput = new QLineEdit(dialog);

    // Кнопка подтверждения
    QPushButton* confirmButton = new QPushButton("Подтвердить", dialog);
    connect(confirmButton, &QPushButton::clicked, dialog, &QDialog::accept);



    // Макет для диалогового окна
    QVBoxLayout* dialogLayout = new QVBoxLayout();
    dialogLayout->addWidget(oldRootLabel);
    dialogLayout->addWidget(oldRootField);
    dialogLayout->addWidget(newRootLabel);
    dialogLayout->addWidget(newRootInput);
    dialogLayout->addWidget(confirmButton);
    dialog->setLayout(dialogLayout);

    // Показываем диалоговое окно и ждем подтверждения
    if (dialog->exec() == QDialog::Accepted)
    {
        QString textofReq = QString::number(size);
        textofReq +=  ";" + newRootInput->text();
        formRequest(CHANGE_ROOTS_COUNT_REQUEST, textofReq);

    }

    delete dialog;
}

void TInterface::changeRootAndAN(QString& anText, QString& indexText)
{
    if (anText.length() > 0)
    {
        formRequest(SET_CANONIC_COEF_REQUEST, anText);
    }

    bool ok;
    int index = indexText.toInt(&ok);

    if (!ok || index < 0)
    {
        QMessageBox::information(this, "Инфо", "Корни не переданы, изменен только a_n");
        return;
    }

    // Создаем диалоговое окно для изменения корня
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Изменить корень");

    // Метка и readonly поле для старого корня
    QLabel* oldRootLabel = new QLabel("Текущий полином (нумерация корней с нуля):", dialog);
    QLineEdit* oldRootField = new QLineEdit(dialog);
    tempOutputField = oldRootField;

    oldRootField->setText(strPolynom);
    oldRootField->setReadOnly(true);

    // Поле ввода для нового корня
    QString newRootLabelText = "Новый корень по индексу " + QString::number(index) + ":";
    QLabel* newRootLabel = new QLabel(newRootLabelText, dialog);
    QLineEdit* newRootInput = new QLineEdit(dialog);

    // Кнопка подтверждения
    QPushButton* confirmButton = new QPushButton("Подтвердить", dialog);
    connect(confirmButton, &QPushButton::clicked, dialog, &QDialog::accept);

    // Макет для диалогового окна
    QVBoxLayout* dialogLayout = new QVBoxLayout();
    dialogLayout->addWidget(oldRootLabel);
    dialogLayout->addWidget(oldRootField);
    dialogLayout->addWidget(newRootLabel);
    dialogLayout->addWidget(newRootInput);
    dialogLayout->addWidget(confirmButton);
    dialog->setLayout(dialogLayout);

    // Показываем диалоговое окно и ждем подтверждения
    if (dialog->exec() == QDialog::Accepted)
    {
        QString reqText = QString::number(index) + separator + newRootInput->text();
        formRequest(CHANGE_ROOT_REQUEST, reqText);
    }

    delete dialog;
}

void TInterface::calculateValueAtX(const QString& x)
{
       formRequest(CALCULATE_VALUE_AT_X_REQUEST, x);
}

void TInterface::setNewPolynomial(QString& anText, QString& rootsText)
{
    clearStrPolynom(); // Сброс текущего состояния полинома

    if (anText.length() == 0 && rootsText.length() == 0)
    {
        QMessageBox::information(this, "Инфо", "Поля пустые, полином сброшен");
        clearOutput();
        return;
    }

    if (anText.length() > 0 && rootsText.length() > 0)
    {
        formRequest(SET_NEW_POLYNOMIAL_REQUEST, anText + separator + rootsText);
        return;
    }

    if (anText.length() > 0) {
        formRequest(SET_CANONIC_COEF_REQUEST, anText);
    }

    if (rootsText.length() > 0)
    {
        formRequest(ADD_ROOTS_REQUEST, rootsText);
    }
}

// Общий вид запроса: <РежимЧиселПолинома;ТипЗапроса;Полином;ОстальныеПараметры>
void TInterface::formRequest(RequestType requestType, const QString& params)
{
    if (r_mode->isChecked()) {
        emit request(QString::number(REAL_MODE) + separator+ QString::number(requestType) + separator + strPolynom + separator + params);
    } else {
        emit request(QString::number(COMPLEX_MODE) + separator+ QString::number(requestType) + separator + strPolynom + separator + params);
    }

    // Эмитируем сигнал запроса для отправки на сервер.
}

void TInterface::answer(const QString& response)
{

    char separatorChar = separator.toLatin1(); // Преобразование QChar в char
    int pos = response.indexOf(separatorChar); // Используем response для поиска сепаратора

    // Проверка на наличие разделителя
    if (pos == -1) {
        qDebug() << "Separator not found in message.";
        return;
    }

    int requestType = response.left(pos).toInt(); // Получаем тип запроса
    QString strMsg = response.mid(pos + 1); // Убираем тип запроса из сообщения

    const QString statusCode = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Получаем статус код из запроса
    strMsg = strMsg.remove(0, strMsg.indexOf(separatorChar) + 1); // Убираем статус код из запроса

    switch (requestType) {
    case CANONICAL_FORM_ANSWER:
        outputField->setText(strMsg);
        break;
    case CLASSICAL_FORM_ANSWER:
        strPolynom = strMsg; // Остальная часть ответа - полином
        outputField->setText(strPolynom);
        break;
    case SET_CANONIC_COEF_ANSWER:
        if (statusCode == "ERR")
        {
            outputField->setText("error in SET_CANONIC_COEF_ANSWER");
            break;
        }

        strPolynom = strMsg; // Остальная часть ответа - полином
        outputField->setText(strPolynom);
        if (tempOutputField) tempOutputField->setText(strPolynom);
        tempOutputField = nullptr;
        break;
    case SET_NEW_POLYNOMIAL_ANSWER:
    {
        strPolynom = strMsg; // Остальная часть ответа - полином
        outputField->setText(strPolynom);
        break;
    }
    case CHANGE_ROOT_ANSWER:
        if (statusCode == "ERR")
        {
            QMessageBox::critical(this, "Ошибка", "Корень не изменился, проверьте правильность ввода");
            break;
        }

        strPolynom = strMsg; // Остальная часть ответа - полином
        outputField->setText(strPolynom);
        QMessageBox::information(this, "Успех", "Корень изменён успешно");
        break;
    case ADD_ROOTS_ANSWER:
    {
        strPolynom = strMsg; // Остальная часть ответа - полином
        outputField->setText(strPolynom);
        break;
    }

    case CALCULATE_VALUE_AT_X_ANSWER:
    {
        // Извлекаем индекс корня (первый параметр)
        QString valP = strMsg.mid(0, strMsg.indexOf(separatorChar));
        // Удаляем первую часть строки до первого разделителя
        strMsg.remove(0, strMsg.indexOf(separatorChar) + 2);

        // Извлекаем второй параметр (сам корень)
        QString valX = strMsg.mid(0, strMsg.indexOf(separatorChar));

        // Если второй параметр - это последнее значение в строке, и нет второго разделителя
        if (valX.isEmpty()) {
            valX = strMsg; // Берем всю оставшуюся строку
        }
        QString outpuPx = "P(" + valX + ") = " + valP;
        // Извлекаем значение в точке х и выводим его
        outputField->setText(outpuPx);
        break;
    }

    case CHANGE_ROOTS_COUNT_ANSWER:
    {
        if (statusCode == "ERR1")
        {
            QMessageBox::critical(this, "Ошибка", "Количество корней не может быть отрицательным");
            break;
        }

        if (statusCode == "ERR2")
        {
            QMessageBox::critical(this, "Ошибка", "Количество корней совпадает с исходным");
            break;
        }

        if (statusCode == "ERR3")
        {
            QMessageBox::critical(this, "Ошибка", "Количество введенных корней не совпадает с выбранным");
            break;
        }

        strPolynom = strMsg; // Остальная часть ответа - полином
        outputField->setText(strPolynom);
        QMessageBox::information(this, "Успех", "Полином изменён успешно");
        break;
    }
    default:
        outputField->setText("Неизвестный ответ.");
        break;
    }
}

void TInterface::clearOutput()
{
    outputField->clear(); // Очистка поля вывода
}

void TInterface::exitApplication()
{
    QApplication::quit(); // Завершение приложения
}
