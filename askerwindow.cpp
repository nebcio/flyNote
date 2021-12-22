#include "askerwindow.h"
#include <QDebug>

AskerWindow::AskerWindow(QWidget *parent, QString style) : QWidget(parent) {

    this->resize(200, 70);
    m_frame = new QFrame(this);{
    m_frame->setFixedSize(200, 70);
    m_frame->setGeometry(0, 0, 200, 70);
    m_frame->setStyleSheet("border: 1px solid #888");}

    m_question = new QLabel(this); {
    m_question->setGeometry(15, 10, 170, 25);
    m_question->setText("Quit or save and quit?");
    m_question->setFont(QFont("Roboto", 13)); }

    m_save_note = new QPushButton(this);{
    m_save_note->setGeometry(100, 40, 45, 25);
    m_save_note->setText("Save");
    m_save_note->setProperty("style", style);}
    connect(m_save_note, &QPushButton::clicked, this, &AskerWindow::saveParent);

    m_just_quit = new QPushButton(this);{
    m_just_quit->setGeometry(150, 40, 45, 25);
    m_just_quit->setText("Quit");
    m_just_quit->setProperty("style", style);}
    connect(m_just_quit, &QPushButton::clicked, this, &AskerWindow::closeParent);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    show();
}

AskerWindow::~AskerWindow(){
    delete m_question;
    delete m_save_note;
    delete m_just_quit;
}

void AskerWindow::saveParent(){
    /* Trigger saving note, then closing */
    emit signalSave();
    close();
}

void AskerWindow::closeParent(){
    /* Trigger closing note */
    emit signalQuit();
    close();
}

