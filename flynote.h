#ifndef FLYNOTE_H
#define FLYNOTE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class flyNote; }
QT_END_NAMESPACE

class flyNote : public QWidget
{
    Q_OBJECT
private:
    Ui::flyNote *ui;
    QString name;
    QString path;
    // shortcut
    void init_connects();
    bool moving=false;
    QPoint startMovePos;

public:
    explicit flyNote(QWidget *parent = nullptr, QString path=nullptr);
    ~flyNote();

signals:
    void signal_create_new(QString*);
    void signal_delete_me();
    void signal_update_list();

private slots:
    void load_note(); // _open
    void close_note();
    void fly_set();
    void delete_note();
    void create_new(); // potrzeba?
    void opacity_set(int a);
    void save_note();
    void ask_to_save();
    void save_and_close();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // FLYNOTE_H
