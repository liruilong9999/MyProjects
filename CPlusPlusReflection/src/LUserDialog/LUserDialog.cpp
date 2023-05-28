/*
继承QDialog实现一个窗口，要求:
1.使用C++(C11)实现；
2.不使用原本的窗口边框；
3.继承的窗口包含关闭、最大化/最小化、拖拽功能；
*/

#include "LUserDialog.h"
#include <QApplication>
#include <QtGui/QMouseEvent>

CustomWindow::CustomWindow(QWidget* parent)
{
    mMoveing = false;
    // Qt::FramelessWindowHint 无边框
    // Qt::WindowStaysOnTopHint 窗口在最顶端，不会拖到任务栏下面
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->setStyleSheet("QDialog{border:2px solid green;}");
}
CustomWindow::~CustomWindow()
{
}

// 重写鼠标按下事件
void CustomWindow::mousePressEvent(QMouseEvent* event)
{
    mMoveing      = true;
    // 记录下鼠标相对于窗口的位置
    // event->globalPos()鼠标按下时，鼠标相对于整个屏幕位置
    // pos() this->pos()鼠标按下时，窗口相对于整个屏幕位置
    mMovePosition = event->globalPos() - pos();
    return QDialog::mousePressEvent(event);
}

// 重写鼠标移动事件
void CustomWindow::mouseMoveEvent(QMouseEvent* event)
{
    //(event->buttons() && Qt::LeftButton)按下是左键
    // 鼠标移动事件需要移动窗口，窗口移动到哪里呢？就是要获取鼠标移动中，窗口在整个屏幕的坐标，然后move到这个坐标，怎么获取坐标？
    // 通过事件event->globalPos()知道鼠标坐标，鼠标坐标减去鼠标相对于窗口位置，就是窗口在整个屏幕的坐标
    if (mMoveing && (event->buttons() && Qt::LeftButton) && (event->globalPos() - mMovePosition).manhattanLength() > QApplication::startDragDistance())
    {
        move(event->globalPos() - mMovePosition);
        mMovePosition = event->globalPos() - pos();
    }
    return QDialog::mouseMoveEvent(event);
}
void CustomWindow::mouseReleaseEvent(QMouseEvent* event)
{
    mMoveing = false;
}