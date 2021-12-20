#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <mainwindow.h>

class TitleBar : public QWidget {
    Q_OBJECT

private:
    MainWindow *m_parent;
    QLabel *m_title;
    QPushButton *m_exit_button;
    QPushButton *m_min_button;
    QHBoxLayout *layout_h;

    QPoint m_restorePos;    // maximize and minimize variables;
    QSize m_restoreSize;

    bool parent_moving;
    QPoint m_startMovePos;

    void initIntefaces();
    void initConnects();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void signalExit();
    void signalMin();
    void signalUpdateConfig();

protected slots:
    void onButtonExitClicked();
    void onButtonMinClicked(); // minimized

public:
    explicit TitleBar(MainWindow *parent = nullptr);
    ~TitleBar();
    void setProperties(QString& style);
};

#endif // TITLEBAR_H
