#include "Health.h"
#include "Parameter.h"
#include <QFont>

Health::Health(QGraphicsItem *parent,int pos_y): QGraphicsTextItem(parent){
    setPos(HEALTH_POSITION_X,HEALTH_POSITION_Y);
    //setZValue(TEXT_ITEM_ORDER);
    setPlainText(QString("Health: ") + QString::number(health));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",16));
}


void Health::decrease(int value){
    health -= value;
    setPlainText(QString("Health: ") + QString::number(health));
}

int Health::getHealth(){
    return health;
}

