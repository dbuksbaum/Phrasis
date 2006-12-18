#include <QtGui>

#include "dialogimpl.h"

//
DialogImpl::DialogImpl( QWidget * parent, Qt::WFlags f)
        : QDialog(parent, f)
{
    setupUi(this);
    //showFullScreen();

    textEdit->setVerticalScrollBar( vScrollBar );
    vScrollBar->show();

    setObjectName("window");
    readSettings();

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

    connect(textEdit->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    setCurrentFile("");
    documentWasModified();

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
    QFont font = textEdit->document()->defaultFont();
    font.setPointSize(font.pointSize()+2);
    textEdit->document()->setDefaultFont(font);
}

void DialogImpl::zoomOut()
{
    QFont font = textEdit->document()->defaultFont();
    font.setPointSize(font.pointSize()-2);
    textEdit->document()->setDefaultFont(font);
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
        textEdit->clear();
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

void DialogImpl::documentWasModified()
{
    setWindowModified(textEdit->document()->isModified());

    const QString text( textEdit->document()->toPlainText() );

    QStringList list = text.split(" ",QString::SkipEmptyParts);
    const int words = list.count();

    list = text.split("\n",QString::SkipEmptyParts);
    const int paras = list.count();

    label->setText(tr("Chars\n"
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

    setStyleSheet(QString("QDialog#window, QTextEdit#textEdit {"
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
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setPlainText(in.readAll());
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
