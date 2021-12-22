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
    QLabel* m_question;
    QPushButton* m_just_quit;
    QPushButton* m_save_note;
    QFrame* m_frame;


};

#endif // ASKERWINDOW_H
