#include "beveledbutton.h"

#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QPoint>
#include <QPolygon>
#include <QStyleOptionButton>

BeveledButton::BeveledButton(QWidget *parent) : QPushButton(parent) {
  palette = QApplication::palette(this);
  color = palette.button().color();
  colorAnimation = new QPropertyAnimation(this, "color");
  colorAnimation->setDuration(200);
  colorAnimation->setEasingCurve(QEasingCurve::InOutSine);
}

BeveledButton::~BeveledButton() { delete colorAnimation; }

QColor BeveledButton::getColor() const { return color; }

void BeveledButton::setColor(const QColor &color) {
  this->color = color;
  update();
}

int BeveledButton::getBevelSize() const { return bevelSize; }

void BeveledButton::setBevelSize(int bevelSize) {
  this->bevelSize = bevelSize;
  update();
}

BevelCorners BeveledButton::getBevelCorners() const { return bevelCorners; }

void BeveledButton::setBevelCorners(BevelCorners bevelCorners) {
  this->bevelCorners = bevelCorners;
  update();
}

void BeveledButton::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(getColor());
  painter.setPen(palette.accent().color());
  QPolygon polygon;
  if (bevelCorners & BevelCorner::TopLeft) {
    polygon << QPoint(0, bevelSize);
    polygon << QPoint(bevelSize, 0);
  } else {
    polygon << QPoint(0, 0);
  }
  if (bevelCorners & BevelCorner::TopRight) {
    polygon << QPoint(width() - bevelSize, 0);
    polygon << QPoint(width(), bevelSize);
  } else {
    polygon << QPoint(width(), 0);
  }
  if (bevelCorners & BevelCorner::BottomRight) {
    polygon << QPoint(width(), height() - bevelSize);
    polygon << QPoint(width() - bevelSize, height());
  } else {
    polygon << QPoint(width(), height());
  }
  if (bevelCorners & BevelCorner::BottomLeft) {
    polygon << QPoint(bevelSize, height());
    polygon << QPoint(0, height() - bevelSize);
  } else {
    polygon << QPoint(0, height());
  }
  painter.drawPolygon(polygon);

  painter.setPen(palette.buttonText().color());
  QRect rect = this->rect();
  rect.adjust(bevelSize, 0, bevelSize, 0);
  if (!icon().isNull()) {
    QSize iconSize = this->iconSize();
    QRect iconRect((height() - iconSize.height()) / 2, (height() - iconSize.height()) / 2, iconSize.width(),
                   iconSize.height());
    icon().paint(&painter, iconRect, Qt::AlignCenter);
  }
  painter.drawText(rect, Qt::AlignCenter, text());
}

void BeveledButton::enterEvent(QEnterEvent *event) {
  colorAnimation->stop();
  colorAnimation->setStartValue(color);
  colorAnimation->setEndValue(palette.mid().color());
  colorAnimation->start();
}

void BeveledButton::leaveEvent(QEvent *event) {
  colorAnimation->stop();
  colorAnimation->setStartValue(color);
  colorAnimation->setEndValue(palette.button().color());
  colorAnimation->start();
}

void BeveledButton::mousePressEvent(QMouseEvent *event) {
  QPushButton::mousePressEvent(event);
  colorAnimation->stop();
  colorAnimation->setStartValue(color);
  colorAnimation->setEndValue(palette.dark().color());
  colorAnimation->start();
}

void BeveledButton::mouseReleaseEvent(QMouseEvent *event) {
  QPushButton::mouseReleaseEvent(event);
  colorAnimation->stop();
  colorAnimation->setStartValue(color);
  colorAnimation->setEndValue(palette.button().color());
  colorAnimation->start();
}
