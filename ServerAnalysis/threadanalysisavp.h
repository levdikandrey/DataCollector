#ifndef THREADANALYSISAVP_H
#define THREADANALYSISAVP_H

#include <deque>

#include <QObject>
#include <QSqlQuery>
/**
 * @brief The ThreadAnalysisAVP class
 */
class ThreadAnalysisAVP: public QObject
{
    Q_OBJECT

public:
    /**
     * @brief ThreadAnalysisAVP
     */
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
    void resultReady(const QString &);
    /**
     * @brief sendAnswerAnalysisAVP
     */
    void sendAnswerAnalysisAVP(uint64_t &);

private:
    /**
     * @brief initListAVP
     */
    void initListAVP();
    /**
     * @brief analysisAVP
     * @param idAVP
     */
    void analysisAVP(uint64_t idAVP);
    /**
     * @brief idViolation
     * @param nameViolation
     * @return
     */
    uint64_t idViolation(QString nameViolation);
    /**
     * @brief addAnalysisResult
     * @param idAVP
     * @param idViolation
     * @param percent
     */
    void addAnalysisResult(uint64_t idAVP, uint64_t idViolation, QString percent);

    uint64_t idAVP;
    QSqlQuery *query;
    QSqlQuery *query1;
    std::deque<uint64_t> m_listNonAnalysisAVP;
};

#endif // THREADANALYSISAVP_H
