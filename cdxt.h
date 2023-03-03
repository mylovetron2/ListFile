#ifndef CDXT_H
#define CDXT_H
#include <QFile>

class CDxt
{
public:
    CDxt();
    CDxt(QString sfileName);
    QFile m_file;
    QString readLine();
    QByteArray readArrayInLine();
    QString readNextLine();

private:

    QByteArray m_arrData;
    QStringList m_lstLine;
    QString m_line;
};

#endif // CDXT_H
