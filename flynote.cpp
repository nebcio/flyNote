#include "flynote.h"
#include "ui_flynote.h"

flyNote::flyNote(QWidget *parent, QString path_) : QWidget(parent),  ui(new Ui::flyNote), path(path_) { // heh heh
    ui->setupUi(this);
    qDebug() << "Path: " << path;
    if (path != nullptr) load_note();
    init_connects();
}

flyNote::~flyNote() {
    delete ui;
    delete ask_for_save;
    // delete path;
}

void flyNote::init_connects(){
    saving_shortcut = new QShortcut(QKeySequence::Save, this, SLOT(save_note()));
    connect(ui->button_new, SIGNAL(clicked()), this, SLOT(create_new()));
    connect(ui->button_trash, SIGNAL(clicked()), this, SLOT(delete_note()));
    connect(ui->button_close, SIGNAL(clicked()), this, SLOT(close_note()));
    connect(ui->checkbox_fly, SIGNAL(stateChanged(int)), this, SLOT(fly_set(int)));
    connect(ui->slider_opacity, SIGNAL(valueChanged(int)), this, SLOT(opacity_set(int)));

}

void flyNote::load_note() {
    /* Open note, only main window can call it. Read non latin characters
     * only if file is encoding in UTF-8 or UTF-8 BOM */

    QFile note(path);
    QTextStream in(&note);

    // in.setCodec("UTF-8"); // UTF-8 jest domyslny
    note.open(QIODevice::ReadOnly);

    ui->text_space->setText(QString(note.readAll()).toUtf8());
    note.close();
}

void flyNote::close_note() {
    /* Close note */
    if (path == nullptr){
        ask_to_save();
    }
    else {
        save_and_close();
    }
}

void flyNote::fly_set(int a) {
    /* Enable/Disable WindowStaysOnTopHint flag */

    if (a == 2) setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else if (a == 0) setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);

    show();
}

void flyNote::delete_note() {

}

void flyNote::create_new() {
    emit signal_create_new(nullptr);
}

void flyNote::opacity_set(int a) {
    /* Set the opacity of window between 30 and 255(100% visible) */
    setWindowOpacity(a / 255.0);
}

void flyNote::ask_to_save() {
    ask_for_save = new AskerWindow();
    connect(ask_for_save, SIGNAL(signal_save()), this, SLOT(save_and_close()));
    connect(ask_for_save, &AskerWindow::signal_quit, [this](){
        emit signal_delete_me();
        close(); } );
}

void flyNote::save_note() {
    if (path != nullptr) {
        QFile note(path); // podobno lepiej QFileInfo, ale to na koniec

        if (note.exists()){
            note.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out(&note);
            out << ui->text_space->toPlainText();
            note.close();
        }
    }
    else if (path == nullptr){
        path = QFileDialog::getSaveFileName(this, tr("Save as..."), tr("flynote.txt"), tr("*.txt"));
        QFile note(path);
        QFileInfo notr(path);
        name = notr.baseName();
        note.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&note);
        out << ui->text_space->toPlainText();
        note.close();

        QFile flynotes("flynotes.json"); // podobno lepiej QFileInfo, ale to na koniec
        QString file_text = "";
        QJsonObject files;
        if (flynotes.exists()){
            flynotes.open(QIODevice::ReadOnly | QIODevice::Text);
            file_text = flynotes.readAll();
            flynotes.close();
            files = QJsonDocument::fromJson(file_text.toUtf8()).object();   // create object json based on value of file
        }
        else {
            files = QJsonObject();                                 // empty json object
        }
        files[path] = name;
        if(flynotes.open(QIODevice::WriteOnly | QIODevice::Text)){
            flynotes.write(QJsonDocument(files).toJson());
        }
        flynotes.close();
    }
}

void flyNote::save_and_close() {
    save_note();
    emit signal_delete_me();
    close();
}

QString flyNote::get_name(){
    return name;
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
