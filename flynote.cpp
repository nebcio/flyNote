#include "flynote.h"
#include "ui_flynote.h"

flyNote::flyNote(QWidget *parent, QString path_, QString style_) : QWidget(parent),  ui(new Ui::flyNote), path(path_) { // heh heh
    ui->setupUi(this, style_);
    if (path != nullptr) load_note();
    style = style_;
    init_connects();
}

flyNote::~flyNote() {
    delete ask_for_save;
    delete saving_shortcut;
    delete ui;
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
        if(save_note()){
        emit signal_delete_me(name);
        close();}
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
    ask_for_save = new AskerWindow(nullptr, style);
    connect(ask_for_save, &AskerWindow::signal_save, [this]() {
        save_note();
        emit signal_delete_me(name);
        close();
    });
    connect(ask_for_save, &AskerWindow::signal_quit, [this](){
        emit signal_delete_me(name);
        close(); } );
}

bool flyNote::save_note() {
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
    return true;
}

void flyNote::save_and_close() {
    /*save_note();
    emit signal_delete_me(name);
    close();*/
}

QString flyNote::get_name(){
    return name;
}

void flyNote::switch_style(QString style) {
    ui->button_close->setProperty("style", style);
    // ui->button_close->style()->unpolish(ui->button_close);
    ui->button_close->style()->polish(ui->button_close);
    ui->button_close->update();

    ui->slider_opacity->setProperty("style", style);
    ui->slider_opacity->style()->polish(ui->slider_opacity);
    ui->slider_opacity->update();

    ui->button_trash->setProperty("style", style);
    ui->button_trash->style()->polish(ui->button_trash);
    ui->button_trash->update();

    ui->button_new->setProperty("style", style);
    ui->button_new->style()->polish(ui->button_new);
    ui->button_new->update();

    ui->line->setProperty("style", style);
    ui->line->style()->polish(ui->line);
    ui->line->update();

    ui->checkbox_fly->setProperty("style", style);
    ui->checkbox_fly->style()->polish(ui->checkbox_fly);
    ui->checkbox_fly->update();
}

void flyNote::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
        if (event->pos().x() < 10 || event->pos().x() > width() - 10 ||
                event->pos().y() < 10 || event->pos().y() > height() - 10) {
            start_x = event->pos().x();
            start_y = event->pos().y();
            start_pos = event->globalPos();
            resizing = true;
        }
        else {
            moving = true;
            start_pos = event->globalPos();
        }
    }
}
void flyNote::mouseMoveEvent(QMouseEvent *event) {
    if ((event->pos().x() < 10 && event->pos().y() < 10) || // fdiag ->
        (event->pos().x() > width() - 10 && event->pos().y() > height() - 10)) {
        setCursor(Qt::SizeFDiagCursor);
    }
    else if ((event->pos().x() < 10 && event->pos().y() > height() - 10) || // bdiag <-
             (event->pos().y() < 10 && event->pos().x() > width() - 10)) {
        setCursor(Qt::SizeBDiagCursor);
    }
    else if (event->pos().x() < 10 || event->pos().x() > width() - 10) { // horyzontal
        setCursor(Qt::SizeHorCursor);
    }
    else if (event->pos().y() < 10 || event->pos().y() > height() - 10) { // vertical
        setCursor(Qt::SizeVerCursor);
    }
    else { setCursor(Qt::ArrowCursor); }

    if(moving){
        setCursor(Qt::SizeAllCursor);
        QPoint movePoint = event->globalPos() - start_pos;
        ui->centralwidget->move(ui->centralwidget->pos() + movePoint);
        start_pos = event->globalPos();
    }
    else if (resizing){
        if (start_x < 10 && start_y < 10){ // lewy gorny rog
            int new_width = width() - event->globalX() + pos().x();
            int new_height = height() - event->globalY() + pos().y();

            // obsluga zmiany szerokosci
            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(event->globalX(), pos().y(), new_width, height());
            }

            // obsluga zmiany wysokosci
            if (new_height < minimumSize().height()){
                setGeometry(pos().x(), pos().y(), width(), minimumSize().height());
            }
            else if (new_height > maximumSize().height()) {
                setGeometry(pos().x(), pos().y(), width(), maximumSize().height());
            }
            else {
                setGeometry(pos().x(), event->globalY(), width(), new_height);
            }

        }
        else if (start_x < 10 && start_y > height() - 10) { // lewy dolny
            int new_width = width() - event->globalX() + pos().x();
            int new_height = height() + event->globalY() - start_pos.y();
            int delta_y = height() - start_y;

            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(event->globalX(), pos().y(), new_width, height());
            }

            // obsluga zmiany wysokosci
            if (new_height < minimumSize().height()){
                setGeometry(pos().x(), pos().y(), width(), minimumSize().height());
            }
            else if (new_height > maximumSize().height()) {
                setGeometry(pos().x(), pos().y(), width(), maximumSize().height());
            }
            else {
                setGeometry(pos().x(), pos().y(), width(), new_height);
            }
            start_y = height() - delta_y;
            start_pos.setY(event->globalY());
        }
        else if (start_x > width() - 10 && start_y < 10) { // prawy gorny
            int new_width = width() + event->globalX() - start_pos.x();
            int new_height = height() - event->globalY() + pos().y();
            int delta_x = width() - start_x;

            // obsluga zmiany szerokosci
            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(pos().x(), pos().y(), new_width, height());
            }
            start_x = width() - delta_x;
            start_pos.setX(event->globalX());

            // obsluga zmiany wysokosci
            if (new_height < minimumSize().height()){
                setGeometry(pos().x(), pos().y(), width(), minimumSize().height());
            }
            else if (new_height > maximumSize().height()) {
                setGeometry(pos().x(), pos().y(), width(), maximumSize().height());
            }
            else {
                setGeometry(pos().x(), event->globalY(), width(), new_height);
            }

        }
        else if (start_x > width() - 10 && start_y > height() - 10){ // prawy dolny
            int new_width = width() + event->globalX() - start_pos.x();
            int new_height = height() + event->globalY() - start_pos.y();
            int delta_x = width() - start_x, delta_y = height() - start_y;

            setGeometry(pos().x(), pos().y(), new_width, new_height);
            start_x = width() - delta_x;
            start_y = height() - delta_y;
            start_pos = event->globalPos();
        }
        else if (start_x > width() - 10) {
            int new_width = width() + event->globalX() - start_pos.x();
            int delta_x = width() - start_x;

            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(pos().x(), pos().y(), new_width, height());
            }
            start_x = width() - delta_x;
            start_pos.setX(event->globalX());
        }
        else if (start_y > height() - 10) {
            int new_height = height() + event->globalY() - start_pos.y();
            int delta_y = height() - start_y;
            // obsluga zmiany wysokosci
            if (new_height < minimumSize().height()){
                setGeometry(pos().x(), pos().y(), width(), minimumSize().height());
            }
            else if (new_height > maximumSize().height()) {
                setGeometry(pos().x(), pos().y(), width(), maximumSize().height());
            }
            else {
                setGeometry(pos().x(), pos().y(), width(), new_height);
            }
            start_y = height() - delta_y;
            start_pos.setY(event->globalY());
        }
        else if (start_x < 10) {
            int new_width = width() - event->globalX() + pos().x();

            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(event->globalX(), pos().y(), new_width, height());
            }
        }
        else if (start_y < 10) { //
            int new_height = height() - event->globalY() + pos().y();
            // obsluga zmiany wysokosci
            if (new_height < minimumSize().height()){
                setGeometry(pos().x(), pos().y(), width(), minimumSize().height());
            }
            else if (new_height > maximumSize().height()) {
                setGeometry(pos().x(), pos().y(), width(), maximumSize().height());
            }
            else {
                setGeometry(pos().x(), event->globalY(), width(), new_height);
            }
        }
    }
}
void flyNote::mouseReleaseEvent(QMouseEvent *event) {
    moving = false;
    resizing = false;
    setCursor(Qt::ArrowCursor);
}
