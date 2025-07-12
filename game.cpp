#include "game.h"
#include "deathdialog.h"
#include <QDebug>
#include<ctime>
#include <QRandomGenerator>

Spaceship::Spaceship(const QPixmap& normal, const QPixmap& thrusting)
    : normalPixmap(normal), thrustingPixmap(thrusting) {
    setPixmap(normalPixmap);
    setTransformOriginPoint(boundingRect().center());
    this->setZValue(2);

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
    if(pos().x()<-800 or pos().y()<-800 or pos().x() > 1920 or pos().y()>1080){ // Странно, тут нужно писать -800 так как размер картинки 800
        //qDebug() << "dead";
        DeathDialog dialog("ПОТЕРЯН В КОСМОСЕ");
        GameScene obj;

        obj.setPaused(!obj.isPaused);
        if (dialog.exec() == QDialog::Accepted) {
            QCoreApplication::quit();
        }



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
    qreal speed = QRandomGenerator::global()->bounded(1, 5) * 0.1;

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

    QPixmap meteorPixmap("png-transparent-paper-asteroid-sticker-adhesive-planet-asteroid-child-sleep-slate-thumbnail.png"); // путь к изображению метеора
    meteor = new Meteor(meteorPixmap);
    meteor->setPos(width() / 2, height() / 2); // или любая другая начальная позиция
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
    case Qt::Key_Escape: setPaused(!isPaused); break;
    }
}

void GameScene::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
        spaceship->setThrusting(false);
    }
}

