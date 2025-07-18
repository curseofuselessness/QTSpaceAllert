#ifndef EXPLOSION_H
#define EXPLOSION_H
#include<QGraphicsObject>
#include<QGraphicsScene>
#include<QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QPainter>



class AnimatedExplosion : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius)
public:
    AnimatedExplosion(const QPointF& center, QGraphicsItem* parent = nullptr)
        : QGraphicsObject(parent), m_center(center), m_radius(0) {

        // Анимация расширения
        QPropertyAnimation* expand = new QPropertyAnimation(this, "radius");
        expand->setDuration(300);
        expand->setStartValue(0);
        expand->setEndValue(300);
        expand->setEasingCurve(QEasingCurve::OutQuad);

        // Анимация затухания
        QPropertyAnimation* fade = new QPropertyAnimation(this, "opacity");
        fade->setDuration(200);
        fade->setStartValue(1.0);
        fade->setEndValue(0.0);

        // Групповая анимация
        QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
        group->addAnimation(expand);
        group->addAnimation(fade);

        connect(group, &QAbstractAnimation::finished, this, [this]() {
            scene()->removeItem(this);
            deleteLater();
        });

        group->start();
    }

    qreal radius() const { return m_radius; }
    void setRadius(qreal r) {
        m_radius = r;
        update();
    }

    QRectF boundingRect() const override {
        return QRectF(-m_radius, -m_radius, 2*m_radius, 2*m_radius)
        .translated(m_center);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        QRadialGradient grad(m_center, m_radius);
        grad.setColorAt(0, QColor(255, 200, 0, 200));
        grad.setColorAt(0.7, QColor(255, 100, 0, 150));
        grad.setColorAt(1, Qt::transparent);

        painter->setBrush(grad);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(m_center, m_radius, m_radius);

        // Вспышка в центре
        painter->setBrush(QColor(255, 255, 200, 150));
        painter->drawEllipse(m_center, m_radius/4, m_radius/4);
    }

private:
    QPointF m_center;
    qreal m_radius;
};


#endif // EXPLOSION_H
