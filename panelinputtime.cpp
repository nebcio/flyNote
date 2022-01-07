#include "panelinputtime.h"

PanelInputTime::PanelInputTime(QString style, int x, int y, QWidget *parent) : QWidget(parent) {

    this->setGeometry(x, y, 300, 200);
    m_frame = new QFrame(this);{
    m_frame->setFixedSize(300, 200);
    m_frame->setGeometry(0, 0, 300, 200);
    m_frame->setStyleSheet("border: 1px solid #146b70");}

    m_calendar = new QCalendarWidget(this);{
    m_calendar->setGeometry(10, 10, 280, 140);
    m_calendar->setProperty("style", style);
    m_calendar->setGridVisible(false);
    m_calendar->setSelectionMode(QCalendarWidget::SingleSelection);
    m_calendar->setMinimumDate(QDate::currentDate());}
    connect(m_calendar, &QCalendarWidget::selectionChanged, this, &PanelInputTime::takeDateTime);

    m_time_describe = new QLabel(this); {
    m_time_describe->setGeometry(10, 160, 50, 30);
    m_time_describe->setText("Time");
    m_time_describe->setFont(QFont("Roboto", 13)); }

    m_time_input = new QTimeEdit(this); {
    m_time_input->setGeometry(60, 160, 60, 30);
    connect(m_time_input, &QTimeEdit::timeChanged, this, &PanelInputTime::takeDateTime);
    }

    m_remove_notification = new QPushButton(this);{
    m_remove_notification->setGeometry(240, 160, 50, 30);
    m_remove_notification->setText("Remove");
    m_remove_notification->setProperty("style", style);}
    connect(m_remove_notification, &QPushButton::clicked, [this](){ emit signalRemoveNotification(); });

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    show();
}

void PanelInputTime::takeDateTime() {

    QDateTime date_time = QDateTime();
    date_time.setDate(m_calendar->selectedDate());
    date_time.setTime(m_time_input->time());

    emit signalSetNotification(date_time);
}

void PanelInputTime::loadNotificationTime() {

}
