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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init_connects();
    files();
    selected_files = new QVector<QListWidgetItem*>;
    children_window = new QVector<flyNote*>; // poczytaj
}

MainWindow::~MainWindow() {
    delete saving_shortcut;
    delete children_window;
    delete selected_files;
    delete decrease_shortcut;
    delete increase_shortcut;
    delete ui;

}

void MainWindow::init_connects() {
    saving_shortcut = new QShortcut(QKeySequence::Save, this, SLOT(save_opened()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(select_file(QListWidgetItem*)));
    connect(ui->button_open, SIGNAL(clicked()), this, SLOT(open_selected()));
    connect(ui->button_trash, SIGNAL(clicked()), this, SLOT(delete_selected()));
    connect(ui->button_close, SIGNAL(clicked()), this, SLOT(close_opened()));
    connect(ui->button_new, SIGNAL(clicked()), this, SLOT(create_new()));
    connect(ui->button_style, SIGNAL(clicked()), this, SLOT(switch_style()));
    connect(this, SIGNAL(signal_style(QString)), ui->title_bar, SLOT(switch_style(QString)));
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
        ui->listWidget->addItem(item);
    }
}

void MainWindow::select_file(QListWidgetItem *item){
    /* Managing selection items from list */
    if (selected_files->empty()) {
        selected_files->push_back(item);
        item->setSelected(true);
    }
    else {
        int repeated;
        bool if_it_is_in_list = false;
        for (int i = 0; i < selected_files->size(); i++) {      // przechodzi przez wybrane i sprawdza czy aktualnie klikniety sie nie powtarza
            if (item->text() == selected_files->at(i)->text()) { // daj do osobnej funkcji !!!!
                if_it_is_in_list = true;
                repeated = i;
            }
        }

        if (if_it_is_in_list){
            selected_files->remove(repeated);
            item->setSelected(false);
        }
        else {
            selected_files->push_back(item);
            item->setSelected(true);
        }
    }
}

void MainWindow::open_selected() {
    /* Open selected notes */
    for (QListWidgetItem *item : *selected_files) {
        int i = 0;
        while (item->text() != names_files.value(names_files.keys().at(i)).toString()){
            i++;
        }
        QString tmp_path = names_files.keys().at(i);
        QString *path = &tmp_path;
        create_new(path);
        item->setSelected(false);
    }
    selected_files->clear();
}

void MainWindow::delete_selected() {
    /* Delete selected notes */
    for (QListWidgetItem *item : *selected_files){
        QString name = item->text();
        int i = 0;
        while (name == names_files.value(names_files.keys().at(i)).toString()){
            i++;
        }
        QString tmp_path = names_files.keys().at(i);
        QString *path = &tmp_path;
        QFile file_item(*path);

        if (file_item.exists()) {
            file_item.remove();
        }

        if (names_files.keys().contains(*path)){
            names_files.remove(*path);
        }

        item->setSelected(false);
    }
    selected_files->clear();

    QFile flynotes("flynotes.json"); // podobno lepiej QFileInfo, ale to na koniec
    if (flynotes.exists()){
        flynotes.open(QIODevice::WriteOnly | QIODevice::Text);
        flynotes.write(QJsonDocument(names_files).toJson());
        flynotes.close();
    }

    files();
}

void MainWindow::create_new(QString *path) {
    if (path == nullptr) {
        children_window->push_back(new flyNote(nullptr, nullptr, style));
    }
    else if (path != nullptr) {
        children_window->push_back(new flyNote(nullptr, *path, style));
    }

    children_window->back()->show();            // zabezpieczenie przed pustym by nie bylo undefined behavior
    connect(this, SIGNAL(signal_close()), children_window->back(), SLOT(close_note()));
    connect(this, SIGNAL(signal_save()), children_window->back(), SLOT(save_note()));
    connect(this, SIGNAL(signal_style(QString)), children_window->back(), SLOT(switch_style(QString)));

    connect(children_window->back(), SIGNAL(signal_create_new(QString*)), this, SLOT(create_new(QString*)));
    connect(children_window->back(), SIGNAL(signal_delete_me(QString)), this, SLOT(clean_data(QString)));
    connect(children_window->back(), SIGNAL(signal_update_list()), this, SLOT(files()));
}

void MainWindow::save_opened() {
    /* Save opened notes */
    emit signal_save();
}

void MainWindow::close_opened() {
    /* Close opened notes */
    emit signal_close();
}

void MainWindow::clean_data(QString name){
    for (int i = 0; i < children_window->size(); i++){
        if (children_window->at(i)->get_name() == name){
            children_window->erase(children_window->begin()+i);
        }
    }

}

void MainWindow::update_data() {
    // trzeba rozdzielić update zamknietych i usunietych/zapisanych

}

void MainWindow::switch_style() {
    if (style == "blue") { style = "red"; }
    else { style = "blue"; }

    emit signal_style(style);

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
    ui->listWidget->update();

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
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
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
    else { setCursor(Qt::ArrowCursor);}

    if (resizing) {
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
                qDebug() << "here";
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
void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    resizing = false;
}
