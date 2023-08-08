#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QDialog>

class CustomWindow : public QDialog
{
    Q_OBJECT

public:
    CustomWindow(QWidget* parent = 0);
    ~CustomWindow();

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    bool   mMoveing;
    QPoint mMovePosition;
};

#endif // CUSTOMWINDOW_H