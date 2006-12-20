/****************************************************************************
**
** Select Parts Copyright (C) 2004-2006 Trolltech ASA. All rights reserved.
** 
****************************************************************************/

#ifndef PREVIEWFORM_H
#define PREVIEWFORM_H

#include <QDialog>
#include <QList>

class QComboBox;
class QDialogButtonBox;
class QLabel;
class QTextCodec;
class QTextEdit;

class PreviewForm : public QDialog
{
    Q_OBJECT

public:
    PreviewForm(QWidget *parent = 0);

    void setCodecList(const QList<QTextCodec *> &list);
    void setEncodedData(const QByteArray &data);
    int mib() const;
private slots:
    void updateTextEdit();

private:
    QByteArray encodedData;
    QString decodedStr;

    QComboBox *encodingComboBox;
    QLabel *encodingLabel;
    QTextEdit *textEdit;
    QDialogButtonBox *buttonBox;
};

#endif
