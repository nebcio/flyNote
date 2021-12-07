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
    QWidget *m_parent;
    //QLabel *m_icon;
    QLabel *m_title;
    QPushButton *m_exit_button;
    QPushButton *m_min_button;
    QHBoxLayout *layout_h;
    QPoint m_restorePos;    // maximize and minimize variables;
    QSize m_restoreSize;

    bool parent_moving;
    QPoint m_startMovePos;

    void init_intefaces();
    void init_connects();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void signal_exit(); // exit //
    void signal_min();

protected slots:
    void on_button_exit_clicked();
    void on_button_min_clicked(); // minimized
    void switch_style(QString style);

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

    void saveRestoreInfo(const QPoint point, const QSize size); // Save/Get the position and size of the window before maximizing;
};

#endif // TITLEBAR_H
