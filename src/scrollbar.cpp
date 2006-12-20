#include <QtGui>

#include "scrollbar.h"
//
scrollBar::scrollBar( QWidget * parent )
        : QScrollBar(Qt::Vertical,parent)
{
    QPalette* p = new QPalette(Qt::green);
    setPalette(*p);
}
//
void scrollBar::paintEvent ( QPaintEvent * event )
{
    //QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    // painter.translate(width() / 2, height() / 2);
    // painter.scale(side / 200.0, side / 200.0);

    QScrollBar::paintEvent(event);
}
