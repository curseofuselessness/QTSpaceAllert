#include "game.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GameScene scene;
    QGraphicsView view;
   // view.setFixedSize(1920, 1080);
  //  view.resize(1920, 1080);
    //view.resize(1920, 1080);

    QRectF r = qGuiApp->primaryScreen()->availableGeometry();
    view.setFixedSize(r.width(), r.height());
    scene.setSceneRect(0, 0, view.width(), view.height());
    view.setScene(&scene);

   // QRectF rect = view.sceneRect();
   // qDebug() << "Scene rect:" << r;

    view.setRenderHint(QPainter::Antialiasing);
    view.setWindowFlags(Qt::FramelessWindowHint);
    view.showFullScreen();
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Убрать горизонтальный скролл
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Убрать вертикальный скролл
    view.setBackgroundBrush(Qt::black);
    view.show();

    return a.exec();
}
