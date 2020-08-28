#ifndef DSTARTPROGRESSDIALOG_H
#define DSTARTPROGRESSDIALOG_H

#include <QObject>
#include <QProgressDialog>

class DStartProgressDialog: public QObject
{
    Q_OBJECT

public:
    /**
     * @brief CImportDataKinopoisk- конструктор класса
     * @param parent - указатель на родителя
     */
    DStartProgressDialog(QObject *parent = nullptr);
    /**
     * @brief ~CImportDataKinopoisk - деструктор класса
     */
    ~DStartProgressDialog();

signals:
    /**
     * @brief resultReady - функция для возврата значения в вызывающий поток
     * @param result - возращаемое значение
     */
//    void resultReadyStartProgressDialog(const QString &result);

public slots:
    /**
     * @brief doWork - функция выполнения потока
     */
    void doWork(const QString &);

};
#endif // DSTARTPROGRESSDIALOG_H
