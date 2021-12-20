#include "flynote.h"
#include "ui_flynote.h"

flyNote::flyNote(QWidget *parent,  QString name, QString path_, QString style_) : QWidget(parent),  ui(new Ui::flyNote), path(path_) { // heh heh
    ui->setupUi(this, style_);
    installEventFilter(this);
    if (path != nullptr) loadNote();
    style = style_;
    initConnects();
    loadConfig();
    ui->status_bar->setText(QString(name));
    compareTime();
}

flyNote::~flyNote() {
    delete saving_shortcut;
    delete ui;
}

void flyNote::initConnects(){
    connect(ui->button_new, &QPushButton::clicked, this, &flyNote::createNew);
    connect(ui->button_trash, &QPushButton::clicked, this, &flyNote::deleteNote);
    connect(ui->button_close, &QPushButton::clicked, this, &flyNote::closeNote);
    connect(ui->checkbox_fly, &QCheckBox::stateChanged, this, &flyNote::flySet);
    connect(ui->slider_opacity, &QSlider::valueChanged, this, &flyNote::opacitySet);
}

void flyNote::loadConfig() {
    /* Create list of flyNotes */
    QFile config_file("config.json"); // podobno lepiej QFileInfo, ale to na koniec
    QString file_text = "";
    config_file.open(QIODevice::ReadOnly | QIODevice::Text);
    file_text = config_file.readAll();
    config_file.close();
    font_size = QJsonDocument::fromJson(file_text.toUtf8()).object().find(QString("MainWindow")).value().toObject().find(QString("font-size")).value().toInt();
    if (QJsonDocument::fromJson(file_text.toUtf8()).object().contains(path)){
        config = QJsonDocument::fromJson(file_text.toUtf8()).object().find(QString(path)).value().toObject();   // create object json based on value of file
    }
    else { // default config, just for mainwindow
        const QString init_json(QStringLiteral("{ \"x\": 900,"
"                                               \"y\": 300,"
"                                               \"width\": 360,"
"                                               \"height\": 320,"
"                                               \"fly\": false,"
"                                               \"opacity\": 255}"));
        config = QJsonObject(QJsonDocument::fromJson(init_json.toUtf8()).object());
    }

    setGeometry(config.find(QString("x")).value().toInt(),
                config.find(QString("y")).value().toInt(),
                config.find(QString("width")).value().toInt(),
                config.find(QString("height")).value().toInt());

    ui->slider_opacity->setValue(config.find(QString("opacity")).value().toDouble());

    if (config.find(QString("fly")).value().toBool() == true) {
        ui->checkbox_fly->setChecked(true);
    }

    ui->text_space->setFont(QFont("Arial", font_size));
}

void flyNote::updateConfig() {
    /* Update config file */
    QFile config_file("config.json"); // podobno lepiej QFileInfo, ale to na koniec
    QString file_text = "";
    QJsonObject all_config;
    if (config_file.exists()){
        config_file.open(QIODevice::ReadOnly | QIODevice::Text);
        file_text = config_file.readAll();
        config_file.close();
        all_config = QJsonDocument::fromJson(file_text.toUtf8()).object();   // create object json based on value of file

        all_config.insert(QString(path), config);
        config_file.open(QIODevice::WriteOnly | QIODevice::Text);
        config_file.write(QJsonDocument(all_config).toJson());
        config_file.close();
    }
}

void flyNote::updateFontSize(int f) {
    ui->text_space->setFont(QFont("Arial", f));
}

void flyNote::compareTime() {
    qDebug() << "Notification! Time: " << QDateTime::currentDateTime();
}

void flyNote::setTimeNotification() {

}

void flyNote::sendNotification() {
    qDebug() << "Notification! Time: " << time_notification << " " << QDateTime::currentDateTime();
}

void flyNote::loadNote() {
    /* Open note, only main window can call it. Read non latin characters
     * only if file is encoding in UTF-8 or UTF-8 BOM */
    QFile note(path);

    // in.setCodec("UTF-8"); // UTF-8 is default
    if (note.exists()) {
        QTextStream in(&note);
        note.open(QIODevice::ReadOnly);

        ui->text_space->setText(QString(note.readAll()).toUtf8());
        note.close();
    }
}

void flyNote::closeNote() {
    /* Close note */
    if (path == nullptr){
        askToSave();
    }
    else {
        if(saveNote()){
            emit signalDeleteMe(name);
            close();}
    }
}

void flyNote::flySet(int a) {
    /* Enable/Disable WindowStaysOnTopHint flag */
    if (a == 2) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        config.insert("fly", true);
    }
    else if (a == 0) {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
        config.insert("fly", false);
    }

    updateConfig();
    show();
}

void flyNote::deleteNote() {

}

void flyNote::createNew() {
    emit signalCreateNew(nullptr);
}

void flyNote::opacitySet(int a) {
    /* Set the opacity of window between 30 and 255(100% visible) */
    setWindowOpacity(a / 255.0);
    config.insert("opacity", windowOpacity() * 255.0);
    updateConfig();
}

void flyNote::askToSave() {
    m_ask_for_save = new AskerWindow(nullptr, style);
    connect(m_ask_for_save, &AskerWindow::signalSave, [this]() {
        saveNote();
        emit signalDeleteMe(name);
        close();
    });
    connect(m_ask_for_save, &AskerWindow::signalQuit, [this](){
        emit signalDeleteMe(name);
        close();
    });
}

bool flyNote::saveNote() {
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

void flyNote::saveClose() {
    saveNote();
    emit signalDeleteMe(name);
    close();
}

void flyNote::switchStyle(QString& style) {
    /* Update interface to given style */
    this->style = style;
    ui->button_close->setProperty("style", style);
    // ui->button_close->style()->unpolish(ui->button_close);
    ui->button_close->style()->polish(ui->button_close);
    ui->button_close->update();

    ui->slider_opacity->setProperty("style", style);
    ui->slider_opacity->style()->polish(ui->slider_opacity);
    ui->slider_opacity->update();

    ui->button_calendar->setProperty("style", style);
    ui->button_calendar->style()->polish(ui->button_calendar);
    ui->button_calendar->update();

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

    ui->status_bar->setProperty("style", style);
    ui->status_bar->style()->polish(ui->status_bar);
    ui->status_bar->update();
}

void flyNote::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
        if (event->pos().x() < 10 || event->pos().x() > width() - 10 ||
                event->pos().y() < 10 || event->pos().y() > height() - 10) { // resize
            start_x = event->pos().x();
            start_y = event->pos().y();
            start_pos = event->globalPos();
            resizing = true;
        }
        else { // move
            moving = true;
            start_pos = event->globalPos();
        }
    }
}
bool flyNote::eventFilter(QObject *obj, QEvent *e) {
    /* Changing shape of cursor on boundaries */
    if (e->type() == QEvent::MouseMove) {
        QMouseEvent* event = static_cast<QMouseEvent*>(e);

        const auto main_pos = pos();
        const auto cur_pos = event->globalPos() - main_pos;

        if ((cur_pos.x() < 10 && cur_pos.y() < 10) || // fdiag ->
                (cur_pos.x() > width() - 10 && cur_pos.y() > height() - 10)) {
            setCursor(Qt::SizeFDiagCursor);
        }
        else if ((cur_pos.x() < 10 && cur_pos.y() > height() - 10) || // bdiag <-
                 (cur_pos.y() < 10 && cur_pos.x() > width() - 10)) {
            setCursor(Qt::SizeBDiagCursor);
        }
        else if (cur_pos.x() < 10 || cur_pos.x() > width() - 10 ) { // horyzontal
            setCursor(Qt::SizeHorCursor);
        }
        else if (cur_pos.y() < 10 || cur_pos.y() > height() - 10) { // vertical
            setCursor(Qt::SizeVerCursor);
        }
        else { setCursor(Qt::ArrowCursor);}
    }
    return false;
}
void flyNote::mouseMoveEvent(QMouseEvent *event) {
    /* Changing shape of cursor on boundaries */
    // moving and resizing
    if(moving){
        setCursor(Qt::SizeAllCursor);
        QPoint movePoint = event->globalPos() - start_pos;
        move(pos() + movePoint);
        start_pos = event->globalPos();
    }
    else if (resizing) {
        if (start_x < 10 && start_y < 10){                              // top-left
            const int new_width = width() - event->globalX() + pos().x();
            const int new_height = height() - event->globalY() + pos().y();

            // changing width
            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(event->globalX(), pos().y(), new_width, height());
            }

            // changing height
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
        else if (start_x < 10 && start_y > height() - 10) {             // bottom-left
            const int new_width = width() - event->globalX() + pos().x();
            const int new_height = height() + event->globalY() - start_pos.y();
            const int delta_y = height() - start_y;

            // changing width
            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(event->globalX(), pos().y(), new_width, height());
            }

            // changing height
            if (new_height < minimumSize().height()){
                setGeometry(pos().x(), pos().y(), width(), minimumSize().height());
            }
            else if (new_height > maximumSize().height()) {
                setGeometry(pos().x(), pos().y(), width(), maximumSize().height());
            }
            else {
                setGeometry(pos().x(), pos().y(), width(), new_height);
            }

            // update new pos to variables
            start_y = height() - delta_y;
            start_pos.setY(event->globalY());
        }
        else if (start_x > width() - 10 && start_y < 10) {              // top-right
            int new_width = width() + event->globalX() - start_pos.x();
            int new_height = height() - event->globalY() + pos().y();
            int delta_x = width() - start_x;

            // changing width
            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(pos().x(), pos().y(), new_width, height());
            }

            // update new pos to variables
            start_x = width() - delta_x;
            start_pos.setX(event->globalX());

            // changing height
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
        else if (start_x > width() - 10 && start_y > height() - 10){    // bottom-right
            const int new_width = width() + event->globalX() - start_pos.x();
            const int new_height = height() + event->globalY() - start_pos.y();
            const int delta_x = width() - start_x, delta_y = height() - start_y;

            // changing width and height
            setGeometry(pos().x(), pos().y(), new_width, new_height);

            // update new pos to variables
            start_x = width() - delta_x;
            start_y = height() - delta_y;
            start_pos = event->globalPos();
        }
        else if (start_x > width() - 10) {                              // right
            const int new_width = width() + event->globalX() - start_pos.x();
            const int delta_x = width() - start_x;

            // changing width
            if (new_width < minimumSize().width()){
                setGeometry(pos().x(), pos().y(), minimumSize().width(), height());
            }
            else if (new_width > maximumSize().width()) {
                setGeometry(pos().x(), pos().y(), maximumSize().width(), height());
            }
            else {
                setGeometry(pos().x(), pos().y(), new_width, height());
            }

            // update new pos to variables
            start_x = width() - delta_x;
            start_pos.setX(event->globalX());
        }
        else if (start_y > height() - 10) {                             // bottom
            const int new_height = height() + event->globalY() - start_pos.y();
            const int delta_y = height() - start_y;

            // changing height
            if (new_height < minimumSize().height()){
                setGeometry(pos().x(), pos().y(), width(), minimumSize().height());
            }
            else if (new_height > maximumSize().height()) {
                setGeometry(pos().x(), pos().y(), width(), maximumSize().height());
            }
            else {
                setGeometry(pos().x(), pos().y(), width(), new_height);
            }

            // update new pos to variables
            start_y = height() - delta_y;
            start_pos.setY(event->globalY());
        }
        else if (start_x < 10) {                                        // left
            const int new_width = width() - event->globalX() + pos().x();

            // changing width
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
        else if (start_y < 10) {                                        // top
            const int new_height = height() - event->globalY() + pos().y();

            // changing height
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
    setCursor(Qt::ArrowCursor); // return do default cursor after moving

    config.insert("width", width());
    config.insert("height", height());
    config.insert("x", pos().x());
    config.insert("y", pos().y());
    updateConfig();
}
