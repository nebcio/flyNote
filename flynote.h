#ifndef FLYNOTE_H
#define FLYNOTE_H

#include <QWidget>
#include <QMouseEvent>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QShortcut>
#include <QMouseEvent>
#include <QDateTime>
#include <QCursor>
#include <QTimer>
#include <QMessageBox>
#include "askerwindow.h"
#include "panelinputtime.h"

QT_BEGIN_NAMESPACE
namespace Ui { class flyNote; }
QT_END_NAMESPACE

class flyNote : public QWidget
{
    Q_OBJECT
private:
    const QShortcut *saving_shortcut = new QShortcut(QKeySequence::Save, this, SLOT(saveNote()));
    Ui::flyNote *ui;

    QString name;
    QString path;

    bool moving = false;
    bool resizing = false;

    QJsonObject config;
    int font_size = 15;
    QPoint start_pos;
    int start_x;
    int start_y;
    QString style = "blue"; // for dialog window

    QDateTime time_notification;
    bool panel_opened = false;
    bool notified;
    QTimer* timer;
    QMessageBox* notification;

    AskerWindow* m_ask_for_save = nullptr;


public:
    explicit flyNote(QWidget *parent = nullptr, QString name="flyNote –", QString path=nullptr, QString style="blue");
    ~flyNote();

signals:
    void signalCreateNew(QString*);
    void signalUpdateList();
    void signalCleanConfig();
    void signalclosePanelTime();

private slots:
    void initConnects();
    void loadNote();
    void closeNote();
    void flySet(int a);
    void deleteNote();
    void createNew();
    void opacitySet(int a);
    bool saveNote();
    void askToSave();
    void closeBeforeDeleting(QString some_path);

    void showPanelTime();
    void compareTime();
    void setTimeNotification(QDateTime date_time);
    void sendNotification();
    void removeNotification();

    void loadConfig();
    void updateConfig();
    void updateFontSize(int f);
    void switchStyle(QString& style);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *e) override;
};

#endif // FLYNOTE_H
