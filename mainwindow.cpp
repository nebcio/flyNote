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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init_connects();
    files();
    selected_files = new QVector<QListWidgetItem*>;
    children_window = new QVector<QWidget*>;
}

MainWindow::~MainWindow() {
    delete ui;
    delete saving_shortcut;
    delete children_window;
    delete selected_files;
    delete decrease_shortcut;
    delete increase_shortcut;
}

void MainWindow::init_connects() {
    saving_shortcut = new QShortcut(QKeySequence::Save, this, SLOT(save_opened()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(select_file(QListWidgetItem*)));
    connect(ui->button_open, SIGNAL(clicked()), this, SLOT(open_selected()));
    connect(ui->button_trash, SIGNAL(clicked()), this, SLOT(delete_selected()));
    connect(ui->button_close, SIGNAL(clicked()), this, SLOT(close_opened()));
    connect(ui->button_new, SIGNAL(clicked()), this, SLOT(create_new()));
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
        children_window->push_back(new flyNote());
    }
    else if (path != nullptr) {
        children_window->push_back(new flyNote(nullptr, *path));
    }

    children_window->back()->show();            // zabezpieczenie przed pustym by nie bylo undefined behavior
    connect(this, SIGNAL(signal_close()), children_window->back(), SLOT(close_note()));
    connect(this, SIGNAL(signal_save()), children_window->back(), SLOT(save_note()));

    connect(children_window->back(), SIGNAL(signal_create_new(QString*)), this, SLOT(create_new(QString*)));
    connect(children_window->back(), SIGNAL(signal_delete_me()), this, SLOT(update_data()));
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

void MainWindow::update_data() {

}
