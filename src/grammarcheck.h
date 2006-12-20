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

