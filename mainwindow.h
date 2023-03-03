#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <CLis.h>
#include <cdxt.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QByteArray getDataSGK(QString s);
    QByteArray getDataBK79(QString s);
    QByteArray getDataPS(QString s);
    QByteArray getDataIBK7(QString s);

public:
    void writeLineLas(float iCurDepth, QByteArray r);
    void getData(QByteArray r);
private:
    Ui::MainWindow *ui;
    QFile file;
    QFile filenew;
    CLis *myLisFile;
    CDxt *myDxt;
    QFile m_fileLas;
    QList<float> m_arrIBK7;
    QList<float> m_arrTint;
    QList<float> m_arrUBK7;
    QList<float> m_arrIBK9;
    QList<float> m_arrUBK9;
    int m_minIBK7;
    int m_minUBK7;
    int m_minIBK9;
    int m_minUBK9;
    float kGK;
    float aRBK7;
    float bRBK7;
    float aRBK9;
    float bRBK9;
    float aGK;
    float bGK;

};
#endif // MAINWINDOW_H
