#include "titlebar.h"
#include <QFile>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QSizePolicy>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>


TitleBar::TitleBar(QWidget *parent) : m_parent(parent) {

    init_intefaces();
    init_connects();
    //connect(close, SIGNAL(clicked()), parent, SLOT(on_button_close_clicked()));
}

TitleBar::~TitleBar(){
    delete m_parent;
    delete m_title;
    delete m_exit_button;
    delete m_min_button;
    delete layout_h;
} // ?????? pointers

void TitleBar::init_intefaces(){
    m_title = new QLabel("flyNote");
    m_title->setProperty("style", "blue");

    m_exit_button = new QPushButton();{
    m_exit_button->setObjectName("exit_button");
    m_exit_button->setFixedSize(QSize(30, 30));
    m_exit_button->setIcon(QIcon("./img/close.png"));
    m_exit_button->setIconSize(QSize(10, 10));
    m_exit_button->setProperty("style", "blue");}

    m_min_button = new QPushButton(); {
    m_min_button->setObjectName("min_button");
    m_min_button->setFixedSize(QSize(30, 30));
    m_min_button->setText("_");
    m_min_button->setProperty("style", "blue");}

    layout_h = new QHBoxLayout(this); {
    layout_h->addWidget(m_title);
    layout_h->addWidget(m_min_button);
    layout_h->addWidget(m_exit_button);
    layout_h->insertStretch(1, 500); // co to xDDD
    layout_h->setSpacing(0);}

    QFontDatabase::addApplicationFont("C:/Users/zuzaw/Projekty/flyNote/style/domestic-manners.regular.ttf");
    m_title->setFont(QFont("Domestic Manners"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAutoFillBackground(true);
}

void TitleBar::init_connects() {
    connect(m_exit_button, SIGNAL(clicked()), this, SLOT(on_button_exit_clicked()));
    connect(m_min_button, SIGNAL(clicked()), this, SLOT(on_button_min_clicked()));

}

void TitleBar::switch_style(QString style) {
    m_title->setProperty("style", style);
    m_title->style()->polish(m_title);
    m_title->update();

    m_exit_button->setProperty("style", style);
    m_exit_button->style()->polish(m_exit_button);
    m_exit_button->update();

    m_min_button->setProperty("style", style);
    m_min_button->style()->polish(m_min_button);
    m_min_button->update();
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
        parent_moving = true;
        m_startMovePos = event->pos();
    }
}
void TitleBar::mouseMoveEvent(QMouseEvent *event) {   
    if (parent_moving) m_parent->move(event->globalPos() - m_startMovePos);

    /*if (parent_moving && m_parent->pos().x() > QGuiApplication::primaryScreen()->geometry().width() - m_parent->width()) {
        m_parent->move(QGuiApplication::primaryScreen()->geometry().width() - m_parent->width(), m_parent->pos().y());
    }
    else if (parent_moving && m_parent->pos().x() < 0){
        m_parent->move(0, m_parent->pos().y());
    }

    if (parent_moving && m_parent->pos().y() > QGuiApplication::primaryScreen()->geometry().height() - m_parent->height()) {
        m_parent->move(m_parent->pos().x(), QGuiApplication::primaryScreen()->geometry().height() - m_parent->height());
    }
    else if (parent_moving && m_parent->pos().y() < 0){
        m_parent->move(m_parent->pos().x(), 0);
    }*/
}
void TitleBar::mouseReleaseEvent(QMouseEvent *event) {
    parent_moving = false;
}

// moze lepiej?
void TitleBar::on_button_exit_clicked() {
    m_parent->close();
}
void TitleBar::on_button_min_clicked() {
    m_parent->showMinimized();
}

void TitleBar::saveRestoreInfo(const QPoint point, const QSize size){
    m_restorePos = point;
    m_restoreSize = size;
}
