#include <QtWidgets>
#include <QtPrintSupport/QPrintDialog>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QRect>
#include <QDesktopWidget>

#include "include/eagleeye.h"

//constructor
EagleEye::EagleEye(QWidget *parent)
    : QWizard(parent)
{
    setPage(Page_Start, new StartPage);
    setPage(Page_Info, new InfoPage);
    setPage(Page_License, new LicensePage);
    setPage(Page_FinalSetup, new FinalSetupPage);
    setPage(Page_Conclusion, new ConclusionPage);

    setStartId(Page_Start);

#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif
    setOption(HaveHelpButton, true);

    connect(this, &QWizard::helpRequested, this, &EagleEye::showHelp);

    setWindowTitle(tr("Avouch Linux Package"));
    setMinimumSize(640, 480);
    setMaximumSize(640, 480);
}

//help function; display help on each page related to that page
void EagleEye::showHelp()
{
    static QString lastHelpMessage;

    QString message;

    switch (currentId()) {
    case Page_Start:
        message = tr("This software let you install <i>Avouch&trade; Linux</i> onto your system.");
        break;
    case Page_License:
        message = tr("This software is distributed under "
                     "<i>GNU General Public License Version 2</i>."
                     " without any waranty."
                     " Please do read the terms and condition of the license");
        break;
    case Page_FinalSetup:
        message = tr("The setup will take few minutes; depending on your system hardwares. "
                     "Please wait till the installation process completes.");
        break;
    case Page_Conclusion:
        message = tr("If you reach at this page it means that "
                     "<i>Package</i>"
                     "installed successfully");
        break;
    default:
        message = tr("This is littel help about different pages of the <i>EagleEye&trade; Avouch Linux Package Installer</i>.");
    }

    if (lastHelpMessage == message)
        message = tr("Sorry, I already gave what help I could. "
                     "Maybe you should try asking a human?");

    QMessageBox::information(this, tr("EagleEye&trade; Help"), message);

    lastHelpMessage = message;
}

StartPage::StartPage(QWidget *parent)
    : QWizardPage(parent)
{

    setTitle(tr("Avouch Linux Installer"));
    setSubTitle(tr("Install the Avouch Linux into the hard disk with simple steps"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/WatermarkPixmapWelcome.png"));

    //set font
     QFont font;
     font.setPointSize(20);
     font.setBold(true);

     package_file = ":/example.html";
    const QStringList args = QCoreApplication::arguments();
    if (qApp->arguments().count() == 2){
        package_file = args.at(1);
        qDebug() << "Package file is: " + package_file;
    }
    else{
       qDebug() << "Package file not specified ";
    }


    Top_Label = new QLabel(tr("This software let you install packages for "
                             "<i>Avouch&trade; Linux</i> "
                             "on to your computer."));
    Top_Label->setWordWrap(true);

    pictureLabel = new QLabel();
    //pictureLabel->setMaximumSize(1000,600);
    //pictureLabel->setPixmap(QPixmap(":/images/welcome.png"));

    lineEdit_tmpDirPath = new QLineEdit;
    registerField("tpmdirpath", lineEdit_tmpDirPath);
    lineEdit_tmpDirPath->hide();


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(Top_Label);
    mainLayout->addWidget(pictureLabel);
    mainLayout->addWidget(lineEdit_tmpDirPath);
    //mainLayout->addWidget(horizontalGroupBox1);
    //mainLayout->addWidget(horizontalGroupBox2);
    setLayout(mainLayout);
//    StartPage::getTempDir();
    StartPage::extractPackage();

    //connect(Install_Button, SIGNAL(clicked()), this, SLOT(showLicensePage()));
    //connect(Close_Button, SIGNAL(clicked()), this, SLOT(close()));
}

void StartPage::initializePage(){
    //StartPage::extractPackage();
}

QString StartPage::getPackage(){

    QFileInfo file(package_file); //archive.tar.avhp.zstd

    absolute_filename = file.absoluteFilePath().trimmed();  // base = "archive"
    //pkgname = fi.baseName();  // base = "archive"
    //qDebug() << "File is " + absolute_filename;
    return absolute_filename;
}

void StartPage::GetRandomString()
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 12; // assuming you want random strings of 12 characters
//   quint32 value = QRandomGenerator::global()->generate();


   for(int i=0; i<randomStringLength; ++i)
   {
       int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
//   qDebug()<<"Random string is : " + randomString;
//   return randomString;
}

void StartPage::getTempDir(){
    StartPage::GetRandomString();
    temp_dir_path = "/tmp/" + StartPage::randomString;

    lineEdit_tmpDirPath->setText(temp_dir_path);
//    QString temp_dir_path ="/tmp/ee";

//    qDebug()<<lineEdit_tmpDirPath->text();
//    QString path ="/tmp/ee";
    QDir temp_dir;
    if(temp_dir.exists(temp_dir_path)) {
        temp_dir.remove(temp_dir_path);
        //qDebug()<<"directory exists";
    }
    else {
        temp_dir.mkpath(temp_dir_path);
        //qDebug()<<"directory now exists";
    }
//     qDebug() << "Temp dir is " + temp_dir_path;
//    return temp_dir_path;
}

void StartPage::extractPackage(){
    StartPage::getTempDir();
    QString package = StartPage::getPackage();
//    QString temp_dir = StartPage::temp_dir_path;
    //QString temp_directory = StartPage::getTempdir();
    //QString program_extractpkg = "/usr/bin/tar -Jxf " + package + " -C " + temp_dir;
    //qDebug() << "Extraact package command is : " + program_extractpkg;
    QFile file(package);
    qDebug() << "Temp dir is " + temp_dir_path;

    QString program = "/usr/bin/bsdtar";
    const QStringList arguments = {"-xf", package, "-C", temp_dir_path};
    //QStringList arguments;
    //arguments << "-Jxf" << package << "-C" << temp_dir;

    //QProcess *myProcess = new QProcess();
    if (!file.exists()){
        qDebug() << "File does not exists";
        file.error();
    }
    else{
        //qDebug() << "Package exists";
        process_extractpkg = new QProcess();
        process_extractpkg->QProcess::start(program, arguments, QIODevice::ReadWrite);
        //process_extractpkg->start(program_extractpkg);
        process_extractpkg->waitForFinished(-1);
        //process_extractpkg->close();
        //qDebug() << "Reached here";
        //getPackageInfo();
    }
}

int StartPage::showLicensePage(){

    return EagleEye::Page_Info;
}

int StartPage::nextId() const
{
    return EagleEye::Page_Info;
}

InfoPage::InfoPage(QWidget *parent)
    : QWizardPage(parent) {

    setTitle(tr("Avouch Linux Installer"));
    setSubTitle(tr("Install the Avouch Linux package with simple steps"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/WatermarkPixmapInfo.png"));



    process_installpkg = new QProcess(this);
    process_extractpkg = new QProcess(this);
    process_install = new QProcess(this);
    process_postinstall = new QProcess(this);

    textedit_pkginfo = new QTextEdit();

//    StartPage *ptr = new StartPage();
//    temp_dir = ptr->temp_dir_path;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textedit_pkginfo);    
    setLayout(layout);

    InfoPage::getPackageInfo();
}

void InfoPage::initializePage(){
    tpm_dir_path = field("tpmdirpath").toString();
//    tpm_dir_path = field("tpmdirpath").toString();
    qDebug() << "Temp dir is " + tpm_dir_path;
    package_config_dir = tpm_dir_path + "/usr/share/avouch/pkginfo/";
    qDebug() << "Package config dir is : " + package_config_dir;

}

QString InfoPage::getPackageConfigFile(){
//    StartPage *ptr = new StartPage();
//    QString temp_dir = ptr->temp_dir_path;
    // QString temp_dir=StartPage::getTempDir();
//    tpm_dir_path = field("tpmdirpath").toString();

//    qDebug() << "Temp dir is " + tpm_dir_path;
//    QString package_config_dir = tpm_dir_path + "/usr/share/avouch/pkginfo/";
//    qDebug() << "Package config dir is : " + package_config_dir;

    QDir dir = package_config_dir;
    dir.setNameFilters(QStringList("*.xml"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList file_config = dir.entryList();
    //qDebug() << "Reached here too";
    // convert QStringList to QString
    // there is only one file pkgname.xml
    QString package_config_file = file_config.at(0);
    //qDebug() << "Reached here too too";
    //qDebug() << package_config_dir + package_config_file;
    return package_config_dir + package_config_file;
}

QString InfoPage::getPackageInfo(){

    //tanverse the directory recursively and find *.db
    // There should be only one .db file othervise it produces error

    //QString package = StartPage::getPackage();
    QString package_config_file = InfoPage::getPackageConfigFile();

    QFile xmlFile(package_config_file);
    qDebug() << "Package config file is: " + package_config_file;

    //QString ifile = tmpdir.path() + "/usr/share/avouch/pkginfo/" + dbfile;

    if (!xmlFile.exists()) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","The package is currupted or is not an Avouch Linux package! \n "
                                      "Please verify md5 hash value.");
        messageBox.setFixedSize(500,200);
        exit(1);
     }
     else {
        //xmlFile = new QFile(tmpdir.path() + "/usr/share/avouch/pkginfo/" + dbfile);
        if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this,"Load XML File Problem",
            "Couldn't open xmlfile.xml to load settings for download",
            QMessageBox::Ok);
            return nullptr;
                }
         QXmlStreamReader xmlReader(&xmlFile);

        //Parse the XML until we reach end of it
        while(!xmlReader.atEnd() && !xmlReader.hasError()) {
            if(xmlReader.readNext() == QXmlStreamReader::StartElement) {
                if (xmlReader.name() == "Name"){
                    pkgname = xmlReader.readElementText();
                    textedit_pkginfo->append("Package Name    = " + pkgname + "\n");
                    continue;
                }
                else if (xmlReader.name() == "Version"){
                    pkgver = xmlReader.readElementText();
                    textedit_pkginfo->append("Package Version = " + pkgver + "\n");
                    continue;
                }
                else if (xmlReader.name() == "Release"){
                    pkgrel = xmlReader.readElementText();
                    textedit_pkginfo->append("Package Release = " + pkgrel + "\n");
                    continue;
                }
                else if (xmlReader.name() == "Distribution"){
                    dist = xmlReader.readElementText();
                    textedit_pkginfo->append("Avouch Version   = " + dist + "\n");
                    continue;
                }
                else if (xmlReader.name() == "Architecture"){
                    carch = xmlReader.readElementText();
                    textedit_pkginfo->append("Architecture        = " + carch + "\n");
                    continue;
                }
                else if (xmlReader.name() == "Description"){
                    description = xmlReader.readElementText();
                    textedit_pkginfo->append("Description         = " + description + "\n");
                    continue;
                }
                else if (xmlReader.name() == "SourceUrl"){
                    srcurl = xmlReader.readElementText();
                    textedit_pkginfo->append("Source Url           = " + srcurl + "\n");
                    continue;
                }
                else if (xmlReader.name() == "PackageUrl"){
                    pkgurl = xmlReader.readElementText();
                    textedit_pkginfo->append("Package Url         = " + pkgurl + "\n");
                    continue;
                }
                else if (xmlReader.name() == "DateCreated"){
                    date = xmlReader.readElementText();
                    textedit_pkginfo->append("Package Date      = " + date + "\n");
                    continue;
                }
                else if (xmlReader.name() == "Dependancies"){                                     
                    depends = xmlReader.readElementText();
                    textedit_pkginfo->append("Depends              = " + depends + "\n");
                    continue;
                }
            }
        }

        if(xmlReader.hasError()) {
            QMessageBox::critical(this,
            "xmlFile.xml Parse Error",xmlReader.errorString(),
            QMessageBox::Ok);
                //return;
        }
        //close reader and flush file
        xmlReader.clear();
        xmlFile.close();
    }
    // By default the cursor will be at the end of the file, so the file displayed will be at the end
    // By the following code we take the cursor at the start of the file.
    QTextCursor textCursor = textedit_pkginfo->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    textedit_pkginfo->setTextCursor(textCursor); // The line to add
    pkgname = pkgname.trimmed();
    //qDebug() << "Reached here";
    return pkgname;
}

int InfoPage::nextId() const {
    return EagleEye::Page_License;
}

LicensePage::LicensePage(QWidget *parent)
    : QWizardPage(parent) {
    setTitle(tr("End User License Agreement (EULA)"));
    setSubTitle(tr("Read the Endu Usel License Agreement"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/WatermarkPixmapEULA.png"));

    textEdit = new QTextEdit();
    licenseAgreementCheckBox = new QCheckBox(tr("&I have read the license terms and condition and agreed to continue"));
    registerField("evaluate.checkbox*", licenseAgreementCheckBox);

    QVBoxLayout *layout = new QVBoxLayout;
    //layout->addWidget(topLabel);
    layout->addWidget(textEdit);
    layout->addWidget(licenseAgreementCheckBox);
    setLayout(layout);
}

QString LicensePage::getPackageConfigFile(){
//    QString package_config_dir ="/tmp/ee/usr/share/avouch/pkginfo/";
    QDir dir = package_config_dir;
    dir.setNameFilters(QStringList("*.xml"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList file_config = dir.entryList();
    // convert QStringList to QString
    // there is only one file pkgname.xml
    QString package_config_file = file_config.at(0);
    //qDebug() << package_config_dir + package_config_file;

    QFile xmlFile(package_config_dir + package_config_file);
    if (!xmlFile.exists()) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","The package is currupted or is not an Avouch Linux package! \n "
                                      "Please verify md5 hash value.");
        messageBox.setFixedSize(500,200);
        exit(1);
     }
     else {
        //xmlFile = new QFile(tmpdir.path() + "/usr/share/avouch/pkginfo/" + dbfile);
        if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this,"Load XML File Problem",
            "Couldn't open xmlfile.xml to load settings for download",
            QMessageBox::Ok);
            //return 0;
                }
    QXmlStreamReader xmlReader(&xmlFile);
    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        if(xmlReader.readNext() == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == "Name"){
                pkgname = xmlReader.readElementText();
                 pkgname =  pkgname.trimmed();
                continue;
            }
           }
       }
    }
    return pkgname;
}

void LicensePage::initializePage() {

    tpm_dir_path = field("tpmdirpath").toString();
//    tpm_dir_path = field("tpmdirpath").toString();
    qDebug() << "Temp dir is " + tpm_dir_path;
    package_config_dir = tpm_dir_path + "/usr/share/avouch/pkginfo/";
    qDebug() << "Package config dir is : " + package_config_dir;

    pkgname = LicensePage::getPackageConfigFile();
    //QFile file("/usr/share/licenses/gnu/gpl-2.0.txt");

    QFile file(tpm_dir_path + "/usr/share/licenses/" + pkgname + "/LICENSE");
    qDebug() << "License File is :" + tpm_dir_path + "/tmp/ee/usr/share/licenses/" + pkgname + "/LICENSE";

    QString line;
    //textEdit->setMinimumHeight(600);
    //textEdit->setMaximumHeight(600);

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream( &file ); // Set the stream to read from myFile

        while(!stream.atEnd()){

            line = stream.readLine(); // this reads a line (QString) from the file
            //qDebug() << line;
            textEdit->append(line);

            // By default the cursor will be at the end of the file, so the file displayed will be at the end
            // By the following code we take the cursor at the start of the file.
            QTextCursor textCursor = textEdit->textCursor();
            textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
            textEdit->setTextCursor(textCursor); // The line to add
      }
  }

    file.close();
}




int LicensePage::nextId() const
{
    if (licenseAgreementCheckBox->isChecked()) {
        return EagleEye::Page_FinalSetup;
    } else {
        return EagleEye::Page_License;
    }
}

/******************* End of of License Page *****************************************/

FinalSetupPage::FinalSetupPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Final Setup Page"));
    setSubTitle(tr("Packae installation is in progress. This will take few minutes "
                   "depending upon system hardwares"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/WatermarkPixmapCopyingFiles.png"));

    //StartPage* s = new StartPage;
    //pkgname = s->getPackageInfo();
    //tempdir = s->tempDir;
    //qDebug() << pkgname;
    //qDebug() << "FinalSetupPage Temp dir is " + tempdir;

    topLabel = new QLabel(tr("Installation Progress:"));
    progressBar = new QProgressBar(this);
    //progressBar->setMinimum(0);
    //progressBar->setMaximum(0);
    //progressBar->setValue(0);
    //progressBar->setVisible(true);

    checkbox_complete = new QCheckBox(tr(""));
    textedit_show_process = new QTextEdit;
    label_process_finished = new QLabel();
    lineEdit_procfinished = new QLineEdit;

    //label_procfinished->setBuddy(lineEdit_procfinished);
    registerField("evaluate.procfinished*", label_process_finished);

    timer = new QTimer(this);
    count = 0;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(progressBar);
    layout->addWidget(textedit_show_process);
    layout->addWidget(label_process_finished);
    setLayout(layout);
}

void FinalSetupPage::updateProgressBar(const int value){

    progressBar->setValue(value);

}// Update ProgressBar

void FinalSetupPage::initializePage(){

    tpm_dir_path = field("tpmdirpath").toString();
//    tpm_dir_path = field("tpmdirpath").toString();
    qDebug() << "Temp dir is " + tpm_dir_path;
    package_config_dir = tpm_dir_path + "/usr/share/avouch/pkginfo/";
    qDebug() << "Package config dir is : " + package_config_dir;

    wizard()->button(QWizard::BackButton)->setDisabled(true);
    pkgname = FinalSetupPage::getPackageConfigFile();
    QString program = "sudo";
    QStringList arguments;
    // arguments << "/usr/bin/eescript" << pkgname << "/tmp/ee" << "install";
    arguments << "/usr/bin/ee" << "-ifg" << tpm_dir_path;
    qDebug() << arguments;
    //arguments << "/usr/bin/cp" << "-Rvf" << tempdir + "/*" << "/";

    //QString program = "pkexec /usr/bin/eescript " + pkgname + " " + tempdir + "install";
    //QStringList arguments;
    //arguments << "/usr/bin/eescript" << pkgname << tempdir << "install";
    progressBar->reset();

    process_final_setup = new QProcess();
    process_final_setup->setProcessChannelMode(QProcess::MergedChannels);
    process_final_setup->start(program, arguments);
    
    connect(process_final_setup, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(process_final_setup, SIGNAL(finished(int)), this, SLOT(finalProcessFinished()));
    connect(this, SIGNAL(updateProgressValue(int)), this, SLOT(updateProgressBar(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateValue()));

    if (process_final_setup->state() == QProcess::NotRunning){
        qDebug() << "Process not started";
    }
    else if (process_final_setup->state() == QProcess::Starting){
        qDebug() << "Process is starting";
        //delay(9000);
        timer->start(100);
    }
    else if (process_final_setup->state() == QProcess::Running){
        qDebug() << "Process is running";
        timer->start(100);
    }

    //qDebug() << "Arguments are: " << program << arguments;

    //connect(process_final_setup, SIGNAL(started()), this, SLOT(processStarted()));
    //connect(process_final_setup, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    //connect(process_final_setup, SIGNAL(finished(int)), this, SLOT(finalProcessFinished()));

    //process_final_setup->waitForFinished(-1);
    //process_final_setup->close();
    //FinalSetupPage::finalProcessFinished();
}

QString FinalSetupPage::getPackageConfigFile(){
//    QString package_config_dir ="/tmp/ee/usr/share/avouch/pkginfo/";
    QDir dir = package_config_dir;
    dir.setNameFilters(QStringList("*.xml"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList file_config = dir.entryList();
    // convert QStringList to QString
    // there is only one file pkgname.xml
    QString package_config_file = file_config.at(0);
    //qDebug() << package_config_dir + package_config_file;

    QFile xmlFile(package_config_dir + package_config_file);
    if (!xmlFile.exists()) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","The package is currupted or is not an Avouch Linux package! \n "
                                      "Please verify md5 hash value.");
        messageBox.setFixedSize(500,200);
        exit(1);
     }
     else {
        //xmlFile = new QFile(tmpdir.path() + "/usr/share/avouch/pkginfo/" + dbfile);
        if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this,"Load XML File Problem",
            "Couldn't open xmlfile.xml to load settings for download",
            QMessageBox::Ok);
            //return 0;
                }
    QXmlStreamReader xmlReader(&xmlFile);
    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        if(xmlReader.readNext() == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == "Name"){
                pkgname = xmlReader.readElementText();
                 pkgname =  pkgname.trimmed();
                continue;
            }
           }
       }
    }
    //qDebug() << "Package name is : " << pkgname;
    return pkgname;
}

void FinalSetupPage::readyReadStandardOutput(){

    output.append(process_final_setup->readAllStandardOutput());
    textedit_show_process->setText(output);

    textedit_show_process->verticalScrollBar()
        ->setSliderPosition(
            textedit_show_process->verticalScrollBar()->maximum());
}

void FinalSetupPage::finalProcessFinished()
{
    qDebug() << "processStoped()";
    // Set the encoding status by checking output file's existence
    QString fileName = "/usr/share/avouch/pkginfo/" + pkgname + ".xml";
    qDebug() << fileName;

    if (QFile::exists(fileName)) {
        label_process_finished->setText("Successful");
        label_process_finished->setStyleSheet("color: green;");
        FinalSetupPage::SlotDetectFinish();
    }
    else {
        label_process_finished->setText("Failed");
        label_process_finished->setStyleSheet("color: red;");
    }
}

void FinalSetupPage::SlotDetectFinish(){
    //qDebug() << "Exit Code is: " << exitCode;
    //qDebug() << "Exit Status is: " << exitSatus;
    topLabel->setText("Installation completed");
    topLabel->setStyleSheet("color: green;");
    count = 100;
    progressBar->setValue(100);
    //progressBar->close();
    wizard()->button(QWizard::NextButton)->setEnabled(true);
    //wizard()->button(QWizard::BackButton)->setDisabled(true);
    //FinalSetupPage::isComplete();
}

void FinalSetupPage::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void FinalSetupPage::handleTimeOut()
{
     for(value=1; value < 100; value++){
        progressBar->setValue(value);
        delay(200);
      }
}

void FinalSetupPage::updateValue() {

    if(count == 100) {
        timer->stop();
        return;
    }// if
    count++;
    emit updateProgressValue(count);

}// updateValue

void FinalSetupPage::showEvent(QShowEvent *) {
    wizard()->button(QWizard::BackButton)->setDisabled(true);
}

int FinalSetupPage::nextId() const {

    return EagleEye::Page_Conclusion;
}

/******************* End of of Final Setup Page *****************************************/

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent) {

    setTitle(tr("Installation completed successfully"));
    setSubTitle(tr("Press Finish button to exit this wizard."));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/WatermarkPixmapCompleted.png"));

    bottomLabel = new QLabel("Installation completed");
    bottomLabel->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(bottomLabel);
    setLayout(layout);
}

void ConclusionPage::initializePage() {

    QString licenseText;

    if (wizard()->hasVisitedPage(EagleEye::Page_FinalSetup)) {
        licenseText = tr("Package installed successfully. Now you can finish this wizard.");
    } else {
        licenseText = tr("<u>Avouch&trade; Linux:</u> "
                         "installation is not seccessfull.");
    }

    bottomLabel->setText(licenseText);
}

int ConclusionPage::nextId() const {
    return -1;
}
