#include "game.h"
#include "deathdialog.h"
#include "explosion.h"
#include <QDebug>
#include<ctime>
#include <QRandomGenerator>
#include <QtMath>




bool menu_skiped = false;
bool dead = false;
bool first_start = true;

Starting_menu::Starting_menu(const QPixmap &pxmp): Starting_menu_pic(pxmp)
    {
    setPixmap(Starting_menu_pic);
        this->setZValue(10);
    }
Spaceship::Spaceship(const QPixmap& normal, const QPixmap& thrusting, const QPixmap& normalDamaged, const QPixmap& thrustingDamaged)
    : normalPixmapDamaged(normalDamaged), thrustingPixmapDamaged(thrustingDamaged), normalPixmap(normal), thrustingPixmap(thrusting)  {
    if(HP > 40)setPixmap(normalPixmap);
    else setPixmap(normalPixmapDamaged);

    setTransformOriginPoint(boundingRect().center());
    this->setZValue(2);

}
void Spaceship::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);
    /*
    // Для отладки - рисуем хитбокс
    QPen *pen = new QPen(Qt::blue);
    pen->setWidth(5);
    painter->setPen(*pen);
    painter->drawPath(shape());
*/
}
void Blackhole::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QGraphicsPixmapItem::paint(painter, option, widget);
    setZValue(4);
    /*
QPen *pen = new QPen(Qt::blue);

    pen->setWidth(5);
    painter->setPen(*pen);
    painter->drawPath(shape());
*/
}

Blackhole::Blackhole(const QPixmap &blackholepic) : QGraphicsPixmapItem(blackholepic)
{
    setTransformOriginPoint(boundingRect().center());

}
void Blackhole::advance(int phase) {
    if (!phase) return;

    // Черная дыра медленно вращается
    setRotation(rotation() + 3);
    QList<QGraphicsItem*> items = scene()->items();
    foreach(QGraphicsItem* item, items) {
        if (Spaceship* ship = dynamic_cast<Spaceship*>(item)) {
            qreal dx = ship->pos().x()- pos().x();
            qreal dy = ship->pos().y()- pos().y();
            qreal distance = qSqrt(dx*dx + dy*dy);
            QPointF blackhole_velocity(-1*dx, -1*dy);
            ship->velocity = ship->velocity +  30 / (distance * distance) * blackhole_velocity;
            if (collidesWithItem(ship) and dead == false) {
                DeathDialog dialog("ЗАСОСАЛО В ЧЕРНУЮ ДЫРУ");
                GameScene obj;
                dead = true;

                obj.setPaused(true);
                if (dialog.exec() == QDialog::Accepted) {
                    QCoreApplication::quit();
                }
                QCoreApplication::quit();
                return;
            }
        }

    }


}

QPainterPath Blackhole::shape() const
{

    QPainterPath path;

    path.addEllipse(boundingRect().center(), 30, 30); // Или более точная форма вашего корабля
    return path;

}


QPainterPath Spaceship::shape() const
{
    QPainterPath path;
    QPolygonF polygon;
    polygon << QPointF(100, 10)   // Добавляем точки последовательно
            << QPointF(170, 170)
            << QPointF(30, 170)
            << QPointF(100, 0)  ;

    path.addPolygon(polygon);
    //path.addEllipse(boundingRect()); // Или более точная форма вашего корабля
    return path;
}
void GameScene::setPaused(bool paused) {
    isPaused = paused;
    if (paused) {

        foreach (QTimer *timer, findChildren<QTimer*>()) {
            timer->stop();

        }
    } else {
        // Возобновляем таймеры
        foreach (QTimer *timer, findChildren<QTimer*>()) {

            timer->start(16);
            if(timer == event_timer){
                event_timer->start(10000);
            }
                /*
            if(timer == event_timer and !first_start)event_timer->start(30000);
            if(timer == event_timer and first_start){
                first_start = false;
                event_timer->start();
                event_timer->stop();
                //event_timer->start(30000);

            }
*/
            if(timer == meteorTimer) meteorTimer->start(meteordelay);
            if(timer == blackholeTimer and event == 2)
            {
                blackholeTimer->start(blackholedelay);

            }


                //
            //

        }
    }
}

void GameScene::gameOver(){
    setPaused(true);
}


void Spaceship::advance(int phase) {
    if (!phase) return;

    setRotation(rotation);

    if (thrusting) {
        if(HP > 40 )setPixmap(thrustingPixmap);
        else setPixmap(thrustingPixmapDamaged);
        qreal angle = qDegreesToRadians(rotation);
        velocity.rx() += qSin(angle) * thrust;
        velocity.ry() += -qCos(angle) * thrust;
    } else {
        if(HP > 40 )setPixmap(normalPixmap);
        else setPixmap(normalPixmapDamaged);
    }

    velocity *= friction;
    setPos(pos() + velocity);

    // столкновение с метеором

    QList<QGraphicsItem*> items = scene()->items();
    foreach(QGraphicsItem* item, items) {
        if (Meteor* meteor = dynamic_cast<Meteor*>(item)) {
            if (collidesWithItem(meteor)) {
                QPointF vel = meteor->velocity;
                bool deleted = 0;
                if(qAbs(velocity.x())<2 ) {
                    velocity.rx() = vel.x() + 5;
                }
                else
                {
                    velocity.rx() = velocity.x()-vel.x() + 1;
                    velocity.rx() *= -0.6;
                    qreal speed = qSqrt(velocity.x()*velocity.x() + velocity.y()*velocity.y());
                    if(speed > 6)
                    {

                        deleted = true;
                        delete meteor;
                    }
                }

                if(qAbs(velocity.y())<2) {
                    velocity.ry() = vel.y() + 5;
                }
                else
                {
                    velocity.ry() = velocity.y()-vel.y() + 1;
                    velocity.ry() *= -0.6;
                    qreal speed = qSqrt(velocity.x()*velocity.x() + velocity.y()*velocity.y());
                    if(speed > 6 and deleted == false) {
                    delete meteor;
                    }
                    if(speed * 7 > 15) HP = HP - speed * 7;
                }
                deleted = false;


                 }
            if(HP<=0 and dead == false){
                     // Использование:
                     AnimatedExplosion* explosion = new AnimatedExplosion(this->pos());
                     //explosion->setRadius(200);
                     scene()->addItem(explosion);
                     delete this;
                DeathDialog dialog("СТОЛКНОВЕНИЕ С МЕТЕОРОМ");
                GameScene obj;
                dead = true;
              //  obj.setPaused(true);
                if (dialog.exec() == QDialog::Accepted) {
                    QCoreApplication::quit();
                }
                QCoreApplication::quit();
                return;
            }

        }

    }
    if((pos().x()<-800 or pos().y()<-800 or pos().x() > 1920 or pos().y()>1080) and dead == false){ // Странно, тут нужно писать -800 так как размер картинки 800
        //qDebug() << "dead";
        DeathDialog dialog("ПОТЕРЯН В КОСМОСЕ");
        GameScene obj;
        dead = true;
        obj.setPaused(!obj.isPaused);
        if (dialog.exec() == QDialog::Accepted) {
            QCoreApplication::quit();
        }
        QCoreApplication::quit();



    }
    qreal speed = qSqrt(velocity.x()*velocity.x() + velocity.y()*velocity.y());
    if (speed > 10) {
        velocity = velocity / speed * 10;
    }
}

Meteor::Meteor(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap) {
    setTransformOriginPoint(boundingRect().center());
    setZValue(3);

    // Устанавливаем случайное направление и скорость
    qreal angle = QRandomGenerator::global()->bounded(360);
    qreal speed = QRandomGenerator::global()->bounded(30, 60) * 0.1;

    velocity = QPointF(speed * cos(angle * M_PI / 180),
    speed * sin(angle * M_PI / 180));

    // Устанавливаем случайный угол поворота
    setRotation(QRandomGenerator::global()->bounded(360));
}


void Meteor::advance(int phase) {
    if (!phase) return;


    velocity *= friction;
    setPos(pos() + velocity);
    // Метеор медленно вращается
    setRotation(rotation() + 0.5);
    if(pos().x()<-1000 or pos().y()<-1000 or pos().x() > 2320 or pos().y()>1780){
        scene()->removeItem(this);
        delete this;
    }

}

void Meteor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QGraphicsPixmapItem::paint(painter, option, widget);
/*
    // Для отладки - рисуем хитбокс
    QPen *pen = new QPen(Qt::blue);
    pen->setWidth(5);
    painter->setPen(*pen);
    painter->drawPath(shape());
*/
}

QPainterPath Meteor::shape() const
{
    QPainterPath path;

    path.addEllipse(boundingRect());
    return path;
}

void Spaceship::setThrusting(bool thrusting) { this->thrusting = thrusting; }
void Spaceship::rotateLeft() {
   rotation -= rotationSpeed;

}
void Spaceship::rotateRight() {
     rotation += rotationSpeed;
}

void GameScene::StatusBlackHole(int delay){
  QPixmap blackholepic("blackhole.png");


        blackhole = new Blackhole(blackholepic);
      blackhole->setZValue(4);
        qreal x =  qAbs(spaceship->pos().x() - 960);
      qreal y = qAbs(spaceship->pos().y() - 540);

        blackhole->setPos(x, y);
        addItem(blackhole);
        QTimer* blackholeTimer = new QTimer(this);
        blackholeTimer->setSingleShot(true);
        blackholeTimer->start(blackholedelay);

        connect(blackholeTimer, &QTimer::timeout, this, [this]() {
            delete blackhole;
        });


}

void GameScene::StatusMeteorRain(int kolvo, int delay)
{

    QPixmap meteorPixmap("meteor.png"); // путь к изображению метеора
    meteorTimer = new QTimer;
    int temp_kolvo = kolvo;

        connect(meteorTimer, &QTimer::timeout, this, [this, meteorPixmap, temp_kolvo]() {
            int pattern = 1;
            static int c = 0;
            pattern = QRandomGenerator::global()->bounded(1, 5);
            meteor = new Meteor(meteorPixmap);
            qreal x = 0;
            qreal y = 0;
            switch(pattern) { //стороны
            case 1: //левая
                x = -200;
                y = QRandomGenerator::global()->bounded(200, 1000);
                if(meteor->velocity.x()<0) meteor->velocity.rx()*=-1;
                break;
            case 2: // правая
                x = 2120;
                if(meteor->velocity.x()>0) meteor->velocity.rx()*=-1;
                y = QRandomGenerator::global()->bounded(200, 900);
                break;
            case 3: // верхняя
                if(meteor->velocity.y()<0) meteor->velocity.ry()*=-1;
                x = QRandomGenerator::global()->bounded(200, 1900);
                y = -200;
                break;
            case 4: //нижняя
                if(meteor->velocity.y()>0) meteor->velocity.ry()*=-1;
                x = QRandomGenerator::global()->bounded(100, 1900);
                y = 1280;
                break;


            }
            c++;
            qDebug() << c << " " << temp_kolvo;
            if(c>temp_kolvo){
                c=0;
                 meteorTimer->stop();
            }
            meteor->setPos(x, y);
            addItem(meteor);


        });

        meteorTimer->start(delay);


}


GameScene::GameScene(QObject* parent) : QGraphicsScene(parent) {
    // Загрузка изображений
    QPixmap starting_menu_pic("menu_Start-1.png");
    menu_obj = new Starting_menu(starting_menu_pic);

    QPixmap normalShip("spaceship_normal.png");
    QPixmap thrustingShip("spaceship_thrusting.png");
    QPixmap normalShipDamaged("spaceship_normal_damaged.png");
    QPixmap thrustingShipDamaged("spaceship_thrusting_damaged.png");

    addItem(menu_obj);
    //removeItem();
    if (normalShip.isNull() || thrustingShip.isNull()) {
        qDebug() << "Failed to load ship images!";
        return;
    }

    spaceship = new Spaceship(normalShip, thrustingShip, normalShipDamaged, thrustingShipDamaged);
    addItem(spaceship);
    spaceship->setPos(960, 540);

   //StatusMeteorRain(false);
   event_timer = new QTimer(this);
   connect(event_timer, &QTimer::timeout, this, [this]() {
     event = QRandomGenerator::global()->bounded(1, 3);
       qDebug() << event;
       switch(event){
       case 1:
           meteordelay = 500;
           StatusMeteorRain(20, meteordelay);
           break;
       case 2:
           meteordelay = 1000;
            StatusMeteorRain(10, meteordelay);
           blackholedelay = 10000;
           StatusBlackHole(blackholedelay);
           blackhole->setZValue(5);
           break;
       }
   });
  // event_timer->start(10000);








    // Создаем звездное небо
    srand(time(0));
    for (int i = 0; i < 1000; ++i) {
        QGraphicsEllipseItem *star = new QGraphicsEllipseItem(0, 0, 2, 2);
        star->setBrush(Qt::white);
        star->setPos(rand() % 1920, rand() % 1080);
        addItem(star);
    }

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameScene::advance);
    timer->start(16);
    setPaused(true);
}

void GameScene::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Any and menu_skiped != true) {
        removeItem(menu_obj);
        setPaused(false);
        menu_skiped = true;
        return;
    }
    switch (event->key()) {
    case Qt::Key_Left: spaceship->rotateLeft(); break;
    case Qt::Key_Right: spaceship->rotateRight(); break;
    case Qt::Key_Space: spaceship->setThrusting(true); break;
    }
}

void GameScene::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
        spaceship->setThrusting(false);
    }
}




