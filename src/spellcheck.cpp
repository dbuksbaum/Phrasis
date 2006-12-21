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
#include "spellcheck.h"
#include <enchant++.h>

void spellCheck::list_dicts (const char * const lang_tag,
                             const char * const provider_name,
                             const char * const provider_desc,
                             const char * const provider_file,
                             void * user_data)
{
    spellCheck* checker = static_cast<spellCheck *> (user_data);

    checker->buffer.append( QString::fromAscii(lang_tag) );

    Q_UNUSED(provider_name);
    Q_UNUSED(provider_desc);
    Q_UNUSED(provider_file);
}

spellCheck::spellCheck(  ) : dict(0)
{
    broker = enchant::Broker::instance();
}

spellCheck::~spellCheck()
{
    if(dict)
        delete dict;
}

QStringList spellCheck::langs()
{
    //QMutexLocker locker(&mutex);

    buffer.clear();

    broker->list_dicts(list_dicts,this);

    return buffer;
}

bool spellCheck::setLang(QString lang)
{
    try
    {
        //FIXME there may be a memory leak here
        dict = broker->request_dict ( lang.toUtf8().data() );
    }
    catch (enchant::Exception & ex)
    {
        qDebug("[Spelling] Unable to set lanuage [%s] Reason: %s", lang.toUtf8().data(), ex.what());
        return false;
    }

    return true;
}

bool spellCheck::checkWord(QString word)
{
    if (!dict)
    {
        qDebug() << "[Spelling] No Dict! return true";
        return true;
    }
    bool ok = dict->check( word.toUtf8().data() );
    lastWord = word;
    //qDebug("[Spelling] Checking: %s (%d)", word.toUtf8().data(), ok);
    return ok;
}

QStringList spellCheck::suggestions(QString word)
{
    if (!dict)
    {
        qDebug() << "[Spelling] No Dict! return true";
        return QStringList();
    }
    if (word==QString())
    {
        if ( lastWord==QString() )
        {
            qDebug() << "[Spelling] No word has been checked, please specify";
            return QStringList();
        }
        word = lastWord;
    }

    std::vector<std::string> suggs;
    dict->suggest (word.toUtf8().data(), suggs);

    qDebug() << "[Spelling] Suggestions for " << word << " found.";

    QStringList list;

    std::string string;
    foreach ( string, suggs )
    {
        list.append( QString::fromStdString(string) );
    }
    return list;
}


bool spellCheck::addToSession(QString word)
{
    qDebug() << "[Spelling] Adding " << word << " to session dictionary.";
    ;
    dict->add_to_session (word.toUtf8().data());
    return true;
}

bool spellCheck::addToPersonalDict(QString word)
{
    qDebug() << "[Spelling] Adding " << word << " to personal dictionary.";
    ;
    dict->add_to_pwl (word.toUtf8().data());
    return true;
}



// test personal wordlist dictionaries
//   dict = broker->request_pwl_dict ("test.pwl");
////describe_dict (dict);
////run_dict_tests (dict);
//   delete dict;

