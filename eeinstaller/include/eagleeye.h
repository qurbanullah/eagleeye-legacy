#ifndef EAGLEEYE_H
#define EAGLEEYE_H

#include <QWizard>
#include <QProcess>
#include <QMessageBox>
#include <QTime>
#include <QTableWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QtSql>

class QCheckBox;
class QLabel;
class QLineEdit;
class QRadioButton;
class QCheckBox;
class QTextEdit;
class QGroupBox;
class QTime;
class QProgressBar;
class QXmlStreamReader;


class EagleEye : public QWizard
{
    Q_OBJECT

public:
    enum { Page_Start, Page_Info, Page_License, Page_FinalSetup, Page_Conclusion };

    EagleEye(QWidget *parent = nullptr);

private slots:
    void showHelp();
};

class StartPage : public QWizardPage
{
    Q_OBJECT

public:
    StartPage(QWidget *parent = nullptr);
    void initializePage() Q_DECL_OVERRIDE;
    int nextId() const Q_DECL_OVERRIDE;
    void GetRandomString();
    void getTempDir();

    QString getPackage();
    QString package_file;
    QString tempDir;
    QString temp_dir_path;
    QString randomString;

private slots:
    //void createHorizontalGroupBox();
    int showLicensePage();
    void extractPackage();

private:
    QLabel *Top_Label;
    QLabel *pictureLabel;
    QLabel *Install_Label;
    QLabel *Close_Label;
    QLineEdit *lineEdit_tmpDirPath;
    QString tpmdirpath;
    QPushButton *Install_Button;
    QPushButton *Close_Button;
    QGroupBox *horizontalGroupBox1;
    QGroupBox *horizontalGroupBox2;
    QString absolute_filename;
    QProcess *process_extractpkg;


};

class InfoPage : public QWizardPage
{
    Q_OBJECT

public:
    InfoPage(QWidget *parent = nullptr);
    QString getPackageInfo();
    QString getPackageConfigFile();

    void initializePage() Q_DECL_OVERRIDE;
    int nextId() const Q_DECL_OVERRIDE;
    QString package_file;
    QString tempDir;
    QString tpm_dir_path;
    QString package_config_dir;


private slots:
    //void createHorizontalGroupBox();
    //int showLicensePage();



    //bool load(const QString &f);
    //void setCurrentFilename(const QString &filename);
    //void fileNew();
    //void show_installing_window();
    //QStringList findSqliteDbFile(const QStringList &dbfiles);



private:
    QLabel *Top_Label;
    QLabel *Install_Label;
    QLabel *Close_Label;
    QLineEdit *lineEdit_packagename;
    QPushButton *Install_Button;
    QPushButton *Close_Button;
    QGroupBox *horizontalGroupBox1;
    QGroupBox *horizontalGroupBox2;
    QFileDialog *fd;   
    QString packagename;
    QString temp_dir;
    QString pkgname;
    QString pkgver;
    QString pkgrel;
    QString dist;
    QString carch;
    QString description;
    QString srcurl;
    QString pkgurl;
    QString date;
    QString depends;
    QString absolute_filename;
    QString filename;
    QString *tmp_dir_path;
    QProcess *process_installpkg;
    QProcess *process_extractpkg;
    QProcess *process_install;
    QProcess *process_postinstall;
    QProcess *sqlitefile;
    QTab *tab_pkginfo;
    QTab *tab_maintaner;
    QTab *tab_license;
    QTextEdit *textedit_pkginfo;
    QTextEdit *textedit_maintainer;
    QTextEdit *textedit_contributer;
    QTextEdit *textedit_license;
    QFile xmlFile;
    QXmlStreamReader xmlReader;
    QString temp_directory;
    QStringList file_config;

    QString tpmdirpath;
};

/******************* End of of LicensePage *****************************************/

class LicensePage : public QWizardPage
{
    Q_OBJECT

public:
    LicensePage(QWidget *parent = nullptr);
    QString getPackageConfigFile();
    //QString getLicense(const QString);
    void initializePage() Q_DECL_OVERRIDE;
    int nextId() const Q_DECL_OVERRIDE;
    QString pkgname;
    QString tpm_dir_path;
    QString package_config_dir;

private:
    QLabel *topLabel;
    QRadioButton *registerRadioButton;
    QRadioButton *evaluateRadioButton;
    QCheckBox *licenseAgreementCheckBox;
    QTextEdit *textEdit;

    QString tempdir;
    QFile file;
    QString packagename;

};

/******************* End of of LicensePage *****************************************/

class FinalSetupPage : public QWizardPage
{
    Q_OBJECT

public:
    FinalSetupPage(QWidget *parent = nullptr);
    QString getPackageConfigFile();
    int nextId() const Q_DECL_OVERRIDE;
    void initializePage() Q_DECL_OVERRIDE;
    //bool isComplete() const;
    void delay( int millisecondsToWait );
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    //virtual bool validatePage(){return false;}
    QString tpm_dir_path;
    QString package_config_dir;

signals:
    void updateProgressValue(const int value);

public slots:
    void finalProcessFinished();
    void SlotDetectFinish();
    void handleTimeOut();
    void readyReadStandardOutput();
    void updateProgressBar(const int value);

private:

    QLabel *topLabel;
    QLabel *pictureLabel;
    QLabel *label_process_finished;
    QTextEdit *textedit_show_process;
    QLineEdit *lineEdit_procfinished;
    QProgressBar *progressBar;

    QString pkgname;
    QString tempdir;
    QString output;

    QProcess *process_final_setup;
    QCheckBox *checkbox_complete;
    QTimer *timer;
    int value;    
    int count;

private slots:
    void updateValue();

};

/******************* End of of FinalSetp Page *****************************************/

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = nullptr);

    void initializePage() Q_DECL_OVERRIDE;    
    //void setVisible(bool visible) Q_DECL_OVERRIDE;
    int nextId() const Q_DECL_OVERRIDE;

private slots:
    //void rebootButtonClicked();

private:
    QLabel *bottomLabel;
    QCheckBox *agreeCheckBox;
    QProcess *process;

    QString pkgname;
    QString tempdir;
    QString output;
};

/******************* End of of ConclusionPage Page *****************************************/

#endif // EAGLEEYE_H

