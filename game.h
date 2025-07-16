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

class Starting_menu : public QGraphicsPixmapItem {
public:
    Starting_menu(const QPixmap& pxmp);

private:

    QPixmap Starting_menu_pic;

};
class Spaceship : public QGraphicsPixmapItem {
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    Spaceship(const QPixmap& normal, const QPixmap& thrusting, const QPixmap& normalDamaged, const QPixmap& thrustingDamaged);
    void advance(int phase) override;
    void setThrusting(bool thrusting);
    void rotateLeft();
    void rotateRight();
    QPainterPath shape() const override;

    QPixmap normalPixmapDamaged;
    QPixmap thrustingPixmapDamaged;
    QPixmap normalPixmap;
    QPixmap thrustingPixmap;
    QPointF velocity;
    qreal HP = 100;
    qreal rotation = 0;
    qreal thrust = 0.2;
    qreal targetRotation = 0;
    qreal rotationAccuracy = 0.001;
    qreal rotationSpeed = 15;
    qreal friction = 0.999;
    bool thrusting = false;
};
class Blackhole : public QGraphicsPixmapItem {
public:
   // void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    Blackhole(const QPixmap& blackholepic);
    void advance(int phase) override;

   // QPainterPath shape() const override;

private:
    QPixmap blackholepic;
};
class Meteor : public QGraphicsPixmapItem {
public:

    Meteor(const QPixmap& pixmap);
    void advance(int phase) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QPainterPath shape() const override;
    QPointF velocity;
private:

    qreal friction = 0.999;
};

class GameScene : public QGraphicsScene {
public:
    GameScene(QObject* parent = nullptr);
    bool isPaused = false;
    void setPaused(bool);
    void gameOver();
    void StatusMeteorRain(bool flag);
    void StatusBlackHole(bool flag);
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    bool isBlackHole;
    bool isMeteorRain;
    QTimer* meteorTimer;
    Starting_menu* menu_obj;
    Spaceship* spaceship;
    Blackhole* blackhole;
    Meteor* meteor; // Добавлен метеор
};

#endif // PAINT_WINDOW_H
