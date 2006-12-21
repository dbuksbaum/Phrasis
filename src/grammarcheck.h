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

#ifndef GRAMMARCHECK
#define GRAMMARCHECK

#include <QList>
#include <QStringList>
#include "linkgrammar.h"

class QStringList;

class grammarCheck
{
public:
    grammarCheck( );
    grammarCheck( QStringList sentences );
    void setSentences( QStringList sentences );
    ~grammarCheck( );
    void check( );
    QList<bool> results( );

private:
    linkGrammar* m_checker;
    QStringList m_sentences;
    QList<bool> m_results;
};

#endif

