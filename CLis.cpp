#include "CLis.h"
#include <QDebug>
#include <QDataStream>
#include <QVector>
#include <QFloat16>
#include <QtMath>
#include <QBitArray>
#include <limits.h>
#include <iostream>
#include <math.h>
#include <bitset>


CLis::CLis()
{

    m_firstDataSpecBlock=17611;//17611;//;   //;   // Số này trở lên là cố định
    m_ilenframRec=90;
    m_iLenFrameData=0;
    m_offser=0;
    m_NumberCurve=0;
    //CreateLisFileFromHeader
    m_fileHeader.setFileName("1.LiS");
    if (!m_fileHeader.open(QIODevice::ReadOnly)){
       qDebug()<<"Ko mo duoc file LIS";
       return;
    }
    m_fileHeader.seek(0);

    m_myFile.setFileName("36.LIS");
    //m_myFile.setFileName("TESTDOWN.LIS");
    //m_myFile.setFileName("test001.LIS");

    m_myFile.open(QIODevice::ReadWrite); // Or QIODevice::ReadWrite
    QDataStream out(&m_myFile);
    QByteArray		pTempData;
    pTempData=m_fileHeader.read(m_firstDataSpecBlock);

//        m_myFile.seek(18235);  //17525
//        QByteArray		pTempData2;
//        pTempData2=m_myFile.read(150);
//        int tttt=0;

    QByteArray b1,temp;
    b1.resize(2);
    temp.resize(2);

    //pTempData[17591]=0;     // Depth recording mode
    //pTempData[17543]=1;      //  1:UP/0:DOWN
    //pTempData[17525+13]=42;    //Length Frame
    m_myFile.write(pTempData);
    m_myFile.flush();



}

CLis::CLis(QString sfileName)
{
     m_myFile.setFileName(sfileName);
     m_firstDataSpecBlock=17611;
     m_ilenframRec=250;
     m_iLenFrameData=292;  //292

     //CreateLisFileFromHeader
     m_fileHeader.setFileName("1.LiS");
     if (!m_fileHeader.open(QIODevice::ReadOnly)){
        qDebug()<<"Ko mo duoc file LIS";
        return;
     }
     m_fileHeader.seek(0);

     m_myFile.setFileName(sfileName);

     m_myFile.open(QIODevice::WriteOnly); // Or QIODevice::ReadWrite
     QDataStream out(&m_myFile);
     QByteArray		pTempData;
     pTempData=m_fileHeader.read(m_firstDataSpecBlock);
     //m_myFile.write(pTempData);

     QByteArray b1,temp;
     b1.resize(2);
     temp.resize(2);
     //Tao DataSpecRec
     // 86 byte dau tien cua DataSpecRec
     int iLenFrameData=292;   //292        //DEPT(4)+ TIME(4)+ SKG(256)+ BK79(32)+Tint(4)+4
     b1[0] = iLenFrameData & 0xff;
     b1[1] = (iLenFrameData >> 8) & 0xff;
     pTempData[17538] = b1[1];	//17525+13=17538 vi tri thu 13 tu 17525
     pTempData[17539] = b1[0];    //

     int ilenframRec=250;  //250    // nextlen DataspecRec 86+40+40+40+40+4;
     b1[0]=ilenframRec&0xff;
     b1[1]=(ilenframRec >> 8) & 0xff;
     pTempData[17521]=b1[1];
     pTempData[17522]=b1[0];

     int next=17771;  //
     b1[0]=next&0xff;
     b1[1]=(next >> 8) & 0xff;
     pTempData[17517]=b1[0];       // Khong đảo BIT vẫn chưa hiểu
     pTempData[17518]=b1[1];

     pTempData[17591]=0;     // Depth recording mode

     m_myFile.write(pTempData);
     m_myFile.flush();
     //qDebug()<<m_myFile.pos();
                                    //Mnemonic DEPT
    pTempData.resize(160);  //160
    pTempData[0] = 68;
    pTempData[1] = 69;
    pTempData[2] = 80;
    pTempData[3] = 84;

    //Service ID && Service Order
    for (int i = 4; i < 18; i++)
        pTempData[i] = 0;
    //Unit
    pTempData[18] = 77;    //(M)
    pTempData[19] = 0;
    pTempData[20] = 0;
    pTempData[21] = 0;

    //API code
    pTempData[22] = 0;
    pTempData[23] = 0;
    pTempData[24] = 0;
    pTempData[25] = 0;

    //FileNB
    pTempData[26] = 0;
    pTempData[27] = 0;

    //Size
    pTempData[28] = 0;
    pTempData[29] = 4;

    //nhay 3
    pTempData[30] = 0;
    pTempData[31] = 0;
    pTempData[32] = 0;

    //Sample
    pTempData[33] = 1;

    //Repcode
    pTempData[34] = 73;
    //Offset
    pTempData[35] = 0;

    //nhay 5
    //API code
    pTempData[36] = 0;
    pTempData[37] = 0;
    pTempData[38] = 0;
    pTempData[39] = 0;

   //m_myFile.write(pTempData);

                                    //Mnemonic TIME
   pTempData[40] = 84;
   pTempData[41] = 73;
   pTempData[42] = 77;
   pTempData[43] = 69;

   //Service ID && Service Order
   for (int i = 44; i < 58; i++)
       pTempData[i] = 0;
   //Unit
   pTempData[58] = 115;   //(s)
   pTempData[59] = 0;
   pTempData[60] = 0;
   pTempData[61] = 0;

   //API code
   pTempData[62] = 0;
   pTempData[63] = 0;
   pTempData[64] = 0;
   pTempData[65] = 0;

   //FileNB
   pTempData[66] = 0;
   pTempData[67] = 0;

   //Size
   pTempData[68] = 0;
   pTempData[69] = 4;

   //nhay 3
   pTempData[70] = 0;
   pTempData[71] = 0;
   pTempData[72] = 0;

   //Sample
   pTempData[73] = 1;

   //Repcode
   pTempData[74] = 73;
   //Offset
   pTempData[75] = 4;                //Offset 4

   //nhay 5
   //API code
   pTempData[76] = 0;
   pTempData[77] = 0;
   pTempData[78] = 0;
   pTempData[79] = 0;

                                    //SGK
   pTempData[80] = 83;
   pTempData[81] = 71;
   pTempData[82] = 75;
   pTempData[83] = 0;

   //Service ID && Service Order
   for (int i = 84; i < 98; i++)
       pTempData[i] = 0;
   //Unit
   pTempData[98] = 0;
   pTempData[99] = 0;
   pTempData[100] = 0;
   pTempData[101] = 0;

   //API code
   pTempData[102] = 0;
   pTempData[103] = 0;
   pTempData[104] = 0;
   pTempData[105] = 0;

   //FileNB
   pTempData[106] = 0;
   pTempData[107] = 0;

   //Size
   QByteArray b;
   b.resize(2);
   signed  short isize = 512;    //256
   //b = new BYTE[2];

   b[0] = isize & 0xff;
   b[1] = (isize >> 8) & 0xff;
   pTempData[108] = b[1];
   pTempData[109] = b[0];

   //bytes[1] = num & 0xFF00; // heigh byte
   //bytes[0] = num & 0x00FF; // low byte

   //nhay 3
   pTempData[110] = 0;
   pTempData[111] = 0;
   pTempData[112] = 0;

   //Sample
   pTempData[113] = 1;

   //Repcode
   pTempData[114] = 79;
   //Offset
   pTempData[115] = 8;

   //nhay 5
   pTempData[116] = 0;
   pTempData[117] = 0;
   pTempData[118] = 0;
   pTempData[119] = 0;

   //                           BK79

    pTempData[120] = 66;
    pTempData[121] = 75;
    pTempData[122] = 55;
    pTempData[123] = 57;

    //Service ID && Service Order
    for (int i = 124; i < 138; i++)
    pTempData[i] = 0;
    //Unit
    pTempData[138] = 0;
    pTempData[139] = 0;
    pTempData[140] = 0;
    pTempData[141] = 0;

    //API code
    pTempData[142] = 0;
    pTempData[143] = 0;
    pTempData[144] = 0;
    pTempData[145] = 0;

    //FileNB
    pTempData[146] = 0;
    pTempData[147] = 0;

    //Size
    isize = 64;                     //32
    b[0] = isize & 0xff;
    b[1] = (isize >> 8) & 0xff;
    pTempData[148] = b[1];
    pTempData[149] = b[0];

    //bytes[1] = num & 0xFF00; // heigh byte
    //bytes[0] = num & 0x00FF; // low byte

    //nhay 3
    pTempData[150] = 0;
    pTempData[151] = 0;
    pTempData[152] = 0;

    //Sample
    pTempData[153] = 1;

    //Repcode
    pTempData[154] = 79;
    //Offset
    pTempData[155] = 264;             //OFFSET

    //nhay 5
    pTempData[156] = 0;
    pTempData[157] = 0;
    pTempData[158] = 0;
    pTempData[159] = 0;


   m_myFile.write(pTempData);
   m_myFile.flush();
   //qDebug()<<m_myFile.pos();

}

void CLis::addCurve(QString TYPE, int RepCode, int Size,QString Unit)
{
    QByteArray		pTempData;
    //Mnemonic DEPT
    pTempData.resize(40);

    for(int i=0;i<4;i++)
        pTempData[i]=0;
    for(int i=0;i<TYPE.size();i++){
        pTempData[i] = QChar(TYPE.at(i)).toLatin1();
    }

    //Service ID && Service Order
    for (int i = 4; i < 18; i++)
    pTempData[i] = 0;

    //Unit
    for(int i=0;i<4;i++)
        pTempData[18+i]=0;
    for(int i=0;i<Unit.size();i++){
        pTempData[18+i] = QChar(Unit.at(i)).toLatin1();
    }  //(M)


    //API code
    pTempData[22] = 0;
    pTempData[23] = 0;
    pTempData[24] = 0;
    pTempData[25] = 0;

    //FileNB
    pTempData[26] = 0;
    pTempData[27] = 0;

    //Size
    pTempData[28] = 0;
    pTempData[29] = Size;

    //nhay 3
    pTempData[30] = 0;
    pTempData[31] = 0;
    pTempData[32] = 0;

    //Sample
    pTempData[33] = 1;

    //Repcode
    pTempData[34] = RepCode;
    //Offset
    pTempData[35] = m_offser;

    //nhay 5
    //API code
    pTempData[36] = 0;
    pTempData[37] = 0;
    pTempData[38] = 0;
    pTempData[39] = 0;

    m_offser=m_offser+Size;
    int t;
    t=m_myFile.write(pTempData);
    m_myFile.flush();

    //Cập nhật lại iLenFrameData,ilenframRec

    QByteArray b1,temp;
    QByteArray p1,p2;
    //int iLenFrameData;
    int ilenframRec;
    int iMaxFrame;     //vi tri thu 13 tu 17525
    p1.resize(2);
    p2.resize(2);


    if(m_NumberCurve==13)
        int t=0;
    m_iLenFrameData=m_iLenFrameData+Size;
    //iMaxFrame=m_iLenFrameData-6+4;  //6 là 2Header+4Depth         //TIME(4)+ SKG(256)+ BK79(32)+4
    iMaxFrame=42;
    b1[0] = iMaxFrame & 0xff;
    b1[1] = (iMaxFrame >> 8) & 0xff;
    p1[0] = b1[1];	//17525+13=17538 vi tri thu 13 tu 17525 (Maxium frame minus DEPT,HEADER)
    p1[1] = b1[0];    //
    m_myFile.seek(17538);
    m_myFile.write(p1);
    m_myFile.flush();

    m_ilenframRec=m_ilenframRec+40;
    ilenframRec=m_ilenframRec;      // nextlen DataspecRec 86+40+40+40+40+4;
    b1[0]=ilenframRec&0xff;
    b1[1]=(ilenframRec >> 8) & 0xff;
    p2[0]=b1[1];
    p2[1]=b1[0];
    m_myFile.seek(17521);
    m_myFile.write(p2);
    m_myFile.flush();
    m_NumberCurve++;

    //Cập nhật lại Next(lAddrFirstDataRec)
    int next=m_firstDataSpecBlock+(m_NumberCurve)*40;
    m_lAddrFisrtDataRec=next;

    b1[0]=next&0xff;
    b1[1]=(next >> 8) & 0xff;
    p1[0]=b1[0];       // Khong đảo BIT vẫn chưa hiểu
    p1[1]=b1[1];
    m_myFile.seek(17517);
    m_myFile.write(p1);
    m_myFile.flush();
    m_myFile.seek(m_myFile.size());  // Seek to end file


}

long CLis::addDataRec(long lCurPosition, uint32_t iCurDepth, int iMaxFrameData, uint32_t time, QByteArray r)
{

    long lAddrDataSpecRec =17509;// m_firstDataSpecBlock-16-86;//17509;
    int iNumberCurve = m_NumberCurve;

    long lAddrFisrtDataRec = lAddrDataSpecRec + 16 + 86 + iNumberCurve * 40;  //17891

    QByteArray		pTempData;
    pTempData.resize(m_iLenFrameData+2+4);

    QByteArray group1;
    QByteArray group2;
    QByteArray group3;
    QByteArray group4;
    group1.resize(4);
    group2.resize(4);
    group3.resize(4);
    group4.resize(4);

    for (int i = 0; i < 4; i++) {
        group1[i] = 0;
        group2[i] = 0;
        group3[i] = 0;
        group4[i] = 0;
    }

        //GHI DU LIEU VAO DATARECORD

    long lcurent = lCurPosition;
    int  ilendata = iMaxFrameData + 16 + 2 + 4;
    uint32_t curDepth = iCurDepth;
    long lprev;
    long lnext;
    long lnextlen;

    QByteArray bPtrPrev;
    QByteArray bPtrNext;
    QByteArray bPtrNextLen;
    bPtrPrev.resize(4);
    bPtrNext.resize(4);
    bPtrNextLen.resize(4);

    lnextlen = iMaxFrameData + 2 + 4 + 4;
    lnext = lcurent + ilendata;
    if (lcurent == lAddrFisrtDataRec)
        lprev = m_firstDataSpecBlock-16-86;
    else
        lprev = lcurent - ilendata;

    bPtrPrev = (char *)&lprev;
    bPtrNext = (char *)&lnext;
    bPtrNextLen = (char *)&lnextlen;

    for (int t = 0; t < 4; t++) {   // t<4

        group2[t] = bPtrPrev[t];
        group3[t] = bPtrNext[t];
        group4[t] = bPtrNextLen[t];
    }
    unsigned char bTemp;
    bTemp= group4[0];
    group4[0] = group4[1];
    group4[1] = bTemp;

    m_myFile.write(group1);m_myFile.flush();
    m_myFile.write(group2);m_myFile.flush();
    m_myFile.write(group3);m_myFile.flush();
    m_myFile.write(group4);m_myFile.flush();



    //DEPT
    char *bDEPT = (char *)&curDepth;

    //Header
    pTempData[0] = 0;
    pTempData[1] = 0;
    //m_myFile.write(pTempData,2);m_myFile.flush();

//    QByteArray pTempDepth;
//    pTempDepth.resize(4);
//    pTempDepth[1] = bDEPT[3];
//    pTempDepth[2] = bDEPT[2];
//    pTempDepth[3] = bDEPT[1];
//    pTempDepth[4] = bDEPT[0];

//    m_myFile.write(pTempDepth,2);m_myFile.flush();

    pTempData[2] = bDEPT[3];
    pTempData[3] = bDEPT[2];
    pTempData[4] = bDEPT[1];
    pTempData[5] = bDEPT[0];

    //TIME
    uint32_t dTime = time;
    char *bTime = (char*)&dTime;

    pTempData[6] = bTime[3];
    pTempData[7] = bTime[2];
    pTempData[8] = bTime[1];
    pTempData[9] = bTime[0];

    QByteArray temp;
    unsigned char  *b1;
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
        //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iPs&0xff;
    b1[1]=(iPs >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
        //luu vao file
    for (int i = 0; i < 2; i++)
        pTempData[10 + i] = temp[i];

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
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iIBK7&0xff;
    b1[1]=(iIBK7 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file
    for (int i = 0; i < 2; i++)
            pTempData[12 + i] = temp[i];



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
    temp=this->coverFloat2ArrayLis(fTint);
    //luu vao file
    for (int i = 0; i < 4; i++)
        pTempData[14 + i] = temp[i];

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
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iUBK7&0xff;
    b1[1]=(iUBK7 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file

    for (int i = 0; i < 2; i++)
        pTempData[18 + i] = temp[i];

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
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iIBK9&0xff;
    b1[1]=(iIBK9 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file

    for (int i = 0; i < 2; i++)
        pTempData[20 + i] = temp[i];

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
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iUBK9&0xff;
    b1[1]=(iUBK9 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file

    for (int i = 0; i < 2; i++)
        pTempData[22 + i] = temp[i];

      //****************Tinh RBK7  **********************

    //qint16 iRBK7=iUBK7/iIBK7;
    qint16 iRBK7=1;
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iRBK7&0xff;
    b1[1]=(iRBK7 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file

    for (int i = 0; i < 2; i++)
        pTempData[24 + i] = temp[i];

    //****************Tinh RBK9  **********************

  //qint16 iRBK9=iUBK9/iIBK9;
  qint16 iRBK9=1;
  //doi ra lai byte
  b1=new unsigned char(2);
  b1[0]=iRBK9&0xff;
  b1[1]=(iRBK9 >> 8) & 0xff;
  temp[1]=b1[0];
  temp[0]=b1[1];
  //luu vao file

  for (int i = 0; i < 2; i++)
      pTempData[26 + i] = temp[i];

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
    //doi ra lai byte
    QByteArray qba(reinterpret_cast<const char *>(&iThori), sizeof(unsigned int));

    for (int i = 0; i < 4; i++)
        pTempData[28 + i] = qba[3-i];

    //Tinh URAN
    unsigned int iUran;
    iUran=arrValueSGK[161]+arrValueSGK[172]+arrValueSGK[187];
    //doi ra lai byte
    QByteArray qba2(reinterpret_cast<const char *>(&iUran), sizeof(unsigned int));

    for (int i = 0; i < 4; i++)
        pTempData[32 + i] = qba2[3-i];

    //Tinh Kali
    unsigned int iKali;
    iKali=arrValueSGK[181];
    //doi ra lai byte
    QByteArray qba3(reinterpret_cast<const char *>(&iKali), sizeof(unsigned int));

    for (int i = 0; i < 4; i++)
        pTempData[36 + i] = qba3[3-i];

    //Tinh GK
    unsigned int iGK;
    iGK=iThori+iUran+iKali;
    //doi ra lai byte
    QByteArray qba4(reinterpret_cast<const char *>(&iGK), sizeof(unsigned int));

    for (int i = 0; i < 4; i++)
        pTempData[40 + i] = qba4[3-i];


//    QByteArray temp2;
//    for(int i=0;i<pTempData.size();i++)
//        temp2[i]=pTempData[i+2];

    m_myFile.write(pTempData);
    m_myFile.flush();

    return lcurent + ilendata;
}

long CLis::addDataRec2(long lCurPosition, uint32_t iCurDepth, int iMaxFrameData, uint32_t time, QByteArray r)
{
    long lAddrDataSpecRec =17509;// m_firstDataSpecBlock-16-86;//17509;
    int iNumberCurve = m_NumberCurve;

    long lAddrFisrtDataRec = lAddrDataSpecRec + 16 + 86 + iNumberCurve * 40;  //17891

    QByteArray		pTempData;
    pTempData.resize(m_iLenFrameData+2+4);

    QByteArray group1;
    QByteArray group2;
    QByteArray group3;
    QByteArray group4;
    group1.resize(4);
    group2.resize(4);
    group3.resize(4);
    group4.resize(4);

    for (int i = 0; i < 4; i++) {
        group1[i] = 0;
        group2[i] = 0;
        group3[i] = 0;
        group4[i] = 0;
    }

        //GHI DU LIEU VAO DATARECORD

    long lcurent = lCurPosition;
    int  ilendata = iMaxFrameData + 16 + 2 + 4;
    uint32_t curDepth = iCurDepth;
    long lprev;
    long lnext;
    long lnextlen;

    QByteArray bPtrPrev;
    QByteArray bPtrNext;
    QByteArray bPtrNextLen;
    bPtrPrev.resize(4);
    bPtrNext.resize(4);
    bPtrNextLen.resize(4);

    lnextlen = iMaxFrameData + 2 + 4 + 4;
    lnext = lcurent + ilendata;
    if (lcurent == lAddrFisrtDataRec)
        lprev = m_firstDataSpecBlock-16-86;
    else
        lprev = lcurent - ilendata;

    bPtrPrev = (char *)&lprev;
    bPtrNext = (char *)&lnext;
    bPtrNextLen = (char *)&lnextlen;

    for (int t = 0; t < 4; t++) {   // t<4

        group2[t] = bPtrPrev[t];
        group3[t] = bPtrNext[t];
        group4[t] = bPtrNextLen[t];
    }
    unsigned char bTemp;
    bTemp= group4[0];
    group4[0] = group4[1];
    group4[1] = bTemp;

    m_myFile.write(group1);m_myFile.flush();
    m_myFile.write(group2);m_myFile.flush();
    m_myFile.write(group3);m_myFile.flush();
    m_myFile.write(group4);m_myFile.flush();


    pTempData[0] = 0;
    pTempData[1] = 0;
    m_myFile.write(pTempData,2);m_myFile.flush();


    QByteArray Qta;
    char *bDEPT2 = (char *)&curDepth;
    Qta.resize(4);
    Qta[0] = bDEPT2[3];;
    Qta[1] = bDEPT2[2];;
    Qta[2] = bDEPT2[1];;
    Qta[3] =bDEPT2[0];;
    m_myFile.write(Qta);m_myFile.flush();

    //DEPT
    char *bDEPT = (char *)&curDepth;

    //Header
    pTempData[0] = 0;
    pTempData[1] = 0;

    pTempData[2] = bDEPT[3];
    pTempData[3] = bDEPT[2];
    pTempData[4] = bDEPT[1];
    pTempData[5] = bDEPT[0];

    //TIME
    uint32_t dTime = time;
    char *bTime = (char*)&dTime;

    pTempData[6] = bTime[3];
    pTempData[7] = bTime[2];
    pTempData[8] = bTime[1];
    pTempData[9] = bTime[0];

    QByteArray temp;
    unsigned char  *b1;
                            //****************Tinh PS  **********************

    qint16 iPs;
    iPs=1;
        //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iPs&0xff;
    b1[1]=(iPs >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
        //luu vao file
    for (int i = 0; i < 2; i++)
        pTempData[10 + i] = temp[i];

        //****************Tinh IBK7  **********************
    qint16 iIBK7;
    //xu ly
    iIBK7=1;
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iIBK7&0xff;
    b1[1]=(iIBK7 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file
    for (int i = 0; i < 2; i++)
            pTempData[12 + i] = temp[i];



        //****************Tinh Tint  **********************
    //lấy vi tri
    qint16 iTint;
  //xu ly
    float fTint=1;
    temp=this->coverFloat2ArrayLis(fTint);
    //luu vao file
    for (int i = 0; i < 4; i++)
        pTempData[14 + i] = temp[i];

        //****************Tinh UBK7  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[259*2];       //Đảo bit
    temp[0]=r[259*2+1];
    //doi ra so nguyen
    qint16 iUBK7;
    memcpy(&iUBK7,temp.data(), sizeof(qint16));
    //xu ly
    iUBK7=1;
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iUBK7&0xff;
    b1[1]=(iUBK7 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file

    for (int i = 0; i < 2; i++)
        pTempData[18 + i] = temp[i];

        //****************Tinh IBK9  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[258*2];       //Đảo bit
    temp[0]=r[258*2+1];
    //doi ra so nguyen
    qint16 iIBK9;
    memcpy(&iIBK9,temp.data(), sizeof(qint16));
    //xu ly
    iIBK9=4;
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iIBK9&0xff;
    b1[1]=(iIBK9 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file

    for (int i = 0; i < 2; i++)
        pTempData[20 + i] = temp[i];

        //****************Tinh UBK9  **********************
    //lấy vi tri
    temp.resize(2);
    temp[1]=r[260*2];       //Đảo bit
    temp[0]=r[260*2+1];
    //doi ra so nguyen
    qint16 iUBK9;
    memcpy(&iUBK9,temp.data(), sizeof(qint16));
    //xu ly
    iUBK9=4;
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iUBK9&0xff;
    b1[1]=(iUBK9 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file

    for (int i = 0; i < 2; i++)
        pTempData[22 + i] = temp[i];

      //****************Tinh RBK7  **********************

    //qint16 iRBK7=iUBK7/iIBK7;
    qint16 iRBK7=1;
    //doi ra lai byte
    b1=new unsigned char(2);
    b1[0]=iRBK7&0xff;
    b1[1]=(iRBK7 >> 8) & 0xff;
    temp[1]=b1[0];
    temp[0]=b1[1];
    //luu vao file

    for (int i = 0; i < 2; i++)
        pTempData[24 + i] = temp[i];

    //****************Tinh RBK9  **********************

  //qint16 iRBK9=iUBK9/iIBK9;
  qint16 iRBK9=4;
  //doi ra lai byte
  b1=new unsigned char(2);
  b1[0]=iRBK9&0xff;
  b1[1]=(iRBK9 >> 8) & 0xff;
  temp[1]=b1[0];
  temp[0]=b1[1];
  //luu vao file

  for (int i = 0; i < 2; i++)
      pTempData[26 + i] = temp[i];

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
    iThori=1;
    //doi ra lai byte
    QByteArray qba(reinterpret_cast<const char *>(&iThori), sizeof(unsigned int));

    for (int i = 0; i < 4; i++)
        pTempData[28 + i] = qba[3-i];

    //Tinh URAN
    unsigned int iUran;
    iUran=1;
    //doi ra lai byte
    QByteArray qba2(reinterpret_cast<const char *>(&iUran), sizeof(unsigned int));

    for (int i = 0; i < 4; i++)
        pTempData[32 + i] = qba2[3-i];

    //Tinh Kali
    unsigned int iKali;
    iKali=1;
    //doi ra lai byte
    QByteArray qba3(reinterpret_cast<const char *>(&iKali), sizeof(unsigned int));

    for (int i = 0; i < 4; i++)
        pTempData[36 + i] = qba3[3-i];

    //Tinh GK
    unsigned int iGK;
    iGK=1;
    //doi ra lai byte
    QByteArray qba4(reinterpret_cast<const char *>(&iGK), sizeof(unsigned int));

    for (int i = 0; i < 4; i++)
        pTempData[40 + i] = qba4[3-i];

    QByteArray tem;
    tem.resize(44);
    for(int i=0;i<44;i++)
        tem[i]=pTempData[i+2];

    m_myFile.write(tem);
    m_myFile.flush();

    return lcurent + ilendata;
}

QByteArray CLis::bitsToBytes(QBitArray bits)            // chuyen 8 bit thanh 1Byte
{
    QByteArray bytes;
       bytes.resize(bits.count() / 8 + ((bits.count() % 8)? 1: 0));
       bytes.fill(0x00);
       // Convert from QBitArray to QByteArray
       for(int b = 0; b < bits.count(); ++b)
           bytes[b / 8] = (bytes.at(b / 8) | ((bits[b]? 1: 0) << (b % 8)));
       return bytes;
}

QBitArray CLis::quint8ToQBitArray(quint8 v)
{
    QBitArray ba(8);
        for (int i=0; i<8; i++)
            ba.setBit(7-i, v>>i & 1);
        return ba;
}

QByteArray CLis::coverFloat2ArrayLis(float fval)
{
    //float fval;
    //fval=200.5;
    int n=0;
    for(int i=0;i<255;i++)
        if(fval/pow(2,i)>0.5)
            n++;
         else
            break;
    float tempS=fval/pow(2,n);
    int iE;
    if(fval>0)
        iE=n+128;
    QBitArray arrBitE;
    QBitArray arrBitF;

    arrBitE=this->quint8ToQBitArray(iE);

    arrBitF.resize(23);
    float s=0;
    for(int i=1;i<=23;i++)
        if(s+1/pow(2,i)<tempS){
            s=s+1/pow(2,i);
            arrBitF[i-1]=true;
        }
        else
            arrBitF[i-1]=false;

     QBitArray arrBitFinal;
     arrBitFinal.resize(32);
     arrBitFinal[0]=false;
     for(int i=1;i<9;i++)
         arrBitFinal[i]=arrBitE[i-1];
     for(int i=9;i<32;i++)
         arrBitFinal[i]=arrBitF[i-9];

     //Chuyen arrBitFinal to 4byte
     QByteArray arrWrite;
     //arrWrite.resize(4);
     QBitArray arrTemp(8);
     for(int y=0;y<4;y++){
         for(int i=0;i<8;i++)
             arrTemp[7-i]=arrBitFinal[i+y*8];
         arrWrite.append(this->bitsToBytes(arrTemp));
     }
     return arrWrite;
}


