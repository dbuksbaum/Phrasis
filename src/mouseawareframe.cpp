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

#include "mouseawareframe.h"

mouseAwareFrame::mouseAwareFrame( QWidget * parent )
        : QFrame(parent)
{
    // TODO
}

void mouseAwareFrame::leaveEvent ( QEvent * event )
{
    emit leaving();

    Q_UNUSED(event);
}

void mouseAwareFrame::enterEvent ( QEvent * event )
{
    emit entering();
    Q_UNUSED(event);
}
