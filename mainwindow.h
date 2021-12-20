#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QVector>
#include <QWidget>
#include <QShortcut>
#include <QObject>
#include <QJsonObject>
#include "flynote.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    const QShortcut *saving_shortcut = new QShortcut(QKeySequence::Save, this, SLOT(saveOpened()));
    const QShortcut *increase_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal), this, SLOT(increaseFont()));
    const QShortcut *decrease_shortcut = new QShortcut(QKeySequence::ZoomOut, this, SLOT(decreaseFont()));
    QVector<flyNote*> children_window;
    QVector<QListWidgetItem*> selected_files;

    QJsonObject names_files;
    QJsonObject config;

    bool resizing = false;
    int start_x;
    int start_y;
    QPoint start_pos;
    QString style = "blue";
    int font_size = 15;

    Ui::MainWindow *ui;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *e);
    //const QString* get_key(QString name);

private slots:
    void files();
    void openSelected();
    void deleteSelected();
    void createNew(QString *path=nullptr, QString name = "flyNote –");
    void saveOpened();
    void closeOpened();

    void switchStyle();
    void increaseFont();
    void decreaseFont();
    void loadConfig();
    void updateConfig();
    void cleanConfig();

    void initConnects();
    void exitApp();

signals:
    void signalClose();
    void signalSave();
    void signalStyle(QString& style);
    void signalCloseApp();
    void signalFontSize(int f);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
