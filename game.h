#ifndef PAINT_WINDOW_H
#define PAINT_WINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QPropertyAnimation>


class Spaceship : public QGraphicsPixmapItem {
public:
    Spaceship(const QPixmap& normal, const QPixmap& thrusting);
    void advance(int phase) override;
    void setThrusting(bool thrusting);
    void rotateLeft();
    void rotateRight();

private:

    QPixmap normalPixmap;
    QPixmap thrustingPixmap;
    QPointF velocity;
    qreal rotation = 0;
    qreal thrust = 0.2;
    qreal targetRotation = 0;
    qreal rotationAccuracy = 0.001;
    qreal rotationSpeed = 15;
    qreal friction = 0.999;
    bool thrusting = false;
};

class Meteor : public QGraphicsPixmapItem {
public:
    Meteor(const QPixmap& pixmap);
    void advance(int phase) override;

private:
    QPointF velocity;
    qreal friction = 0.999;
};

class GameScene : public QGraphicsScene {
public:
    GameScene(QObject* parent = nullptr);
    bool isPaused = false;
    void setPaused(bool);
    void gameOver();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:


    Spaceship* spaceship;
    Meteor* meteor; // Добавлен метеор
};

#endif // PAINT_WINDOW_H
