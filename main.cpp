#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    QFile styles("C:/Users/zuzaw/Projekty/flyNote/style/flyNote.css");
    styles.open(QIODevice::ReadOnly);
    app.setStyleSheet(styles.readAll());
    styles.close();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "flyNote_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();
    return app.exec();
}
