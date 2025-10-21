#ifndef BEVELEDBUTTON_H
#define BEVELEDBUTTON_H

#include "qtmetamacros.h"
#include <QColor>
#include <QPalette>
#include <QPolygon>
#include <QPropertyAnimation>
#include <QPushButton>

enum BevelCorner { 
    TopLeft = 0x1, 
    TopRight = 0x2,
    BottomRight = 0x4,
    BottomLeft = 0x8
};
Q_DECLARE_FLAGS(BevelCorners, BevelCorner);
Q_DECLARE_OPERATORS_FOR_FLAGS (BevelCorners);

class BeveledButton : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(QColor color READ getColor WRITE setColor);
  Q_PROPERTY(int bevelSize READ getBevelSize WRITE setBevelSize);
  Q_PROPERTY(BevelCorners bevelCorners READ getBevelCorners WRITE setBevelCorners);

public:
  BeveledButton(QWidget *parent = nullptr);
  ~BeveledButton();
  void paintEvent(QPaintEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  QColor getColor() const;
  void setColor(const QColor &color);
  int getBevelSize() const;
  void setBevelSize(int bevelSize);
  BevelCorners getBevelCorners() const;
  void setBevelCorners(BevelCorners bevelCorners);

private:
  QPalette palette;
  QColor color;
  BevelCorners bevelCorners = BevelCorner::TopLeft | BevelCorner::BottomRight;
  int bevelSize = 10;
  QPropertyAnimation *colorAnimation;
};

#endif
