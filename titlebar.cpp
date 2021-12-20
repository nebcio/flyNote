#include "titlebar.h"
#include <QFile>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QSizePolicy>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>


TitleBar::TitleBar(MainWindow *parent) : m_parent(parent) {
    initIntefaces();
    initConnects();
}

TitleBar::~TitleBar(){}

void TitleBar::initIntefaces(){
    m_title = new QLabel("flyNote", this);
    m_title->setProperty("style", "blue");
    m_title->setProperty("type", "title");

    m_exit_button = new QPushButton(this);{
    m_exit_button->setObjectName("exit_button");
    m_exit_button->setFixedSize(QSize(30, 30));
    m_exit_button->setIcon(QIcon("./img/close.png"));
    m_exit_button->setIconSize(QSize(10, 10));
    m_exit_button->setProperty("style", "blue");
    m_exit_button->setToolTip("Exit");}

    m_min_button = new QPushButton(this); {
    m_min_button->setObjectName("min_button");
    m_min_button->setFixedSize(QSize(30, 30));
    m_min_button->setText("_");
    m_min_button->setProperty("style", "blue");}

    layout_h = new QHBoxLayout(this); {
    layout_h->addWidget(m_title);
    layout_h->addWidget(m_min_button);
    layout_h->addWidget(m_exit_button);
    layout_h->insertStretch(1, 500); // co to xDDD
    layout_h->setSpacing(0);
    layout_h->setContentsMargins(0,0,0,0);}

    QFontDatabase::addApplicationFont("C:/Users/zuzaw/Projekty/flyNote/style/domestic-manners.regular.ttf");
    m_title->setFont(QFont("Domestic Manners"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAutoFillBackground(true);
}

void TitleBar::initConnects() {
    connect(m_exit_button, &QPushButton::clicked, this, &TitleBar::onButtonExitClicked);
    connect(m_min_button, &QPushButton::clicked, this, &TitleBar::onButtonMinClicked);
    connect(this, SIGNAL(signalUpdateConfig()), m_parent, SLOT(updateConfig()));
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

void TitleBar::onButtonExitClicked() {
    /* Trigger exit */
    emit signalExit();
}
void TitleBar::onButtonMinClicked() {
    m_parent->showMinimized();
}

void TitleBar::setProperties(QString& style) {
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
