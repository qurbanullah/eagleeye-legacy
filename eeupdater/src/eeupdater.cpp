#include "include/eeupdater.h"
#include "ui/ui_eeupdater.h"

#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QTime>

EEUpdater::EEUpdater(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EEUpdater)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    showPackageTable();
    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(updatePackages()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));


}

EEUpdater::~EEUpdater()
{
    delete ui;
}


// recursive DOM function
// traverse all the nodes and output what we need
QStringList EEUpdater::traverseXml()
{
    QFile file("/var/avouch/database/PackagesUpdateDatabase.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Cannot read file" << file.errorString();
        exit(0);
    }
    QXmlStreamReader reader(&file);
    QStringList packagesInfo;
    QString name;
    QString version;
    QString release;
    QString distribution;
    QString architecture;
    QString description;
    if (reader.readNextStartElement()) {
            if (reader.name() == "Packages"){
                while(reader.readNextStartElement()){
                    if(reader.name() == "Package"){
                        while(reader.readNextStartElement()){
                            if(reader.isStartElement()){
                                while(reader.readNextStartElement()){
                                    if(reader.name() == "Name"){
                                        name = reader.readElementText();
                                    }
                                    else if(reader.name() == "Version"){
                                        version = reader.readElementText();
                                    }
                                    else if(reader.name() == "Release"){
                                        release = reader.readElementText();
                                    }
                                    else if(reader.name() == "Distribution"){
                                        distribution = reader.readElementText();
                                    }
                                    else if(reader.name() == "Architecture"){
                                        architecture = reader.readElementText();
                                    }
                                    else if(reader.name() == "Description"){
                                        description = reader.readElementText();
                                    }
                                    else
                                        reader.skipCurrentElement();
                                }
                            }
                         }
                        // Add package info in a string list
                        packagesInfo += name + ";" +
                                        version + ";" +
                                        release + ";" +
                                        distribution + ";" +
                                        architecture + ";" +
                                        description;
                    }
                    else
                        reader.skipCurrentElement();
                }
            }
            else
                reader.raiseError(QObject::tr("Incorrect file"));
    }
    //qDebug() << packagesInfo;
    return packagesInfo;
}

void EEUpdater::showPackageTable()
{
    //qDebug() << "packag info is : " << traverseXml();

    QStringList packages = traverseXml();
    qDebug() << "Total packages are : " << packages.size();

    if(ui->tableWidget->rowCount() < packages.size())
    {
        ui->tableWidget->setRowCount(packages.size());
    }
    ui->tableWidget->setColumnCount(6);

//    ui->tableWidget->setColumnWidth(0, 300);
//    ui->tableWidget->setColumnWidth(1, 190);
//    ui->tableWidget->setColumnWidth(2, 190);
//    ui->tableWidget->setColumnWidth(3, 190);

    ui->tableWidget->setHorizontalHeaderLabels(QString("Package; Version; Release; Distribution; Architecture; Description").split(";"));

    int row = 0;

    for(int i = 0; i < packages.size(); i++)
    {
        QString package = packages[i];
        //qDebug() << "package is : " << package;
        // Split package info, so that each attribute is added in separate column
        QStringList currentPackage = package.split(";");
        // qDebug() << "Current package is : " << currentPackage;
        // qDebug() << "Current package size is : " << currentPackage.size();
        for(int cols = 0; cols < currentPackage.size(); cols++)
        {
            //qDebug() << "Current package is : " << package;
            QTableWidgetItem *newItem = new QTableWidgetItem();
            newItem->setTextAlignment(Qt::AlignLeft);
            newItem->setText(currentPackage.at(cols));

            ui->tableWidget->setItem(row, cols, newItem);
            ui->tableWidget->resizeColumnToContents(cols);
        }
    row++;
    }
}

void EEUpdater::updatePackages()
{
    //get package name
    QStringList packages = traverseXml();
    QStringList pkg;
    //qDebug() << "packag info is : " << packages;
    QString program = "sudo";
    QStringList arguments;
    updateProcess = new QProcess();

    //connect(this, SIGNAL(updateProgressValue(int)), this, SLOT(updateProgressBar(packages.size())));

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(packages.size()-1);

    for(int i = 0; i < packages.size(); i++)
    {
        QString package = packages[i];
        // qDebug() << "Package is : " << package;
        QString currentPackage = package.split(";").at(0);
        ui->labelFinished->setText("Installing " + currentPackage);

        //qDebug() << "Current package is : " << currentPackage;
        // install package
        arguments <<  "/usr/bin/ee"  << "-up" << currentPackage;
        qDebug() << program << arguments;

        updateProcess->QProcess::start(program, arguments, QIODevice::ReadWrite);

        if (updateProcess->waitForStarted() && updateProcess->waitForFinished())
        {
            //qDebug() << updateProcess->readAll();
            QString standardOutput = updateProcess->readAllStandardOutput();
            QString standardError = updateProcess->readAllStandardError();

            if(!standardOutput.isEmpty())
            {
                qDebug() << standardOutput;
            }
            if(!standardError.isEmpty())
            {
                qDebug() << standardError;
            }

            ui->progressBar->setValue(i);
            //connect(this, SIGNAL(valueChanged()), this, SLOT(updateProgressBar(i)));
            //updateProcess->waitForFinished(-1);

            arguments.clear();
            updateProcess->close();

        }

    }
    //packages.clear();
    //connect(updateProcess, SIGNAL(QProcess::finished(0, NormalExit)), this, SLOT(finalProcessFinished()));
    ui->tableWidget->hide();


    //qDebug() << "Installng package : " << pkg;

    //connect(updateProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readTerminalData()));
    //




    //connect(updateProcess, SIGNAL(finished(int)), this, SLOT(finalProcessFinished()));
    //updateProcess->waitForFinished(-1);
}

void EEUpdater::readTerminalData()
{
    QByteArray data = updateProcess->readAllStandardOutput();
    qDebug() << "Terminal output:" << data;
}

void EEUpdater::readAllStandardError()
{
    QByteArray data = updateProcess->readAllStandardError();
    if(data.isEmpty())
    {
        qDebug() << "No terminal output.";
    }
    else
    {
        qDebug() << "Terminal output:" << data;
    }
}

void EEUpdater::readyReadStandardOutput()
{
    QString string = updateProcess->readAllStandardOutput();
    qDebug() << string;

}

void EEUpdater::updateProgressBar(const int value)
{
    ui->progressBar->setValue(value);
}

void EEUpdater::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void EEUpdater::handleTimeOut()
{
    int value;
     for(value=1; value < 100; value++){
        ui->progressBar->setValue(value);
        delay(6000);
      }
}

void EEUpdater::finalProcessFinished()
{
    qDebug() << "processStoped()";
    ui->labelFinished->setText("All packages updated successfully.");
}

void EEUpdater::close()
{
    QMainWindow::close();
}
