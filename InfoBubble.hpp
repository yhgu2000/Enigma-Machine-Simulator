#pragma once

const int SHADOW_WIDTH = 15;    // 窗口阴影宽度;
const int TRIANGLE_WIDTH = 15;  // 小三角的宽度;
const int TRIANGLE_HEIGHT = 10; // 小三角的高度;
const int BORDER_RADIUS = 5;    // 窗口边角的弧度;

#include <QLabel>
#include <QWidget>

class InfoBubble : public QWidget
{
  Q_OBJECT

public:
  enum Derection
  {
    left,
    right,
    up,
    down
  };

public:
  InfoBubble(QWidget* parent = nullptr);

  // 设置小三角起始位置;
  void setStartPos(int startX);
  // 设置小三角宽和高;
  void setTriangleInfo(int width, int height);
  void setText(QString s);
  void setDerection(Derection d);
  QString text();
  QLabel* lb_text;

protected:
  void paintEvent(QPaintEvent*);
  void mousePressEvent(QMouseEvent*);

private:
  // 小三角起始位置;
  int m_startX;
  // 小三角的宽度;
  int m_triangleWidth;
  // 小三角高度;
  int m_triangleHeight;
  Derection derect;
};

/*
 *   InfoBubble b;
  b.setDerection(InfoBubble::left);
  b.setGeometry(510, 103, 130, 80);
  b.setStartPos(60);
  b.setTriangleInfo(20, 12);
  b.hide();
*/
