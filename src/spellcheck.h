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

#ifndef SPELLCHECK
#define SPELLCHECK

//#include <QString>
//#include <enchant++.h>
class QString;
class QMutex;

namespace enchant {
class Broker;
class Dict;
};

class spellCheck
{

public:
    spellCheck();
    ~spellCheck();
    QStringList langs();
    bool setLang(const QString lang);
    bool checkWord(const QString word);
    QStringList suggestions(const QString word = QString());
    bool addToSession(const QString word);
    bool addToPersonalDict(const QString word);

    //bool storeReplacement (const QString& bad, const QString& good)

private:
    static void list_dicts (const char * const lang_tag,
                        const char * const provider_name,
                        const char * const provider_desc,
                        const char * const provider_file,
                        void * user_data);

    enchant::Broker* broker;
    enchant::Dict* dict;

    QString lastWord;
    QStringList buffer;
    //QMutex mutex;
};

#endif
