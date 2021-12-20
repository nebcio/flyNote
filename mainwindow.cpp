#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flynote.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QString>
#include <QVector>
#include <QMouseEvent>
#include <QStringLiteral>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    loadConfig();
    initConnects();
    files();
}

void MainWindow::loadConfig() {
    /* Create list of flyNotes */
    QFile config_file("config.json"); // podobno lepiej QFileInfo, ale to na koniec
    QString file_text = "";
    if (config_file.exists()) {
        config_file.open(QIODevice::ReadOnly | QIODevice::Text);
        file_text = config_file.readAll();
        config_file.close();
        config = QJsonDocument::fromJson(file_text.toUtf8()).object().find(QString("MainWindow")).value().toObject();   // create object json based on value of file
    }
    else { // default config, just for mainwindow
        const QString init_json(QStringLiteral("{ \"MainWindow\": { \"style\": \"blue\","
"                                              \"font-size\": 15,"
"                                              \"x\": 900,"
"                                              \"y\": 300,"
"                                              \"width\": 360,"
"                                              \"height\": 320}}"));
        config = QJsonObject(QJsonDocument::fromJson(init_json.toUtf8()).object());
        updateConfig();
    }
    setGeometry(config.find(QString("x")).value().toInt(),
                config.find(QString("y")).value().toInt(),
                config.find(QString("width")).value().toInt(),
                config.find(QString("height")).value().toInt());

    font_size = config.find(QString("font-size")).value().toInt();
    ui->listWidget->setFont(QFont("Arial", font_size));

    if(style != config.find(QString("style")).value().toString()) switchStyle();

}

void MainWindow::updateConfig() {
    /* Update config file */
    QFile config_file("config.json"); // podobno lepiej QFileInfo, ale to na koniec
    QString file_text = "";
    QJsonObject all_config;
    if (config_file.exists()){
        config_file.open(QIODevice::ReadOnly | QIODevice::Text);
        file_text = config_file.readAll();
        config_file.close();
        all_config = QJsonDocument::fromJson(file_text.toUtf8()).object();   // create object json based on value of file
    }
    all_config.insert("MainWindow", config);
    config_file.open(QIODevice::WriteOnly | QIODevice::Text);
    config_file.write(QJsonDocument(all_config).toJson());
    config_file.close();

}

void MainWindow::cleanConfig() {
    QFile config_file("config.json"); // podobno lepiej QFileInfo, ale to na koniec
    QString file_text = "";
    QJsonObject all_config;
    if (config_file.exists()) {
        config_file.open(QIODevice::ReadOnly | QIODevice::Text);
        file_text = config_file.readAll();
        config_file.close();
        all_config = QJsonDocument::fromJson(file_text.toUtf8()).object();   // create object json based on value of file
        const QStringList configs_keys = all_config.keys();
        for (QString path : configs_keys) {
            if (!names_files.contains(path) && path != "MainWindow") {
                all_config.remove(path);
            }
        }
        config_file.open(QIODevice::WriteOnly | QIODevice::Text);
        config_file.write(QJsonDocument(all_config).toJson());
        config_file.close();
    }
    else {
        loadConfig();
    }
}

MainWindow::~MainWindow() {
    delete saving_shortcut;
    delete decrease_shortcut;
    delete increase_shortcut;
    delete ui;
}

void MainWindow::initConnects() {
    connect(ui->button_open, &QPushButton::clicked, this, &MainWindow::openSelected);
    connect(ui->button_trash, &QPushButton::clicked, this, &MainWindow::deleteSelected);
    connect(ui->button_close, &QPushButton::clicked, this, &MainWindow::closeOpened);
    connect(ui->button_new, SIGNAL(clicked()), this, SLOT(createNew()));
    connect(ui->button_style, &QPushButton::clicked, this, &MainWindow::switchStyle);
    connect(ui->title_bar, SIGNAL(signalExit()), this, SLOT(exitApp()));
}

void MainWindow::exitApp() {
    // przed musi zapisac wszystkie notatki czy cos
    QCoreApplication::quit();
}

void MainWindow::files() {
    /* Create list of flyNotes */
    QFile flynotes("flynotes.json"); // podobno lepiej QFileInfo, ale to na koniec
    QString file_text = "";
    if (flynotes.exists()){
        flynotes.open(QIODevice::ReadOnly | QIODevice::Text);
        file_text = flynotes.readAll();
        flynotes.close();
        names_files = QJsonDocument::fromJson(file_text.toUtf8()).object();   // create object json based on value of file
    }
    else {
        names_files = QJsonObject();                                 // empty json object
    }

    ui->listWidget->clear();
    for (QString &element : names_files.keys()){
        QListWidgetItem *item = new QListWidgetItem(QString(QJsonValue(names_files.value(element)).toString()));
        item->setToolTip(QString(element));
        ui->listWidget->addItem(item);
    }
}

void MainWindow::openSelected() {
    /* Open selected notes */
    for(QListWidgetItem *item : ui->listWidget->selectedItems()){
        QString tmp_path = item->toolTip();
        createNew(&tmp_path, item->text());
        item->setSelected(false);
    }
}

void MainWindow::deleteSelected() {
    /* Delete selected notes */
    for(QListWidgetItem *item : ui->listWidget->selectedItems()){
        QString tmp_path = item->toolTip();
        QFile file_item(tmp_path);

        if (file_item.exists()) {
            file_item.remove();
        }

        if (names_files.keys().contains(tmp_path)){
            names_files.remove(tmp_path);
        }

    }

    QFile flynotes("flynotes.json"); // podobno lepiej QFileInfo, ale to na koniec
    if (flynotes.exists()){
        flynotes.open(QIODevice::WriteOnly | QIODevice::Text);
        flynotes.write(QJsonDocument(names_files).toJson());
        flynotes.close();
    }

    files();
    cleanConfig();
}

void MainWindow::createNew(QString* path, QString name) {
    /*if (path == nullptr) {
        children_window.push_back(new flyNote(nullptr, nullptr, style));
    }
    else if (path != nullptr) {
        children_window.push_back(new flyNote(nullptr, *path, style));
    }

    children_window.back()->show();            // zabezpieczenie przed pustym by nie bylo undefined behavior
    connect(this, SIGNAL(signalClose()), children_window.back(), SLOT(closeNote()));
    connect(this, SIGNAL(signalSave()), children_window.back(), SLOT(saveNote()));
    connect(this, SIGNAL(signalStyle(QString&)), children_window.back(), SLOT(switchStyle(QString&)));

    connect(children_window.back(), SIGNAL(signalCreateNew(QString*)), this, SLOT(createNew(QString*)));
    connect(children_window.back(), SIGNAL(signalDeleteMe(QString)), this, SLOT(cleanData(QString)));
    connect(children_window.back(), SIGNAL(signalUpdateList()), this, SLOT(files()));*/

    flyNote *fN;

    if (path == nullptr) {
        fN = new flyNote(nullptr, name, nullptr, style); // with default name
        fN->show();
        connect(this, SIGNAL(signalClose()), fN, SLOT(closeNote()));
        connect(this, SIGNAL(signalSave()), fN, SLOT(saveNote()));
        connect(this, SIGNAL(signalStyle(QString&)), fN, SLOT(switchStyle(QString&)));

        connect(fN, SIGNAL(signalCreateNew(QString*)), this, SLOT(createNew(QString*)));
        connect(fN, SIGNAL(signalUpdateList()), this, SLOT(files()));
    }
    else if (path != nullptr) {
        fN = new flyNote(nullptr, name, *path, style);
        fN->show();
        connect(this, SIGNAL(signalClose()), fN, SLOT(closeNote()));
        connect(this, SIGNAL(signalSave()), fN, SLOT(saveNote()));
        connect(this, SIGNAL(signalStyle(QString&)), fN, SLOT(switchStyle(QString&)));
        connect(this, SIGNAL(signalFontSize(int)), fN, SLOT(updateFontSize(int)));

        connect(fN, SIGNAL(signalCreateNew(QString*)), this, SLOT(createNew(QString*)));
        connect(fN, SIGNAL(signalUpdateList()), this, SLOT(files()));
    }
}

void MainWindow::saveOpened() {
    /* Save opened notes */
    emit signalSave();
}

void MainWindow::closeOpened() {
    /* Close opened notes */
    emit signalClose();
}

void MainWindow::switchStyle() {
    if (style == "blue") { style = "red"; }
    else { style = "blue"; }

    emit signalStyle(style);

    ui->button_close->setProperty("style", style);
    // ui->button_close->style()->unpolish(ui->button_close);
    ui->button_close->style()->polish(ui->button_close);
    ui->button_close->update();

    ui->button_open->setProperty("style", style);
    ui->button_open->style()->polish(ui->button_open);
    ui->button_open->update();

    ui->button_help->setProperty("style", style);
    ui->button_help->style()->polish(ui->button_help);
    ui->button_help->update();

    ui->button_style->setProperty("style", style);
    ui->button_style->style()->polish(ui->button_style);
    ui->button_style->update();

    ui->button_trash->setProperty("style", style);
    ui->button_trash->style()->polish(ui->button_trash);
    ui->button_trash->update();

    ui->button_new->setProperty("style", style);
    ui->button_new->style()->polish(ui->button_new);
    ui->button_new->update();

    ui->line->setProperty("style", style);
    ui->line->style()->polish(ui->line);
    ui->line->update();

    ui->listWidget->setProperty("style", style);
    ui->listWidget->style()->polish(ui->listWidget);
    //ui->listWidget->update();

    ui->title_bar->setProperties(style);

    config.insert("style", QString("%1").arg(style));
    updateConfig();
}

void MainWindow::increaseFont(){
    if (font_size < 35) {
        ++font_size;
        ui->listWidget->setFont(QFont("Arial", font_size));
        config.find(QString("font-size")).value() = font_size;
        updateConfig();
        emit signalFontSize(font_size);
    }
}

void MainWindow::decreaseFont(){
    if (font_size > 4) {
        --font_size;
        ui->listWidget->setFont(QFont("Arial", font_size));
        config.find(QString("font-size")).value() = font_size;
        updateConfig();
        emit signalFontSize(font_size);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton){
        if (event->pos().x() < 10 || event->pos().x() > width() - 10 ||
            event->pos().y() < 10 || event->pos().y() > height() - 10) {
            start_x = event->pos().x();
            start_y = event->pos().y();
            start_pos = event->globalPos();
            resizing = true;
        }
    }
}
bool MainWindow::eventFilter(QObject *obj, QEvent *e) {
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
void MainWindow::mouseMoveEvent(QMouseEvent *event) {

    if (resizing) {
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
void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    /* End of resizing */
    resizing = false;

    config.insert("width", width());
    config.insert("height", height());
    config.insert("x", pos().x());
    config.insert("y", pos().y());
    updateConfig();

}
