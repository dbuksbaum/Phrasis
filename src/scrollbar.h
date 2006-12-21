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

#ifndef SCROLLBAR
#define SCROLLBAR

#include <QScrollBar>

class scrollBar : public QScrollBar
{
    Q_OBJECT
public:
    scrollBar(QWidget * parent = 0);
protected:
    void paintEvent ( QPaintEvent * event );
};
#endif
