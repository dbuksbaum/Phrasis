#ifndef DIALOGIMPL_H
#define DIALOGIMPL_H
//
#include "ui_dialog.h"
//
class QShortcut;
class QFileSystemWatcher;

class DialogImpl : public QDialog, public Ui::Dialog
{
    Q_OBJECT
public:
    DialogImpl( QWidget * parent = 0, Qt::WFlags f = 0 );

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
    void documentWasModified();
    void readSettings();
    void font();
    void help();

private:
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QString curFile;
    QFileSystemWatcher* fw;
protected:
    void closeEvent(QCloseEvent *event);

};
#endif




