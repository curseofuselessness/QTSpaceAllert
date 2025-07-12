#include "game.h"
#include <QDebug>
#include<ctime>
#include <QRandomGenerator>

Spaceship::Spaceship(const QPixmap& normal, const QPixmap& thrusting)
    : normalPixmap(normal), thrustingPixmap(thrusting) {
    setPixmap(normalPixmap);
    setTransformOriginPoint(boundingRect().center());
    this->setZValue(2);

}

void Spaceship::advance(int phase) {
    if (!phase) return;

    setRotation(rotation);

    if (thrusting) {
        setPixmap(thrustingPixmap);
        qreal angle = qDegreesToRadians(rotation);
        velocity.rx() += qSin(angle) * thrust;
        velocity.ry() += -qCos(angle) * thrust;
    } else {
        setPixmap(normalPixmap);
    }

    velocity *= friction;
    setPos(pos() + velocity);
    if(pos().x()<0 or pos().y()<0 or pos().x() > 1920 or pos().y()>1080){
        //qDebug() << "dead";

    }
    qreal speed = qSqrt(velocity.x()*velocity.x() + velocity.y()*velocity.y());
    if (speed > 10) {
        velocity = velocity / speed * 10;
    }
}

Meteor::Meteor(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap) {
    setTransformOriginPoint(boundingRect().center());

    // Устанавливаем случайное направление и скорость
    qreal angle = QRandomGenerator::global()->bounded(360);
    qreal speed = QRandomGenerator::global()->bounded(5, 10);

    velocity = QPointF(speed * cos(angle * M_PI / 180),
    speed * sin(angle * M_PI / 180));

    sceneRect = QRectF(0, 0, 1920, 1080);
    setRotation(QRandomGenerator::global()->bounded(360));

}

void Meteor::checkBounds() {
    QPointF newPos = pos();
    qreal width = boundingRect().width();
    qreal height = boundingRect().height();
    bool changedDirection = false;

    // Проверка правой границы
    if (newPos.x() > sceneRect.right() - width/2) {
        newPos.setX(sceneRect.right() - width/2);
        velocity.setX(-velocity.x());  // Отражаем по X
        changedDirection = true;
    }
    // Проверка левой границы
    else if (newPos.x() < sceneRect.left() - width/2) {
        newPos.setX(sceneRect.left() - width/2);
        velocity.setX(-velocity.x());  // Отражаем по X
        changedDirection = true;
    }

    // Проверка нижней границы
    if (newPos.y() > sceneRect.bottom() - height/2) {
        newPos.setY(sceneRect.bottom() - height/2);
        velocity.setY(-velocity.y());  // Отражаем по Y
        changedDirection = true;
    }
    // Проверка верхней границы
    else if (newPos.y() < sceneRect.top() - height/2) {
        newPos.setY(sceneRect.top() - height/2);
        velocity.setY(-velocity.y());  // Отражаем по Y
        changedDirection = true;
    }

    if (changedDirection) {
        //Случайное изменение направления
        qreal angleVariation = QRandomGenerator::global()->bounded(30) - 15; // ±15 градусов
        qreal newAngle = atan2(velocity.y(), velocity.x()) + angleVariation * M_PI/180;
        qreal speed = sqrt(velocity.x()*velocity.x() + velocity.y()*velocity.y());
        velocity = QPointF(cos(newAngle) * speed, sin(newAngle) * speed);
    }

    setPos(newPos);
}

void Meteor::advance(int phase) {
    if (!phase) return;

    setPos(pos() + velocity);
    setRotation(rotation() + 0.5);

    checkBounds();

}

void Spaceship::setThrusting(bool thrusting) { this->thrusting = thrusting; }
void Spaceship::rotateLeft() {

   rotation -= rotationSpeed;

}
void Spaceship::rotateRight() {
     rotation += rotationSpeed;
}

GameScene::GameScene(QObject* parent) : QGraphicsScene(parent) {
    // Загрузка изображений
    QPixmap normalShip("spaceship_normal.png");
    QPixmap thrustingShip("spaceship_thrusting.png");

    if (normalShip.isNull() || thrustingShip.isNull()) {
        qDebug() << "Failed to load ship images!";
        return;
    }

    spaceship = new Spaceship(normalShip, thrustingShip);
    addItem(spaceship);
    spaceship->setPos(960, 540);

    QPixmap meteorPixmap("meteor.png");
    meteor = new Meteor(meteorPixmap);
    meteor->setPos(860, 540);
    addItem(meteor);

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
}


void GameScene::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Left: spaceship->rotateLeft(); break;
    case Qt::Key_Right: spaceship->rotateRight(); break;
    case Qt::Key_Space: spaceship->setThrusting(true); break;
    case Qt::Key_1: spaceship->setThrusting(true); break;
    case Qt::Key_Escape: setPaused(!isPaused); break;
    }
}

void GameScene::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
        spaceship->setThrusting(false);
    }
}

void GameScene::setPaused(bool paused) {
    isPaused = paused;
    if (paused) {
        // Останавливаем все таймеры
        foreach (QTimer *timer, findChildren<QTimer*>()) {
            timer->stop();
        }
    } else {
        // Возобновляем таймеры
        foreach (QTimer *timer, findChildren<QTimer*>()) {
            timer->start(16);
        }
    }
}

void GameScene::gameOver(){
    setPaused(true);

}

