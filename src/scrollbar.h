#ifndef SCROLLBAR_H
#define SCROLLBAR_H
//
#include <QScrollBar>
//
class scrollBar : public QScrollBar
{
    Q_OBJECT
public:
    scrollBar(QWidget * parent = 0);
protected:
    void paintEvent ( QPaintEvent * event );
};
#endif
