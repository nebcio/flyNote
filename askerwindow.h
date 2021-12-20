#ifndef ASKERWINDOW_H
#define ASKERWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class AskerWindow : public QWidget
{
    Q_OBJECT
signals:
    void signalSave();
    void signalQuit();

public:
    explicit AskerWindow(QWidget *parent = nullptr, QString style="blue");
    ~AskerWindow();

private slots:
    void closeParent();
    void saveParent();

private:
    QLabel* question;
    QPushButton* just_quit;
    QPushButton* save_note;
    QFrame* frame;


};

#endif // ASKERWINDOW_H
