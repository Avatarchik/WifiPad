#ifndef GAMEPADVIEW_H
#define GAMEPADVIEW_H

#include <QtGui/QWidget>
#include "MainWindow.h"
#include "GamePad.h"

class GamePadView : public QWidget
{
    Q_OBJECT

private:
    MainWindow *m_main;
    int m_selectedButton;
protected:
    virtual void paintEvent ( QPaintEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void leaveEvent ( QEvent * event );
public:
    GamePadView(QWidget *parent);
    virtual ~GamePadView() {}

    void SetMainWindow(MainWindow *w) { m_main = w; }
public slots:
    void Refresh();
};

#endif // GAMEPADVIEW_H
