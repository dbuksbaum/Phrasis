#include "grammarcheck.h"
#include <QtGui>

QList<bool> grammarCheck::results( )
{
    return m_results;
}

grammarCheck::~grammarCheck( )
{
    if (m_checker)
        delete m_checker;
}

grammarCheck::grammarCheck( )
{
    //create dict and opt data, give ownership to checker
    Parse_Options opts = parse_options_create();
    Dictionary dict = dictionary_create_lang("en");
    
    m_checker = new linkGrammar(dict,opts);
}

grammarCheck::grammarCheck( QStringList sentences )
{
    //create dict and opt data, give ownership to checker
    Parse_Options opts = parse_options_create();
    Dictionary dict = dictionary_create_lang("en");

    m_sentences = sentences;
    m_checker = new linkGrammar(dict,opts);

    check();
}

void grammarCheck::setSentences( QStringList sentences )
{
    m_sentences = sentences;
    
    check();
}

void grammarCheck::check( )
{
    m_results.clear();

    if ( !m_sentences.count() )
        return;

    bool ok;

    for (int i=0; i<m_sentences.count(); ++i)
    {
        qDebug() << "[Grammar] Checking: " << m_sentences.at(i).toLatin1().data();

        ok = m_checker->parseSentence(m_sentences.at(i));

        m_results.append(ok);
        
        qDebug() << "[Grammar] Correct? " << ok;
        qDebug() << "=======================";
    }
}
