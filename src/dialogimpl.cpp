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

#include "dialogimpl.h"
#include "previewform.h"
#include "grammarcheck.h"
#include "spellcheck.h"

//
DialogImpl::DialogImpl( QWidget * parent, Qt::WFlags f)
        : QWidget(parent, f), sentenceTally(0)
{
    setupUi(this);
    //showFullScreen();

    setObjectName("window");
    readSettings();
    findCodecs();
    previewForm = new PreviewForm(this);
    previewForm->setCodecList(codecs);

    gc = new grammarCheck;

    //qApp->setCursorFlashTime(0)   // set the cursor not to flash


    topArrow->load(tr(":/icons/up.svg"));
    bottomArrow->load(tr(":/icons/down.svg"));

    connect(topArrow, SIGNAL( pressed() ),
            this, SLOT( up() ));
    connect(bottomArrow, SIGNAL( pressed() ),
            this, SLOT( down() ));

    connect(sideFrame, SIGNAL( leaving() ),
            topArrow, SLOT( leave() ));
    connect(sideFrame, SIGNAL( leaving() ),
            bottomArrow, SLOT( leave() ));
    connect(sideFrame, SIGNAL( entering() ),
            topArrow, SLOT( enter() ));
    connect(sideFrame, SIGNAL( entering() ),
            bottomArrow, SLOT( enter() ));

    new QShortcut ( QKeySequence(QKeySequence::New), this, SLOT( newFile() ) );
    new QShortcut ( QKeySequence(QKeySequence::Open), this, SLOT( open() ) );
    new QShortcut ( QKeySequence(QKeySequence::Save), this, SLOT( save() ) );
    new QShortcut ( QKeySequence(QKeySequence::Print), this, SLOT( print() ) );

    //ensure that we have a non-overlapping keybinding on all supported plattformss
    new QShortcut ( QKeySequence(QKeySequence::Underline), this, SLOT( options() ) );
    new QShortcut ( QKeySequence(QKeySequence::Bold), this, SLOT( font() ) );

    new QShortcut ( QKeySequence(QKeySequence::HelpContents), this, SLOT( help() ) );

    new QShortcut ( QKeySequence(QKeySequence::ZoomIn), this, SLOT( zoomIn() ) );
    new QShortcut ( QKeySequence(QKeySequence::ZoomOut), this, SLOT( zoomOut() ) );

    new QShortcut ( QKeySequence(tr("Ctrl+Q", "Quit Application")) , this, SLOT( close() ) );
    new QShortcut ( QKeySequence(tr("Ctrl+F", "Toggle Fullscreen")) , this, SLOT( togleFullScreen() ) );

    // new QShortcut ( QKeySequence(tr("Ctrl+T", "Test")) , this, SLOT( test() ) );

#ifdef Q_OS_WIN32

    QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

    QSettings settings;
#endif

    settings.setValue("README","Please read the help file"
                      " by pressing F1, the help key, for"
                      " instructions on how to modify this file.");

    fw = new QFileSystemWatcher(this);
    fw->addPath( settings.fileName() );

    connect(fw, SIGNAL(fileChanged(const QString)),
            this, SLOT(readSettings()));

    connect(textEdit->document(), SIGNAL(contentsChange(int,int,int)),
            this, SLOT(documentWasModified(int,int,int)));

    setCurrentFile("");
    documentWasModified(0,0,0);

    sc = new spellCheck();
    sc->setLang("en_US");
}

DialogImpl::~DialogImpl()
{
    if (gc)
        delete gc;
    if (sc)
        delete sc;

}

void DialogImpl::up()
{
    QScrollBar* sb = textEdit->verticalScrollBar();
    sb->setValue(sb->value()-25);
    //sb->triggerAction(QAbstractSlider::SliderSingleStepSub);
}

void DialogImpl::down()
{
    QScrollBar* sb = textEdit->verticalScrollBar();
    sb->setValue(sb->value()+25);
    //sb->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}

void DialogImpl::print()
{
    QTextDocument* document = textEdit->document();
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted)
        return;

    document->print(&printer);
}

void DialogImpl::help()
{
    loadFile(":/HELP");
}

void DialogImpl::options()
{
#ifdef Q_OS_WIN32
    QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

    QSettings settings;
#endif

    loadFile(settings.fileName());

}

void DialogImpl::font()
{
    bool ok;
    QFont font = QFontDialog::getFont( &ok, textEdit->document()->defaultFont(), this );

    if (ok)
        textEdit->document()->setDefaultFont(font);
}

void DialogImpl::zoomIn()
{
    textEdit->zoomIn(2);
}

void DialogImpl::zoomOut()
{
    textEdit->zoomOut(2);
}

void DialogImpl::togleFullScreen()
{
    if ( !isFullScreen() )
        showFullScreen();
    else
        showNormal();
}

void DialogImpl::closeEvent(QCloseEvent *event)
{
    qDebug() << "closing";
    if (maybeSave())
    {
        fw->disconnect();
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void DialogImpl::newFile()
{
    if (maybeSave())
    {

        textEdit->document()->blockSignals(true);
        textEdit->clear();
        textEdit->document()->blockSignals(false);

        setCurrentFile("");
    }
}

void DialogImpl::open()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool DialogImpl::save()
{
    //qDebug() << "writeable? " << QFileInfo(curFile).isWritable();
    if (curFile.isEmpty() || !QFileInfo(curFile).isWritable() )
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}

bool DialogImpl::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void DialogImpl::checkGrammer(const QTextBlock& block)
{
    //clearFormating( block );

    // set formating
    QTextCharFormat tf;
    tf.setUnderlineColor( QColor(Qt::cyan) );
    tf.setUnderlineStyle(QTextCharFormat::DotLine);

    QList<int> boundries = sentenceBoundries( block );
    if (boundries.count() < 2)
        return;

    QTextCursor sentence(block);
    QStringList sentences;

    // create sentence list for parser
    for (int i=1; i<boundries.count(); ++i)
    {
        sentence.setPosition(boundries[i-1], QTextCursor::MoveAnchor);
        sentence.setPosition(boundries[i], QTextCursor::KeepAnchor);

        sentences.append( sentence.selectedText().simplified() );
    }

    // check grammer
    gc->setSentences(sentences);

    QList<bool> checked = gc->results();

    // apply format to results
    for (int i=0; i<sentences.count(); ++i)
    {
        if (!checked.at(i))
        {
            sentence.setPosition(boundries[i], QTextCursor::MoveAnchor);
            sentence.setPosition(boundries[i+1], QTextCursor::KeepAnchor);
            sentence.setCharFormat ( tf );
        }
    }
}

void DialogImpl::checkSpelling(const QTextBlock& block)
{
    //clearFormating( block );

    // set formating
    QTextCharFormat tf;
    tf.setUnderlineColor( QColor(Qt::red) );
    tf.setUnderlineStyle(QTextCharFormat::WaveUnderline);

    const QString text( block.text() );

    // nothing to check
    if (text==QString())
        return;

    QRegExp wordsRX("\\s+");
    QStringList words = text.split(wordsRX,QString::SkipEmptyParts);

    //qDebug() << "[Parser] Wordlist: "<< words;

    QTextCursor wordCursor(block);
    QRegExp strip_punct("(^\\W*|\\W*$)");
    int blockOff = block.position();
    int offset = 0;
    int start = 0;

    QString word;
    foreach(word,words)
    {
        //qDebug() << "[Parser] Checking "<< word << " ...";

        start = text.indexOf( word, offset );
        offset = start + word.size();

        if (!sc->checkWord( word.remove( strip_punct )))
        {
            wordCursor.setPosition(blockOff + start, QTextCursor::MoveAnchor);
            wordCursor.setPosition(blockOff + offset, QTextCursor::KeepAnchor);
            wordCursor.setCharFormat ( tf );
        }
    }
}

void DialogImpl::highlightSentences(const QTextBlock& block)
{
    highlightSentences( sentenceBoundries( block ) );
}

void DialogImpl::highlightSentences(QList<int> boundries)
{
    QTextCharFormat stripe[2];
    stripe[0].setFontUnderline ( true );
    stripe[0].setUnderlineColor( QColor(Qt::red) );
    stripe[1].setFontUnderline ( true );
    stripe[1].setUnderlineColor( QColor(Qt::blue) );

    int flop=0;

    QTextCursor sentence(textEdit->document());
    sentence.setPosition(boundries[0]);

    clearFormating( sentence.block() );

    for (int i=1; i<boundries.count(); ++i)
    {
        sentence.setPosition(boundries[i], QTextCursor::KeepAnchor);
        sentence.setCharFormat ( stripe[flop=!flop] );

        sentence.setPosition(boundries[i], QTextCursor::MoveAnchor);
    }

}

void DialogImpl::clearFormating(const QTextBlock& block)
{
    textEdit->document()->blockSignals(true);

    QTextCharFormat tf;
    //tf.setFontUnderline ( false );

    QTextCursor tc(block);
    tc.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );

    tc.setCharFormat ( tf );

    textEdit->document()->blockSignals(false);
}

int DialogImpl::sentenceCount(QTextBlock& block) const
{
    return sentenceBoundries(block).count()-1;
}

QList<int> DialogImpl::sentenceBoundries(const QTextBlock& block) const
{

    // QRegExp rx("("+pre_punct+"[\\.\\?\\!]["+post_punct+"]?\\s+[A-Z])|(.$)");


    // check for sentence barriers

    // ensure that the punctuation comes after a word, not an abreviation
    static QString pre_punct = "[^A-Z][0-9a-z\\(\\)]{2}";
    // allow quotes, or parenthesis/brackets to come after a period
    static QString post_punct = QRegExp::escape("\"[]()") + "\\x201C\\x201D\\x201E\\x201F";
    //                            (end of a word) (punctuation)  (after punct)  (spaces) (new word w/Capital)
    static QString end_of_sentence = pre_punct + "[\\.\\?\\!][" + post_punct + "]?\\s+[A-Z]";
    // the end of a block
    static QString end_of_para = ".$";

    QRegExp boundryRX("("+end_of_sentence+")|("+end_of_para+")");
    boundryRX.setPatternSyntax(QRegExp::RegExp2);

    // create textcursor, with position at the begining of the block
    QTextCursor boundry(block);

    QList<int> sentences;
    sentences.append( boundry.position() );

    while (1)
    {
        boundry = textEdit->document()->find( boundryRX, boundry);

        if ( boundry == QTextCursor() )
            break;

        int newPosition = boundry.position();
        newPosition = boundry.atBlockEnd() ? newPosition : newPosition-1;

        sentences.append( newPosition );
        boundry.setPosition( newPosition );

        if ( boundry.atBlockEnd() )
            break;
    }
    return sentences;
}

void DialogImpl::documentWasModified(int position, int charsRemoved, int charsAdded)
{
    Q_UNUSED(charsRemoved);
    Q_UNUSED(charsAdded);

    setWindowModified(textEdit->document()->isModified());

    const QString text( textEdit->document()->toPlainText() );

    //Compute words
    QRegExp wordsRX("\\s+");
    QStringList list = text.split(wordsRX);
    const int words = list.count();

    //Compute paras
    list = text.split("\n",QString::SkipEmptyParts);
    const int paras = list.count();

    //Compute sentences
    QTextBlock block = textEdit->document()->findBlock ( position );
    //    const int sents = -1; //sentenceCount(block);

    clearFormating( block );
    checkGrammer(block);
    checkSpelling(block);

    // highlightSentences(block);

    statsLabel->setText(tr("Chars\n"
                           "%1\n"
                           "Words\n"
                           "%2\n"
                           "Paras\n"
                           "%3\n"
                           "\n\n",
                           "Statistics"
                          ).arg( text.size() ).arg( words ).arg( paras )
                       );
}

void DialogImpl::readSettings()
{
#ifdef Q_OS_WIN32
    QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

    QSettings settings;
#endif

    QPoint pos = settings.value("WindowState/TopLeftPosition", QPoint(100, 100)).toPoint();
    QSize size = settings.value("WindowState/WindowSize", QSize(300, 200)).toSize();
    QString fontS = settings.value("Font/Font_Settings", textEdit->document()->defaultFont() ).toString();
    QString color = settings.value("Font/Color-Foreground", "#00ff00" ).toString();
    QString back = settings.value("Font/Color-Background", "black" ).toString();

    setStyleSheet(QString("QWidget#window, QTextEdit#textEdit {"
                          "color: %1;"
                          "background-color: %2;"
                          "selection-color: %2;"
                          "selection-background-color: %1;}"
                         ).arg(color).arg(back)
                 );

    QFont font;
    font.fromString(fontS);
    textEdit->document()->setDefaultFont( font );

    resize(size);
    move(pos);

    if ( settings.value("WindowState/ShowFullScreen", true).toBool() )
        showFullScreen();
    else
        showNormal();

}

void DialogImpl::writeSettings()
{

#ifdef Q_OS_WIN32
    QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

    QSettings settings;
#endif

    if ( !isFullScreen() )
    {
        settings.setValue("WindowState/TopLeftPosition", pos());
        settings.setValue("WindowState/WindowSize", size());
    }

    settings.setValue("Font/Font_Settings", textEdit->document()->defaultFont().toString() );
    settings.setValue("WindowState/ShowFullScreen", isFullScreen());
}

bool DialogImpl::maybeSave()
{
    if (textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, qApp->applicationName(),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void DialogImpl::findCodecs()
{
    QMap<QString, QTextCodec *> codecMap;
    QRegExp iso8859RegExp("ISO[- ]8859-([0-9]+).*");

    foreach (int mib, QTextCodec::availableMibs())
    {
        QTextCodec *codec = QTextCodec::codecForMib(mib);

        QString sortKey = codec->name().toUpper();
        int rank;

        if (sortKey.startsWith("UTF-8"))
        {
            rank = 1;
        }
        else if (sortKey.startsWith("UTF-16"))
        {
            rank = 2;
        }
        else if (iso8859RegExp.exactMatch(sortKey))
        {
            if (iso8859RegExp.cap(1).size() == 1)
                rank = 3;
            else
                rank = 4;
        }
        else
        {
            rank = 5;
        }
        sortKey.prepend(QChar('0' + rank));

        codecMap.insert(sortKey, codec);
    }
    codecs = codecMap.values();
}


void DialogImpl::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, qApp->applicationName(),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    in.setAutoDetectUnicode( false );

    QByteArray data = file.read(512);
    previewForm->setEncodedData(data);

    if (previewForm->exec())
    {
        QTextCodec *codec = QTextCodec::codecForMib(previewForm->mib());
        in.setCodec(codec);
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    in.seek(0);
    textEdit->document()->blockSignals(true);
    textEdit->setPlainText(in.readAll());
    textEdit->document()->blockSignals(false);

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
}

bool DialogImpl::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, qApp->applicationName(),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}

void DialogImpl::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg( qApp->applicationName() ));
}

QString DialogImpl::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
