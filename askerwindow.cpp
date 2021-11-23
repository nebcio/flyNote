#include "askerwindow.h"
#include <QDebug>

AskerWindow::AskerWindow(QWidget *parent) : QWidget(parent) {

    this->resize(200, 70);

    question = new QLabel(this);
    question->setGeometry(15, 10, 170, 25);
    question->setText("Quit or save and quit?");
    question->setFont(QFont("Roboto", 13));

    save_note = new QPushButton(this);
    save_note->setGeometry(100, 40, 45, 25);
    save_note->setText("Save and quit");
    connect(save_note, SIGNAL(clicked()), this, SLOT(save_parent()));

    just_quit = new QPushButton(this);
    just_quit->setGeometry(150, 40, 45, 25);
    just_quit->setText("Quit");
    connect(just_quit, SIGNAL(clicked()), this, SLOT(close_parent()));

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    show();
}

AskerWindow::~AskerWindow(){
    delete question;
    delete save_note;
    delete just_quit;
}

void AskerWindow::save_parent(){
    emit signal_save();
    close();
}

void AskerWindow::close_parent(){
    emit signal_quit();
    close();
}

