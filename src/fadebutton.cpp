/****************************************************************************
**
** Copyright (C) 2006 Jacob R Rideout <jacob@jacobrideout.net>
** All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "fadebutton.h"
#include <QtGui>
#include <QtSvg>

fadeButton::fadeButton( QWidget * parent, const QString & file)
        : QAbstractButton(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //setMouseTracking(true);
    setAutoRepeat(true);
    setAutoRepeatDelay(250);
    setAutoRepeatInterval(100);

    svg = new QSvgRenderer(this);
    load(file);

    timeLine = new QTimeLine(650,this);
    timeLine->setFrameRange ( 0, 255 );
    timeLine->setUpdateInterval(30);

    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(repaint()));
}

void fadeButton::load ( const QString & file )
{
    if (file != QString() )
        svg->load(file);
}

void fadeButton::leave( )
{
    timeLine->setDirection(QTimeLine::Backward);
    if (timeLine->state() != QTimeLine::Running)
        timeLine->start();
}

void fadeButton::enter( )
{
    timeLine->setDirection(QTimeLine::Forward);
    if (timeLine->state() != QTimeLine::Running)
        timeLine->start();
}

fadeButton::~fadeButton()
{
    svg->deleteLater();
    timeLine->deleteLater();
}

QSize fadeButton::sizeHint() const
{
    return QSize(64,64);
}

bool fadeButton::hitButton ( const QPoint & pos ) const
{
    // assume a 64x64 icon

    // only return if clicked in the circle
    QPoint p(-32,-32);
    p+=pos;

    if ( ( (p.x() * p.x()) + (p.y() * p.y()) ) <= (31*31))
        return true;

    return false;
}

void fadeButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    svg->render(&painter);

    QRect rectangle(0, 0, 64, 64);

    int alpha = 255 - timeLine->currentFrame();
    painter.setBrush(QBrush(QColor(0,0,0,alpha)));
    painter.drawRect(rectangle);

    Q_UNUSED(event);
}


