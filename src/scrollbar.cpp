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

#include <QtGui>

#include "scrollbar.h"

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
