#ifndef ASKERWINDOW_H
#define ASKERWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class AskerWindow : public QWidget
{
    Q_OBJECT
signals:
    void signal_save();
    void signal_quit();

public:
    explicit AskerWindow(QWidget *parent = nullptr);
    ~AskerWindow();

private slots:
    void close_parent();
    void save_parent();

private:
    QLabel* question;
    QPushButton* just_quit;
    QPushButton* save_note;


};

#endif // ASKERWINDOW_H
