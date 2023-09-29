#include "cdxt.h"
#include "QDebug"

CDxt::CDxt()
{

}

CDxt::CDxt(QString sfileName)
{
    m_file.setFileName(sfileName);
    m_file.setFileName("data.dxt");
    if (!m_file.open(QIODevice::ReadOnly|QIODevice::Text)){
       qDebug()<<"Ko mo duoc file DXT";
       return;
    }
    m_file.seek(562);      // Bỏ đoạn đầu
    //qDebug()<<m_file.readLine();


}


QString CDxt::readLine()
{
    return m_file.readLine();
}

QByteArray CDxt::readArrayInLine()
{
    QByteArray r;
    r.resize(584);
    QString s;
    s=m_file.readLine();
    QStringList  fields = s.split(" ");
    int n=0;
    for(int i=2;i<294;i++)   // bỏ qua 0,1,và 294(\n)
    {
        r[n]=(fields.at(i).toInt() >> 8) & 0xff;
        r[n+1]=fields.at(i).toInt()&0xff;
        n=n+2;
    }
    return r;
}



