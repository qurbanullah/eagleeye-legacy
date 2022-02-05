#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "include/eagleeye.h"

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(EagleEye);

    QApplication app(argc, argv);

#ifndef QT_NO_TRANSLATION
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);
#endif
    // chage the text color to white
    QPalette p = QApplication::palette();
    p.setColor( QPalette::Text, Qt::white );
    p.setColor( QPalette::WindowText, Qt::white );
    p.setColor( QPalette::ButtonText, Qt::white );
    p.setColor( QPalette::BrightText, Qt::white );
    QApplication::setPalette( p );

    // chage the background color
    //wizard.setStyleSheet("background-color: #0b2732;");
    EagleEye wizard;
    // add stylesheet to the application
    QFile file(":/qss/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    wizard.setStyleSheet(styleSheet);

    wizard.show();
    //wizard.setWindowState(Qt::WindowMaximized);
    return app.exec();
}
