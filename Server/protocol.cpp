#include"protocol.h"
#include<cstdlib>
#include<cstring>
#include<QDebug>
PDU* mkPDU(unsigned int uintMsgLen)
{
    unsigned int uintTotalLen = uintMsgLen + sizeof(PDU);
      PDU* pdu = (PDU*)malloc(uintTotalLen);
      if(pdu==NULL)
      {
          qDebug()<<"申请内存失败";
          exit(1);//进程杀掉
      }
      memset(pdu,0,uintTotalLen);
      pdu->uintMsgLen=uintMsgLen;
      pdu->uintTotalLen=uintTotalLen;

      return pdu;
}
void printPDU(PDU* pdu)
{
    qDebug()<<"pdu->uintTotalLen"<<pdu->uintTotalLen<<"\n"
            <<"pdu->uintMsgLen"<<pdu->uintMsgLen<<"\n"
            <<"pdu->uintType"<<pdu->uintType<<"\n"
            <<"pdu->caData"<<pdu->caData<<"\n"
            <<"pdu->caMsg"<<pdu->caMsg<<"\n";
}
