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

#ifndef LINKGRAMMER
#define LINKGRAMMER

#include <QString>

extern "C"
{
#include <link-grammar/link-includes.h>
}

class linkGrammar
{
public:
    linkGrammar();
    linkGrammar(Dictionary dict, Parse_Options opts);
    ~linkGrammar();
    bool parseSentence(const QString text) const;
private:
    Dictionary      m_Dict;
    Parse_Options   m_Opts;
};

#endif
