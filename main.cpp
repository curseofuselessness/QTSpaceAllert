#include "game.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GameScene scene;
    QGraphicsView view;

    view.resize(1920, 1080);

    view.setScene(&scene);
    //scene.setSceneRect(0, 0, 1920, 1080);
    view.setSceneRect(0, 0, 1920, 1080);
    view.setRenderHint(QPainter::Antialiasing);
    view.setWindowFlags(Qt::FramelessWindowHint);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Убрать горизонтальный скролл
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Убрать вертикальный скролл
    view.setBackgroundBrush(Qt::black);
    view.show();

    return a.exec();
}
