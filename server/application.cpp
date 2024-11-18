#include "application.h"
#include "polynom.h"
#include "complex.h"
#include "common.h"
#include "realnumber.h"
#include <QtDebug>

TApplication::TApplication(int argc, char *argv[])
    : QCoreApplication(argc, argv)
{
    TCommParams pars = { QHostAddress("127.0.0.1"), 10000,
                        QHostAddress("127.0.0.1"), 10001 };
    comm = new TCommunicator(pars, this);
    qDebug() << "server started";
    connect(comm, SIGNAL(recieved(QByteArray)), this, SLOT(recieve(QByteArray)));
}

void TApplication::recieve(QByteArray msg)
{
    QString answer; // Тут будет ответ
    QString strMsg = QString(msg); // Извлечение параметров из сообщения

    char separatorChar = separator.toLatin1(); // Преобразование QChar в char
    int pos = strMsg.indexOf(separatorChar);

    // Проверка на наличие разделителя
    if (pos == -1) {
        qDebug() << "Separator not found in message.";
        return; // Обработка случая, когда разделитель не найден
    }

    int polynomMode = strMsg.left(pos).toInt(); // Получаем режим полинома
    strMsg = strMsg.mid(pos + 1); // Убираем режим полинома из сообщения
    qDebug() << "Pmode " << polynomMode;

    switch (polynomMode) {
    case COMPLEX_MODE: {
        answer = handleComplex(strMsg);
        break;
    }
    case REAL_MODE: {
        answer = handleReal(strMsg);
        break;
    }
    default: {
        answer = handleReal(strMsg);
        break; // Когда режим полинома не передан - работаем как в вещ. числами
    }
    }

    comm->send(QByteArray().append(answer.toUtf8())); // Отправляем ответ обратно клиенту
}

QString TApplication::handleComplex(QString &strMsg) {
    QString answer; // Тут будет результат обработки

    char separatorChar = separator.toLatin1(); // Преобразование QChar в char
    int pos = strMsg.indexOf(separatorChar);

    int requestType = strMsg.left(pos).toInt(); // Получаем тип запроса
    strMsg = strMsg.mid(pos + 1); // Убираем тип запроса из сообщения

    // Проверяем, начинается ли параметр "P(x) = "
    const QString prefix = "P(x) = ";
    if (!strMsg.startsWith(prefix)) {
        // Иначе ошибка, тк полином должен идти первым параметром
        qDebug() << "Polynom not found as first param in message.";
        return "";
    }

    QString param = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Получаем первый параметр - это должен быть полином

    TPolynom<TComplex> p(param); // Создание полинома
    p.setPrintMode(EPrintMode::EPrintModeClassic); // Ставим EPrintModeClassic мод для корректной работы

    strMsg.remove(0, strMsg.indexOf(separatorChar) + 1); // Убираем полином из сообщения

    switch (requestType)
    {
    case CANONICAL_FORM_REQUEST:
    {
        p.setPrintMode(EPrintMode::EPrintModeCanonical);
        answer << QString().setNum(CANONICAL_FORM_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case CLASSICAL_FORM_REQUEST:
    {
        answer << QString().setNum(CLASSICAL_FORM_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case CHANGE_ROOTS_COUNT_REQUEST:
    {
        // Извлекаем индекс корня (первый параметр)
        QString StrNewRootsCount = strMsg.mid(0, strMsg.indexOf(separatorChar));
        // Удаляем первую часть строки до первого разделителя
        strMsg.remove(0, strMsg.indexOf(separatorChar) + 1);

        // Извлекаем второй параметр (сам корень)
        QString strNewRoots = strMsg.mid(0, strMsg.indexOf(separatorChar));

        // Если второй параметр - это последнее значение в строке, и нет второго разделителя
        if (strNewRoots.isEmpty()) {
            strNewRoots = strMsg; // Берем всю оставшуюся строку
        }

        int newRootsCount = StrNewRootsCount.toInt();
        int addedCount = newRootsCount - p.getRootsCount();

        if (newRootsCount <= 0)
        {
            answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "ERR1" << p; // Ответ формата - код_запроса;статус
            break;
        }

        // если размер массива уменьшился
        if (addedCount < 0)
        {
            p.changeArrRootSize(newRootsCount);
            answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус
            break;
        }

        // если размер массива не изменился
        if (addedCount == 0)
        {
            answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "ERR2" << p; // Ответ формата - код_запроса;статус
            break;
        }

        // логика обработки корней
        QStringList rootsList = strNewRoots.split(' '); // Разделяем строку на части по пробелу
        if (rootsList.size() != addedCount * 2)
        {
            answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "ERR3" << p; // Ответ формата - код_запроса;статус
            break;
        }

        QString arr[2];
        int iter = 0;
        for (QString& item : rootsList)
        {
            if (!item.isEmpty())
            { // Проверяем, что часть не пустая
                arr[iter++] = item;
            }
            if (iter == 2)
            {
                QString concaetedNum;
                TComplex tmpNum;
                concaetedNum = arr[0] + " " + arr[1];
                concaetedNum >> tmpNum;

                p.changeArrRootSize(p.getRootsCount() + 1);
                p.changeRootByIndex(p.getRootsCount() - 1, tmpNum);
                p.calcCoefFromRoots();

                iter = 0;
            }
        }
        answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case CHANGE_ROOT_REQUEST:
    {
        QString index = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Первый параметр (после полинома) - индекс
        QString strNewRoot = strMsg.mid(strMsg.indexOf(separatorChar)); // Второй параметр (после полинома) - сам корень
        TComplex newRoot;
        strNewRoot >> newRoot;

        bool isChanged = p.changeRootByIndex(index.toInt(), newRoot);

        if (!isChanged) { // Обработка ошибки - если корень не изменился
            answer << QString().setNum(CHANGE_ROOT_ANSWER) << "ERR"; // Ответ формата - код_запроса;статус
            break;
        }

        answer << QString().setNum(CHANGE_ROOT_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }

    case CALCULATE_VALUE_AT_X_REQUEST:
    {
        QString strX = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Первый параметр (после полинома) - точка x
        TComplex x;
        strX >> x;
        answer << QString().setNum(CALCULATE_VALUE_AT_X_ANSWER) << "OK" << p.value(x) << QString(';') << x; // Ответ формата - код_запроса;статус;значение
        break;
    }

    case SET_NEW_POLYNOMIAL_REQUEST:
    {
        QString canonicCoef = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Первый параметр (после полинома) - канон. коэф
        QString rootsText = strMsg.mid(strMsg.indexOf(separatorChar)); // Второй параметр (после полинома) - корни, строкой

        TComplex newCanonicCoef;
        canonicCoef >> newCanonicCoef;
        p.setCanonicCoef(newCanonicCoef);

        QStringList rootsList = rootsText.split(' '); // Разделяем строку корней на части по пробелу
        QString arr[2] = {};
        int tmp = 0;

        for (QString& rootText : rootsList)
        {
            if (!rootText.isEmpty())
            { // Проверяем, что часть не пустая
                arr[tmp++] = rootText;
            }

            if (tmp == 2)
            {
                QString concaetedNum;
                TComplex newRoot;

                concaetedNum = arr[0] + " " + arr[1];
                concaetedNum >> newRoot;

                p.addRoot(newRoot);

                tmp = 0;
            }

        }

        answer << QString().setNum(SET_NEW_POLYNOMIAL_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case SET_CANONIC_COEF_REQUEST:
    {
        TComplex newCanonicCoef;
        strMsg >> newCanonicCoef; // Единственный параметр (после полинома) - канон. коэф

        p.setCanonicCoef(newCanonicCoef);

        answer << QString().setNum(SET_CANONIC_COEF_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case ADD_ROOTS_REQUEST: {
        // Единственный параметр (после полинома) - строка новых корней
        QStringList rootsList = strMsg.split(' '); // Разделяем строку корней на части по пробелу
        QString arr[2] = {};
        int tmp = 0;

        for (QString& rootText : rootsList)
        {
            if (!rootText.isEmpty())
            { // Проверяем, что часть не пустая
                arr[tmp++] = rootText;
            }

            if (tmp == 2)
            {
                QString concaetedNum;
                TComplex newRoot;

                concaetedNum = arr[0] + " " + arr[1];
                concaetedNum >> newRoot;

                p.addRoot(newRoot);

                tmp = 0;
            }

        }

        answer << QString().setNum(ADD_ROOTS_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }

    default:
    {
        answer << QString().setNum(ERROR_UNKNOWN_REQUEST) << "ERR"; // Ответ формата - код_запроса;статус
        break;
    }
    }

    return answer;
}


QString TApplication::handleReal(QString &strMsg) {
    QString answer; // Тут будет результат обработки

    char separatorChar = separator.toLatin1(); // Преобразование QChar в char
    int pos = strMsg.indexOf(separatorChar);

    int requestType = strMsg.left(pos).toInt(); // Получаем тип запроса
    strMsg = strMsg.mid(pos + 1); // Убираем тип запроса из сообщения

    // Проверяем, начинается ли параметр "P(x) = "
    const QString prefix = "P(x) = ";
    if (!strMsg.startsWith(prefix)) {
        // Иначе ошибка, тк полином должен идти первым параметром
        qDebug() << "Polynom not found as first param in message.";
        return "";
    }

    QString param = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Получаем первый параметр - это должен быть полином

    TPolynom<TRealNumber> p(param); // Создание полинома
    p.setPrintMode(EPrintMode::EPrintModeClassic); // Ставим EPrintModeClassic мод для корректной работы

    strMsg.remove(0, strMsg.indexOf(separatorChar) + 1); // Убираем полином из сообщения

    switch (requestType)
    {
    case CANONICAL_FORM_REQUEST:
    {
        p.setPrintMode(EPrintMode::EPrintModeCanonical);
        answer << QString().setNum(CANONICAL_FORM_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case CLASSICAL_FORM_REQUEST:
    {
        answer << QString().setNum(CLASSICAL_FORM_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case CHANGE_ROOTS_COUNT_REQUEST:
    {
        // Извлекаем индекс корня (первый параметр)
        QString StrNewRootsCount = strMsg.mid(0, strMsg.indexOf(separatorChar));
        // Удаляем первую часть строки до первого разделителя
        strMsg.remove(0, strMsg.indexOf(separatorChar) + 1);

        // Извлекаем второй параметр (сам корень)
        QString strNewRoots = strMsg.mid(0, strMsg.indexOf(separatorChar));

        // Если второй параметр - это последнее значение в строке, и нет второго разделителя
        if (strNewRoots.isEmpty()) {
            strNewRoots = strMsg; // Берем всю оставшуюся строку
        }

        int newRootsCount = StrNewRootsCount.toInt();
        int addedCount = newRootsCount - p.getRootsCount();

        if (newRootsCount <= 0)
        {
            answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "ERR1" << p; // Ответ формата - код_запроса;статус
            break;
        }

        // если размер массива уменьшился
        if (addedCount < 0)
        {
            p.changeArrRootSize(newRootsCount);
            answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус
            break;
        }

        // если размер массива не изменился
        if (addedCount == 0)
        {
            answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "ERR2" << p; // Ответ формата - код_запроса;статус
            break;
        }

        // логика обработки корней
        QStringList rootsList = strNewRoots.split(' '); // Разделяем строку на части по пробелу
        if (rootsList.size() != addedCount * 2)
        {
            answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "ERR3" << p; // Ответ формата - код_запроса;статус
            break;
        }

        QString arr[2];
        int iter = 0;
        for (QString& item : rootsList)
        {
            if (!item.isEmpty())
            { // Проверяем, что часть не пустая
                arr[iter++] = item;
            }
            if (iter == 2)
            {
                QString concaetedNum;
                TRealNumber tmpNum;
                concaetedNum = arr[0] + " " + arr[1];
                concaetedNum >> tmpNum;

                p.changeArrRootSize(p.getRootsCount() + 1);
                p.changeRootByIndex(p.getRootsCount() - 1, tmpNum);
                p.calcCoefFromRoots();

                iter = 0;
            }
        }
        answer << QString().setNum(CHANGE_ROOTS_COUNT_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case CHANGE_ROOT_REQUEST:
    {
        QString index = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Первый параметр (после полинома) - индекс
        QString strNewRoot = strMsg.mid(strMsg.indexOf(separatorChar)); // Второй параметр (после полинома) - сам корень
        TRealNumber newRoot;
        strNewRoot >> newRoot;

        bool isChanged = p.changeRootByIndex(index.toInt(), newRoot);

        if (!isChanged) { // Обработка ошибки - если корень не изменился
            answer << QString().setNum(CHANGE_ROOT_ANSWER) << "ERR"; // Ответ формата - код_запроса;статус
            break;
        }

        answer << QString().setNum(CHANGE_ROOT_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }

    case CALCULATE_VALUE_AT_X_REQUEST:
    {
        QString strX = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Первый параметр (после полинома) - точка x
        TRealNumber x;
        strX >> x;
        answer << QString().setNum(CALCULATE_VALUE_AT_X_ANSWER) << "OK" << p.value(x) << QString(';') << x; // Ответ формата - код_запроса;статус;значение
        break;
    }

    case SET_NEW_POLYNOMIAL_REQUEST:
    {
        QString canonicCoef = strMsg.mid(0, strMsg.indexOf(separatorChar)); // Первый параметр (после полинома) - канон. коэф
        QString rootsText = strMsg.mid(strMsg.indexOf(separatorChar)); // Второй параметр (после полинома) - корни, строкой

        TRealNumber newCanonicCoef;
        canonicCoef >> newCanonicCoef;
        p.setCanonicCoef(newCanonicCoef);

        QStringList rootsList = rootsText.split(' '); // Разделяем строку корней на части по пробелу
        QString arr[2] = {};
        int tmp = 0;

        for (QString& rootText : rootsList)
        {
            if (!rootText.isEmpty())
            { // Проверяем, что часть не пустая
                arr[tmp++] = rootText;
            }

            if (tmp == 2)
            {
                QString concaetedNum;
                TRealNumber newRoot;

                concaetedNum = arr[0] + " " + arr[1];
                concaetedNum >> newRoot;

                p.addRoot(newRoot);

                tmp = 0;
            }

        }

        answer << QString().setNum(SET_NEW_POLYNOMIAL_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case SET_CANONIC_COEF_REQUEST:
    {
        TRealNumber newCanonicCoef;
        strMsg >> newCanonicCoef; // Единственный параметр (после полинома) - канон. коэф

        p.setCanonicCoef(newCanonicCoef);

        answer << QString().setNum(SET_CANONIC_COEF_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }
    case ADD_ROOTS_REQUEST: {
        // Единственный параметр (после полинома) - строка новых корней
        QStringList rootsList = strMsg.split(' '); // Разделяем строку корней на части по пробелу
        QString arr[2] = {};
        int tmp = 0;

        for (QString& rootText : rootsList)
        {
            if (!rootText.isEmpty())
            { // Проверяем, что часть не пустая
                arr[tmp++] = rootText;
            }

            if (tmp == 2)
            {
                QString concaetedNum;
                TRealNumber newRoot;

                concaetedNum = arr[0] + " " + arr[1];
                concaetedNum >> newRoot;

                p.addRoot(newRoot);

                tmp = 0;
            }

        }

        answer << QString().setNum(ADD_ROOTS_ANSWER) << "OK" << p; // Ответ формата - код_запроса;статус;полином
        break;
    }

    default:
    {
        answer << QString().setNum(ERROR_UNKNOWN_REQUEST) << "ERR"; // Ответ формата - код_запроса;статус
        break;
    }
    }

    return answer;
}
