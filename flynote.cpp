#include "flynote.h"
#include "ui_flynote.h"
#include <QMouseEvent>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

flyNote::flyNote(QWidget *parent, QString path_) : QWidget(parent),  ui(new Ui::flyNote), path(path_) { // heh heh
    ui->setupUi(this);
        qDebug() << path;
    if (path != nullptr) load_note();
}

flyNote::~flyNote() {
    delete ui;
    // delete path;
}

void flyNote::init_connects(){
    //saving_shortcut = new QShortcut(QKeySequence::Save, this, SLOT(save_note()));
    connect(ui->button_new, SIGNAL(clicked()), this, SLOT(create_new()));
    connect(ui->button_trash, SIGNAL(clicked()), this, SLOT(delete_note()));
    connect(ui->button_close, SIGNAL(clicked()), this, SLOT(close_note()));
    connect(ui->checkbox_fly, SIGNAL(stateChanged(bool)), this, SLOT(fly_set()));
    connect(ui->slider_opacity, SIGNAL(sli(int)), this, SLOT(opacity_set(int)));

}

void flyNote::load_note() {
    /* Open note, only main window can call it */
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QFile note(path);
    QTextStream in(&note);
    in.setCodec("UTF-8");

    note.open(QIODevice::ReadOnly | QIODevice::Text);
    ui->text_space->setText(in.readAll());
    qDebug() << QString::fromUtf8(QString(note.readAll()).toUtf8());
    note.close();
}

void flyNote::close_note() {
    /* Close note */
    if (path == NULL){
        ask_to_save();
    }
    else {
        save_note();
        emit delete_note();
        close();
    }
}

void flyNote::fly_set() {
    /* Enable/Disable WindowStaysOnTopHint flag */
    qDebug() << ui->centralwidget->objectName() << "fly";
    if (ui->checkbox_fly->isChecked()) {
        ui->centralwidget->setWindowFlags(Qt::WindowFlags() | Qt::WindowStaysOnTopHint);
        qDebug() << ui->centralwidget->parentWidget()->objectName();
    }
    else {
        ui->centralwidget->setWindowFlags(Qt::WindowFlags() & ~Qt::WindowStaysOnTopHint);
    }

    ui->centralwidget->show();
}

void flyNote::delete_note() {

}

void flyNote::create_new() {
    emit signal_create_new(nullptr);
}

void flyNote::opacity_set(int a) { // nie działa
    qDebug() << ui->centralwidget->objectName() << "opacity";
    ui->centralwidget->setWindowOpacity(a / 255.0); // nie poatrzeba odwolania do ui ?
}

void flyNote::save_note() {

}

void flyNote::ask_to_save() {

}

void flyNote::save_and_close() {
    save_note();
    emit signal_delete_me();
    close();
}

void flyNote::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
        moving = true;
        startMovePos = event->globalPos();
    }
}
void flyNote::mouseMoveEvent(QMouseEvent *event) {
    if(moving){
        QPoint movePoint = event->globalPos() - startMovePos;
        ui->centralwidget->move(ui->centralwidget->pos() + movePoint);
        startMovePos = event->globalPos();
    }
}
void flyNote::mouseReleaseEvent(QMouseEvent *event) {
    moving = false;
}
