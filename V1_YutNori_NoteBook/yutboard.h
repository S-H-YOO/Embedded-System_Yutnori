#ifndef YUTBOARD_H
#define YUTBOARD_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>

class yutboard : public QWidget
{
    Q_OBJECT

public:
    explicit yutboard(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPoint boardPoints[50]; // 충분히 크게 잡아둠
};

#endif // YUTBOARD_H
