#include "titlebar.h"
#include <QFile>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QSizePolicy>
#include <QDebug>


TitleBar::TitleBar(QWidget *parent) : m_parent(parent) {

    init_intefaces();
    init_connects();
    //connect(close, SIGNAL(clicked()), parent, SLOT(on_button_close_clicked()));
}

TitleBar::~TitleBar(){} // ?????? pointers

void TitleBar::init_intefaces(){
    // m_icon = new QLabel("^_");

    m_title = new QLabel("flyNote");
    m_title->setStyleSheet("color: #146b70; font-size: 30px;");

    m_exit_button = new QPushButton();
    m_exit_button->setObjectName("exit_button");
    m_exit_button->setFixedSize(QSize(30, 30));
    m_exit_button->setIcon(QIcon("./img/close.png"));
    m_exit_button->setIconSize(QSize(10, 10));

    m_min_button = new QPushButton();
    m_min_button->setObjectName("min_button");
    m_min_button->setFixedSize(QSize(30, 30));
    m_min_button->setText("_");

    layout_h = new QHBoxLayout(this);
    //layout_h->addWidget(m_icon);
    layout_h->addWidget(m_title);
    layout_h->addWidget(m_min_button);
    layout_h->addWidget(m_exit_button);
    layout_h->insertStretch(1, 500); // co to xDDD
    layout_h->setSpacing(0);

    QFontDatabase::addApplicationFont("C:/Users/zuzaw/Projekty/flyNote/style/domestic-manners.regular.ttf");
    m_title->setFont(QFont("Domestic Manners"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAutoFillBackground(true);
}

void TitleBar::init_connects() {
    connect(m_exit_button, SIGNAL(clicked()), this, SLOT(on_button_exit_clicked()));
    connect(m_min_button, SIGNAL(clicked()), this, SLOT(on_button_min_clicked()));

}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
        parent_moving = true;
        m_startMovePos = event->pos();
    }
}
void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    //if(parent_moving){    ogarnij czy to potrzebne !!!!!!!!
        m_parent->move(event->globalPos() - m_startMovePos);
    //}
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
