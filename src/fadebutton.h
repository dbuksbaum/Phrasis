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

#ifndef FADEBUTTON
#define FADEBUTTON

#include <QAbstractButton>
class QSvgRenderer;
class QTimeLine;

class fadeButton : public QAbstractButton
{
    Q_OBJECT
public:
    fadeButton(QWidget * parent = 0, const QString & file = QString() );
    void load ( const QString & file );
    ~fadeButton();
    QSize sizeHint() const;

protected:
    bool hitButton ( const QPoint & pos ) const;
    void paintEvent(QPaintEvent *event);

public slots:
    void leave();
    void enter();

private:
    QSvgRenderer* svg;
    QTimeLine* timeLine;
};

#endif
