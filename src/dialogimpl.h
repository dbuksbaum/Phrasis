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

#ifndef DIALOGIMPL
#define DIALOGIMPL

#include "ui_dialog.h"

class QShortcut;
class QFileSystemWatcher;
class PreviewForm;
class grammarCheck;

class DialogImpl : public QDialog, public Ui::Dialog
{
    Q_OBJECT
public:
    DialogImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
    ~DialogImpl();

private slots:
    void togleFullScreen();
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void print();
    void options();
    void zoomIn();
    void zoomOut();
    void documentWasModified(int position, int charsRemoved, int charsAdded);
    void readSettings();
    void font();
    void help();
    void up();
    void down();

private:
    void writeSettings();
    bool maybeSave();
    void findCodecs();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    int sentenceCount(QTextBlock& block) const;
    QList<int> sentenceBoundries(const QTextBlock& block) const;
    void clearFormating(const QTextBlock& block);
    void highlightSentences(QList<int> boundries);
    void highlightSentences(const QTextBlock& block);
    void checkGrammer(const QTextBlock& block);

    PreviewForm *previewForm;
    QList<QTextCodec *> codecs;
    int sentenceTally;
    QString curFile;
    QFileSystemWatcher* fw;
    grammarCheck* gc;
protected:
    void closeEvent(QCloseEvent *event);

};
#endif
