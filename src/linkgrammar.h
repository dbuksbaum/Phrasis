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
