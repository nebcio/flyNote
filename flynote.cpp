#include "flynote.h"
#include "ui_flynote.h"
#include <QMouseEvent>

flyNote::flyNote(QWidget *parent) : QWidget(parent),  ui(new Ui::flyNote) {
    ui->setupUi(this);
    // popraw konstruktor
}

flyNote::~flyNote() {
    delete ui;
}

void flyNote::init_connects(){
    //saving_shortcut = new QShortcut(QKeySequence::Save, this, SLOT(save_note()));
    connect(ui->button_new, SIGNAL(clicked()), this, SLOT(create_new()));
    connect(ui->button_trash, SIGNAL(clicked()), this, SLOT(delete_note()));
    connect(ui->button_close, SIGNAL(clicked()), this, SLOT(close_note()));
    connect(ui->checkbox_fly, SIGNAL(stateChanged()), this, SLOT(fly_set()));
    connect(ui->slider_opacity, SIGNAL(valueChanged()), this, SLOT(opacity_set()));
}

void flyNote::load_note() {
    /* Open note, only main window can call it */
    QFile note(path);
    note.open(QIODevice::ReadOnly | QIODevice::Text);
    ui->text_space->setText(note.readAll());
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
    if (ui->checkbox_fly->isChecked()) {
        setWindowFlags(Qt::WindowFlags() | Qt::WindowStaysOnTopHint);
    }
    else {
        setWindowFlags(Qt::WindowFlags() & ~Qt::WindowStaysOnTopHint);
    }

    show();
}

void flyNote::delete_note() {

}

void flyNote::create_new() {
    emit signal_create_new();
} // potrzeba?

void flyNote::opacity_set() {
    setWindowOpacity(ui->slider_opacity->value() / 255); // nie poatrzeba odwolania do ui ?
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
        startMovePos = event->pos();
    }
}
void flyNote::mouseMoveEvent(QMouseEvent *event) {
    if(moving){ // !!!!!!!!!!!!!!!!!!!!!!!!!
        move(event->globalPos() - startMovePos);
    }
}

void flyNote::mouseReleaseEvent(QMouseEvent *event) {
    // jesli potrzeba bedzie zmieniac kursor a jak nie to usun
}
