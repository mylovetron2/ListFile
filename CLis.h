#ifndef CLIS_H
#define CLIS_H

#include <QFile>
#include <QVector>


class CDataList
{
private:
        QByteArray m_bData;
public:
        CDataList();


};


class CLis
{
public:
    CLis();
    CLis(QString sfileName);
    int m_firstDataSpecBlock;
    int m_lAddrFisrtDataRec;
    int m_ilenframRec;
    int m_iLenFrameData;
    int m_offser;
    int m_NumberCurve;    //Tính luôn DEPTH
    QFile m_myFile;

    long addDataRec(long lCurPosition,uint32_t iCurDepth, int iLenFrameData,uint32_t time,QByteArray SGK,QByteArray BK79,int test);
    void addCurve(QString TYPE,int RepCode,int Size,QString Unit);  // chi add Curve khong add Phổ
    long addDataRec(long lCurPosition,uint32_t iCurDepth, int iMaxFrameData,uint32_t time,QByteArray PS,QByteArray IBK7,QByteArray Tint);
    long addDataRec(long lCurPosition,uint32_t iCurDepth, int iMaxFrameData,uint32_t time,qint16 iPS,qint16 iIBK7,qint16 iTint);
    long addDataRec(long lCurPosition,uint32_t iCurDepth, int iMaxFrameData,uint32_t time, QByteArray r);
    long addDataRec2(long lCurPosition,uint32_t iCurDepth, int iMaxFrameData,uint32_t time, QByteArray r);

    QByteArray bitsToBytes(QBitArray bits);
    QBitArray quint8ToQBitArray(quint8 v);
    QByteArray coverFloat2ArrayLis(float f);   //chuyen số float thành 4 byte trong LIS
private:
private:
    QFile m_fileHeader;


};


#endif // CLIS_H
