#ifndef PANELINPUTTIME_H
#define PANELINPUTTIME_H

#include <QWidget>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QDate>

class PanelInputTime : public QWidget
{
    Q_OBJECT
public:
    explicit PanelInputTime(QString style="blue", int x = 900, int y = 400, QWidget *parent = nullptr);

signals:
    void signalRemoveNotification();
    void signalSetNotification(QDateTime date_time);

private slots:
    void takeDateTime();
    void loadNotificationTime();

private:
    QCalendarWidget* m_calendar;
    QTimeEdit* m_time_input;
    QFrame* m_frame;
    QLabel* m_time_describe;
    QPushButton* m_remove_notification;

};

#endif // PANELINPUTTIME_H
