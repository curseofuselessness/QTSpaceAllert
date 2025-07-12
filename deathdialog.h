#ifndef DEATHDIALOG_H
#define DEATHDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QPixmap>

class DeathDialog : public QDialog
{
    Q_OBJECT
public:

    explicit DeathDialog(QString Death_why, QWidget *parent = nullptr) : QDialog(parent)
    {
        // Настройки окна
        setWindowFlags(Qt::FramelessWindowHint);
        setFixedSize(600, 400);

        // Создаем и настраиваем элементы
        QLabel *titleLabel = new QLabel(Death_why, this);
        QLabel *imageLabel = new QLabel(this);
        QPushButton *exitButton = new QPushButton("ВЫЙТИ", this);

        // Загружаем картинку (замените путь на свой)
        QPixmap spaceImage("deathMenu_logo.png");
        if(!spaceImage.isNull()) {
            imageLabel->setPixmap(spaceImage.scaled(300, 200, Qt::KeepAspectRatio));
            imageLabel->setAlignment(Qt::AlignCenter);
        }

        // Стилизация
        setStyleSheet(
            "QDialog {"
            "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0f2027, stop:1 #2c5364);"
            "   border: 2px solid #00b4ff;"
            "   border-radius: 15px;"
            "}"

            "QLabel {"
            "   color: #ffffff;"
            "   font: bold 28px;"
            "   padding: 10px;"
            "}"

            "QLabel#title {"
            "   color: #ff5555;"
            "   font: bold 36px;"
            "   text-shadow: 2px 2px 4px #000000;"
            "}"

            "QPushButton {"
            "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ff416c, stop:1 #ff4b2b);"
            "   color: #ffffff;"
            "   border: none;"
            "   border-radius: 10px;"
            "   padding: 15px 30px;"
            "   font: bold 18px;"
            "   min-width: 150px;"
            "}"

            "QPushButton:hover {"
            "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ff4b2b, stop:1 #ff416c);"
            "}"
            );

        titleLabel->setObjectName("title");
        titleLabel->setAlignment(Qt::AlignCenter);

        exitButton->setCursor(Qt::PointingHandCursor);

        // Компоновка
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addStretch();
        layout->addWidget(titleLabel);
        layout->addWidget(imageLabel, 0, Qt::AlignCenter);
        layout->addStretch();
        layout->addWidget(exitButton, 0, Qt::AlignCenter);
        layout->addStretch();
        layout->setSpacing(20);

        // Подключение кнопки
        connect(exitButton, &QPushButton::clicked, this, &QDialog::accept);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);

        // Рисуем градиентный фон
        QLinearGradient gradient(0, 0, width(), height());
        gradient.setColorAt(0, QColor(15, 32, 39));  // #0f2027
        gradient.setColorAt(1, QColor(44, 83, 100)); // #2c5364
        painter.fillRect(rect(), gradient);

        // Рисуем звезды (опционально)
        painter.setPen(Qt::white);
        for(int i = 0; i < 50; ++i) {
            painter.drawPoint(rand() % width(), rand() % height());
        }

        QDialog::paintEvent(event);
    }
};

#endif // DEATHDIALOG_H
