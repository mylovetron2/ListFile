#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVector>
#include <QFloat16>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_minIBK7=-2016;
    m_minIBK9=-224;
    m_minUBK7=-5;
    m_minUBK9=-10;

    //kGK=(float)224/75670;
    aGK=1.0/120;                            //1500->4 1380->3
    bGK=3.0-1380.0*aGK;

//    aRBK7=(float)(14-4)/(0.8-0.27);
//    bRBK7=(float)(4-aRBK7*0.27);

    aRBK7=(float)(14-4)/(0.6291-0.2402);
    bRBK7=(float)(4-aRBK7*0.2402);
    //bRBK7=(float)(6-aRBK7*0.2402);

    aRBK9=(1200-2)/(6-0.006);
    bRBK9=(1200-aRBK9*6);

    //myLisFile=new CLis("testFinal2.LIS");
    myLisFile=new CLis();
    //myLisFile->addCurve("DEEE",73,4,"M");
    myLisFile->addCurve("TIME",73,4,"S");
    myLisFile->addCurve("PS",79,2,"");
    myLisFile->addCurve("IBK7",79,2,"");
    myLisFile->addCurve("Tint",68,4,"C");
    myLisFile->addCurve("UBK7",79,2,"");
    myLisFile->addCurve("IBK9",79,2,"");
    myLisFile->addCurve("UBK9",79,2,"");
    myLisFile->addCurve("RBK7",79,2,"");
    myLisFile->addCurve("RBK9",79,2,"");
    myLisFile->addCurve("THOR",73,4,"");
    myLisFile->addCurve("URAN",73,4,"");
    myLisFile->addCurve("KALI",73,4,"");
    myLisFile->addCurve("GK",73,4,"");

    unsigned char  *b1;
    b1=new unsigned char(2);
    QVector<int> arrSGK(512);
    int temp=127;
    b1[0]=temp&0xff;
    b1[1]=(temp >> 8) & 0xff;
    arrSGK[0]=b1[1];
    arrSGK[1]=b1[0];

    temp=128;
        b1[0]=temp&0xff;
        b1[1]=(temp >> 8) & 0xff;
        arrSGK[0]=b1[1];
        arrSGK[1]=b1[0];

    int n=0;
    for(int i=0;i<256;i++)
    {
        b1[0]=i&0xff;
        b1[1]=(i >> 8) & 0xff;
        arrSGK[n]=b1[1];
        arrSGK[n+1]=b1[0];
        n=n+2;
    }

    QString sDxt;
    QByteArray dataSGK,dataBK79,dataPS,dataIBK7,dataTint;
    //dataPS.resize(2);
    //dataIBK7.resize(2);
    dataTint.resize(2);

    myDxt=new CDxt("sgkbk79-290323.dxt");

    //myDxt=new CDxt("log_old.dxt");

    sDxt=myDxt->readLine();
    sDxt=myDxt->readLine();
    QByteArray r;
    r.resize(592);               // mang file DXT
    long test;
    long depth=16300;   //13300
    float aDepth=(float)(119-115)/(298200-297800);
    float bDepth=(float)119-298200*aDepth;

    float real_depth=0;
    int firstRead=0;
    QList<QVariant> list;
    int minIBK7=0;

    m_fileLas.setFileName("3103_23.Las");
    if (!m_fileLas.open(QIODevice::ReadWrite|QIODevice::Text)){
       qDebug()<<"Ko mo duoc file LAS";
       return;
    }

    QString sLas;
    for(int i=0;i<64;i++)
        sLas=m_fileLas.readLine();


/*

//Khi co độ sâu lại
    int step=0;
    int maxStep=11;                   //maxStep=11 cứ 11 điểm thì bỏ 1 điểm
    while (!myDxt->m_file.atEnd()){
        real_depth=(float)depth/100;

        if(real_depth<=0)
            break;

        r=myDxt->readArrayInLine();

        QByteArray check;
        check.resize(2);
        check[1]=r[287*2];
        check[0]=r[287*2+1];
        quint16 dataCheck;
        memcpy(&dataCheck, check.data(), sizeof(quint16));

        if(dataCheck==36371)
        {
            QByteArray temp;
            temp.resize(2);
            temp[1]=r[257*2];
            temp[0]=r[257*2+1];
            quint16 data;
            memcpy(&data, temp.data(), sizeof(qint16));
//Thuật toán co độ sâu lại
//Cứ maxStep thì bỏ 1 điểm
            if(step<maxStep)
            {
                this->writeLineLas(real_depth,r);
                depth=depth-10;
            }
            if(step==maxStep)
                step=0;

        }
        step++;

    }

*/

//Khi giãn độ sâu ra
int step=0;
int maxStep=2;                   //maxStep=5 cứ 5 điểm thì thêm 1 điểm
while (!myDxt->m_file.atEnd()){
    real_depth=(float)depth/100;

    if(real_depth<=0)
        break;

    if(step<maxStep)
        r=myDxt->readArrayInLine();
    if(step==maxStep)
        step=0;


    QByteArray check;
    check.resize(2);
    check[1]=r[287*2];
    check[0]=r[287*2+1];
    quint16 dataCheck;
    memcpy(&dataCheck, check.data(), sizeof(quint16));

    if(dataCheck==36371)
    {
        QByteArray temp;
        temp.resize(2);
        temp[1]=r[257*2];
        temp[0]=r[257*2+1];
        quint16 data;
        memcpy(&data, temp.data(), sizeof(qint16));


        this->writeLineLas(real_depth,r);
        depth=depth-10;


    }
    step++;

}



}

MainWindow::~MainWindow()
{
    delete ui;
}



QByteArray MainWindow::getDataSGK(QString s)
{
    QByteArray arrR;
    arrR.resize(512);
    QStringList  fields = s.split(" ");
    int n=0;

    if(fields.size()>100)   //Debug
        arrR[1]=fields.at(90).toInt();

    //SGK
    for(int i=2;i<fields.size()-1-32;i++){
        arrR[n]=(fields.at(i).toInt() >> 8) & 0xff;
        arrR[n+1]=fields.at(i).toInt()&0xff;
        n=n+2;
    }

    return arrR;
}

QByteArray MainWindow::getDataBK79(QString s)
{
    QByteArray arrR;
    arrR.resize(64);
    QStringList  fields = s.split(" ");

    int n=0;

    //BK79
    for(int i=258;i<fields.size()-1;i++){
        arrR[n]=(fields.at(i).toInt() >> 8) & 0xff;
        arrR[n+1]=fields.at(i).toInt()&0xff;
        n=n+2;
    }
    return arrR;
}

QByteArray MainWindow::getDataPS(QString s)
{
    QByteArray arrR;
    arrR.resize(2);
    QStringList  fields = s.split(" ");
    int n=0;

    //for(int i=0;i<fields.size()-1;i++){
    arrR[n]=(fields.at(266).toInt() >> 8) & 0xff;
    arrR[n+1]=fields.at(266).toInt()&0xff;
    n=n+2;
    //}


    return arrR;
}

QByteArray MainWindow::getDataIBK7(QString s)
{
    QByteArray arrR;
    arrR.resize(2);
    QStringList  fields = s.split(" ");
    int n=0;

    //for(int i=0;i<fields.size()-1;i++){
    arrR[n]=(fields.at(259).toInt() >> 8) & 0xff;
    arrR[n+1]=fields.at(259).toInt()&0xff;
    n=n+2;
    //}


    return arrR;
}

void MainWindow::writeLineLas(float iCurDepth, QByteArray r)
{
    m_fileLas.write(QString::number(iCurDepth).toStdString().c_str());
    m_fileLas.write("\n");

    QByteArray temp;
    unsigned char  *b1;

    //****************Tinh TIME  **********************
    unsigned long Time;
    Time=10000;
    m_fileLas.write(QString::number(Time).toStdString().c_str());
    m_fileLas.write(" ");


    //****************Tinh PS  **********************
    //lấy vi tri ps
    temp.resize(2);
    temp[1]=r[264*2];       //Đảo bit
    temp[0]=r[264*2+1];
    //doi ra so nguyen
    qint16 iPs;
    memcpy(&iPs,temp.data(), sizeof(qint16));
    //xu ly
    iPs=iPs-32768;
    m_fileLas.write(QString::number(iPs).toStdString().c_str());
    m_fileLas.write(" ");

    //****************Tinh IBK7  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[257*2];       //Đảo bit
    temp[0]=r[257*2+1];
    //doi ra so nguyen
    qint16 iIBK7;
    memcpy(&iIBK7,temp.data(), sizeof(qint16));
    //xu ly
    //iIBK7=iIBK7-32768+1945+41;
    iIBK7=iIBK7-32768+2018;


    m_fileLas.write(QString::number(iIBK7).toStdString().c_str());
    m_fileLas.write(" ");

    //****************Tinh Tint  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[288*2];       //Đảo bit
    temp[0]=r[288*2+1];
    //doi ra so nguyen
    qint16 iTint;
    memcpy(&iTint,temp.data(), sizeof(qint16));
    //xu ly
    float fTint=(float)iTint/10;

    m_fileLas.write(QString::number(fTint).toStdString().c_str());
    m_fileLas.write(" ");


    //****************Tinh UBK7  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[259*2];       //Đảo bit
    temp[0]=r[259*2+1];
    //doi ra so nguyen
    qint16 iUBK7;
    memcpy(&iUBK7,temp.data(), sizeof(qint16));
    //xu ly
    //iUBK7=iUBK7-32768+3;
    iUBK7=iUBK7-32768;



    m_fileLas.write(QString::number(iUBK7).toStdString().c_str());
    m_fileLas.write(" ");

    //****************Tinh IBK9  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[258*2];
    temp[0]=r[258*2+1];
    //doi ra so nguyen
    qint16 iIBK9;
    memcpy(&iIBK9,temp.data(), sizeof(qint16));
    //xu ly
    //iIBK9=iIBK9-32544;
    iIBK9=iIBK9-32768;

    if(iIBK9<=0)
        iIBK9=1;
    m_fileLas.write(QString::number(iIBK9).toStdString().c_str());
    m_fileLas.write("\n");

    //****************Tinh UBK9  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[260*2];
    temp[0]=r[260*2+1];
    //doi ra so nguyen
    qint16 iUBK9;
    memcpy(&iUBK9,temp.data(), sizeof(qint16));
    //xu ly
    //iUBK9=iUBK9-32758;
    iUBK9=iUBK9-32768+173;

    m_fileLas.write(QString::number(iUBK9).toStdString().c_str());
    m_fileLas.write(" ");

    //****************Tinh RBK7  **********************

    qint16 iRBK7;
    float fRBK7;

//    if(iIBK7<=0)
//        iIBK7=0.00001;

    if(iIBK7!=0)
    {
        fRBK7=iUBK7;
        fRBK7/=iIBK7;
    }


    fRBK7=aRBK7*fRBK7+bRBK7;

//    if(fRBK7<=0)
//        fRBK7=0.001;

    m_fileLas.write(QString::number(fRBK7).toStdString().c_str());
    m_fileLas.write(" ");

    //****************Tinh RBK9  **********************

    qint16 iRBK9;
    float fRBK9;
    if(iIBK9!=0)
        fRBK9=(float)iUBK9/iIBK9;
    else
        fRBK9=0.001;

    //fRBK9=aRBK9*fRBK9+bRBK9;

//    aRBK9=(float)(1200-2)/(6000-4);
//    bRBK9=(float)(1200-aRBK9*6000);
    fRBK9=aRBK9*fRBK9+bRBK9;

    if(fRBK9<=0)
        fRBK9=0.001;

    m_fileLas.write(QString::number(fRBK9).toStdString().c_str());
    m_fileLas.write(" ");

    //***************SGK - THORI****************************************
    QByteArray arrByteSGK;    //512 byte
    arrByteSGK.resize(512);
    for(int i=0;i<512;i++)
    arrByteSGK[i]=r[i];
    unsigned int arrValueSGK[256];   //256 value
    //arrValueSGK.resize(256);
    int n=0;
    for(int i=0;i<256;i++)
    {
        temp.resize(2);
        temp[1]=r[i*2];       //Đảo bit
        temp[0]=r[i*2+1];
        qint16 iSGK;
        memcpy(&iSGK,temp.data(), sizeof(qint16));
        arrValueSGK[n]=iSGK;
        n++;
    }

    int i;
    for(i=5;i<196;i++){
        if(arrValueSGK[i] > 32000) {		  //32000
        break;
        }
    }
    int k = i/13;
    int it = 13*k + 8 - i;
    unsigned int iTemp[256];
    for(int i=0;i<256;i++){
        if(i-it>0 && i-it<256)
            iTemp[i] = arrValueSGK[i-it];
        else
         iTemp[i]=0;
        }
        for(int i=0;i<256;i++){
            arrValueSGK[i] = iTemp[i];
    }
    //Tinh THORI
    unsigned int iThori;
    iThori=arrValueSGK[169]+arrValueSGK[222]+arrValueSGK[173];                //+ 173
    m_fileLas.write(QString::number(iThori).toStdString().c_str());
    m_fileLas.write(" ");

    //Tinh URAN
    unsigned int iUran;
    iUran=arrValueSGK[161]+arrValueSGK[172]+arrValueSGK[187];
    m_fileLas.write(QString::number(iUran).toStdString().c_str());
    m_fileLas.write(" ");

    //Tinh Kali
    unsigned int iKali;
    iKali=arrValueSGK[181];
    m_fileLas.write(QString::number(iKali).toStdString().c_str());
    m_fileLas.write("\n");

    //Tinh GK
    unsigned int iGK;
    iGK=iThori+iUran+iKali;

//    for(i=155;i<164;i++){              //i 155 i<223 if >32000 then ko cong
//        iGK += arrValueSGK[i];
//    }

//    for(i=165;i<177;i++){
//       iGK += arrValueSGK[i];
//    }

    iGK=0;
    int s=0;
        for(i=155;i<223;i++){              //i 155 i<223 if >32000 then ko cong
            if(arrValueSGK[i]<=20000){    //32000
                iGK += arrValueSGK[i];
                s++;
            }

        }

    float fGK;
    fGK=(float)iGK/s;
    iGK=(int)fGK;
    //iGK=iGK*kGK;
    fGK=aGK*iGK+bGK;
    //iGK=(int)fGK;

    m_fileLas.write(QString::number(fGK).toStdString().c_str());
    m_fileLas.write("\n");

}

void MainWindow::getData(QByteArray r)
{
    QByteArray temp;
    unsigned char  *b1;

    //****************Tinh TIME  **********************
    unsigned long Time;
    Time=10000;


    //****************Tinh PS  **********************
    //lấy vi tri ps
    temp.resize(2);
    temp[1]=r[264*2];       //Đảo bit
    temp[0]=r[264*2+1];
    //doi ra so nguyen
    qint16 iPs;
    memcpy(&iPs,temp.data(), sizeof(qint16));
    //xu ly
    iPs=iPs-32768;


    //****************Tinh IBK7  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[257*2];       //Đảo bit
    temp[0]=r[257*2+1];
    //doi ra so nguyen
    qint16 iIBK7;
    memcpy(&iIBK7,temp.data(), sizeof(qint16));
    //xu ly
    iIBK7=iIBK7-32768;
    m_arrIBK7.append(iIBK7);

    //****************Tinh Tint  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[288*2];       //Đảo bit
    temp[0]=r[288*2+1];
    //doi ra so nguyen
    qint16 iTint;
    memcpy(&iTint,temp.data(), sizeof(qint16));
    //xu ly
    float fTint=(float)iTint/10;
    m_arrTint.append(fTint);


    //****************Tinh UBK7  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[259*2];       //Đảo bit
    temp[0]=r[259*2+1];
    //doi ra so nguyen
    qint16 iUBK7;
    memcpy(&iUBK7,temp.data(), sizeof(qint16));
    //xu ly
    iUBK7=iUBK7-32768;
    m_arrUBK7.append(iUBK7);

    //****************Tinh IBK9  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[258*2];       //Đảo bit
    temp[0]=r[258*2+1];
    //doi ra so nguyen
    qint16 iIBK9;
    memcpy(&iIBK9,temp.data(), sizeof(qint16));
    //xu ly
    iIBK9=iIBK9-32768;
    m_arrIBK9.append(iIBK9);

    //****************Tinh UBK9  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[260*2];       //Đảo bit
    temp[0]=r[260*2+1];
    //doi ra so nguyen
    qint16 iUBK9;
    memcpy(&iUBK9,temp.data(), sizeof(qint16));
    //xu ly
    iUBK9=iUBK9-32768;
    m_arrUBK9.append(iUBK9);

    //****************Tinh RBK7  **********************

    qint16 iRBK7=iUBK7/iIBK7;

    //****************Tinh RBK9  **********************

    qint16 iRBK9=iUBK9/iIBK9;


    //***************SGK - THORI****************************************
    QByteArray arrByteSGK;    //512 byte
    arrByteSGK.resize(512);
    for(int i=0;i<512;i++)
    arrByteSGK[i]=r[i];
    unsigned int arrValueSGK[256];   //256 value
    //arrValueSGK.resize(256);
    int n=0;
    for(int i=0;i<256;i++)
    {
        temp.resize(2);
        temp[1]=r[i*2];       //Đảo bit
        temp[0]=r[i*2+1];
        qint16 iSGK;
        memcpy(&iSGK,temp.data(), sizeof(qint16));
        arrValueSGK[n]=iSGK;
        n++;
    }

    int i;
    for(i=5;i<196;i++){
        if(arrValueSGK[i] > 32000) {		  //32000
        break;
        }
    }
    int k = i/13;
    int it = 13*k + 8 - i;
    unsigned int iTemp[256];
    for(int i=0;i<256;i++){
        if(i-it>0 && i-it<256)
            iTemp[i] = arrValueSGK[i-it];
        else
         iTemp[i]=0;
        }
        for(int i=0;i<256;i++){
            arrValueSGK[i] = iTemp[i];
    }
    //Tinh THORI
    unsigned int iThori;
    iThori=arrValueSGK[169]+arrValueSGK[222];

    //Tinh URAN
    unsigned int iUran;
    iUran=arrValueSGK[161]+arrValueSGK[172]+arrValueSGK[187];

    //Tinh Kali
    unsigned int iKali;
    iKali=arrValueSGK[181];


    //Tinh GK
    unsigned int iGK;
    iGK=iThori+iUran+iKali;

}

