#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QVector>
#include <QWidget>
#include <QShortcut>
#include <QObject>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    QVector<QWidget> *children_window;
    QVector<QListWidgetItem> *selected_files;
    QShortcut *saving_shortcut;
    QShortcut *increase_shortcut;
    QShortcut *decrease_shortcut;
    QJsonObject names_files;    // files ?

    Ui::MainWindow *ui;
    void init_connects();
    void update_data(); // remove closed
    QString* get_key(QString name);

private slots:
    void files();
    void select_file(QListWidgetItem *item);
    void open_selected();
    void delete_selected();
    void create_new(QString *path=nullptr);
    void save_opened();
    void close_opened();

signals:
    void signal_close();
    void signal_save();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
