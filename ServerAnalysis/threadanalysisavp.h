#ifndef THREADANALYSISAVP_H
#define THREADANALYSISAVP_H

#include <deque>

#include <QObject>
#include <QSqlQuery>

class ThreadAnalysisAVP: public QObject
{
    Q_OBJECT

public:
    ThreadAnalysisAVP();
    /**
     * @brief doWork - функция выполнения потока
     */
    void doWork();

signals:
    /**
     * @brief resultReady - функция для возврата значения в вызывающий поток
     * @param result - возращаемое значение
     */
    void resultReady(const QString &result);

private:
    void initListAVP();
    void analysisAVP(uint64_t idAVP);
    uint64_t idViolation(QString nameViolation);
    void addAnalysisResult(uint64_t idAVP, uint64_t idViolation, QString percent);

    uint64_t idAVP;
    QSqlQuery *query;
    QSqlQuery *query1;
    std::deque<uint64_t> m_listNonAnalysisAVP;
};

#endif // THREADANALYSISAVP_H
