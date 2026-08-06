#ifndef PROTOCOL_H
#define PROTOCOL_H



enum ENUM_MSG_TYPE
{
    NONE = 0,
    REQ_LOGIN,
    RECV_LOGIN,
    REQ_REGISTER,
    RECV_REGISTER,
    REQ_FIND_USERNAME,
    RECV_FIND_USERNAME,
    REQ_ADD_FRIEND,
    RECV_ADD_FRIEND,
    REQ_ADD_FRIEND_AGREE,
    RECV_ADD_FRIEND_AGREE,
    REQ_FLUSH_FRIEND,
    RECV_FLUSH_FRIEND,
    REQ_DELETE_FRIEND,
    RECV_DELETE_FRIEND,
    REQ_CHAT,
    RECV_CHAT,
    REQ_MKDIR,
    RECV_MKDIR,
    RECV_UPLOAD_FILE_DATA,
    REQ_FLUSH_FILE,
};
enum ENUM_FIND_USER_RESULT
{
    FIND_USER_FAIL_SQL_ERR = -3,
    FIND_USER_FAIL_NAME_ERR = -2,
    FIND_USER_FAIL_NOT_FOUND = -1,
    FIND_USER_SUCCESS_OFFLINE = 0,
    FIND_USER_SUCCESS_ONLINE = 1,
};
enum ENUM_ADD_FRIEND_RESULT
{
    ADD_FRIEND_FAIL_SQL_ERR = -3,
    ADD_FRIEND_FAIL_NAME_ERR = -2,
    ADD_FRIEND_FAIL_NOT_FOUND = -1,
    ADD_FRIEND_FAIL_OFFLINE = 0,
    ADD_FRIEND_OK = 1,

};
struct PDU // protocol data unit 协议数据单元
{
    unsigned int uintTotalLen; // 协议总长度
    unsigned int uintMsgLen; // 柔性数组的长度
    unsigned int uintType; // 协议类型
    char caData[64]; // 固定长度消息（用以减少内存碎片）
    char caMsg[]; // 实际消息
};
PDU* mkPDU(unsigned int uintMsgLen = 0);
void printPDU(PDU* pdu);

#endif // PROTOCOL_H
