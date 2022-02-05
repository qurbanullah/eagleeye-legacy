#ifndef EEUPDATER_H
#define EEUPDATER_H

#include <QMainWindow>
#include <QTableWidget>
#include <QProcess>
#include <QProgressDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class EEUpdater; }
QT_END_NAMESPACE

class EEUpdater : public QMainWindow
{
    Q_OBJECT

public:
    EEUpdater(QWidget *parent = nullptr);
    ~EEUpdater();

private slots:
    QStringList traverseXml();
    void showPackageTable();
    void updatePackages();
    void readTerminalData();
    void readAllStandardError();
    void readyReadStandardOutput();
    void updateProgressBar(const int value);
    void delay(int millisecondsToWait);
    void handleTimeOut();
    void finalProcessFinished();
    void close();

private:
    Ui::EEUpdater *ui;
    QTableWidget *packagesTable;
    QProcess *updateProcess;
    QProgressDialog *pd;
    QTimer *t;
};
#endif // EEUPDATER_H
