#ifndef DIMPORTDATAOUTEXCEL_H
#define DIMPORTDATAOUTEXCEL_H

#include <QObject>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class D_ImportDataOutExel; }
QT_END_NAMESPACE

/**
 * @brief The DImportDataOutExcel class
 */
class DImportDataOutExcel: public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DImportDataOutExcel
     * @param parent
     */
    DImportDataOutExcel(QWidget *parent = nullptr);
    /**
     * @brief ~DImportDataOutExcel
     */
    ~DImportDataOutExcel();
    /**
     * @brief setText
     * @param text
     */
    void setText(QString text);
    /**
     * @brief setProgress
     * @param progress
     */
    void setProgress(int progress);
    /**
     * @brief clearTextInWindow
     */
    void clearTextInWindow();

private slots:
    /**
     * @brief slotExit
     */
    void slotExit();

private:
    Ui::D_ImportDataOutExel *ui;
};

#endif // DIMPORTDATAOUTEXCEL_H
