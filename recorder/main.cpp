#include "main.h"
#include "farm_login.h"

char serverIp[20] = "127.0.0.1";
const int DEFAULT_PORT = 8000;
SOCKADDR_IN addrSrv;
char userName[20];
MainWindow *mw = NULL;
int main(int argc, char *argv[])
{

    int err, iLen;
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested	=	MAKEWORD(2,2);
    err = WSAStartup(wVersionRequested,&wsaData);               //初始化SOCKET
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//设置服务器ip地址
    addrSrv.sin_port = htons(DEFAULT_PORT);
    if(err != 0){
        cout<<"Load WinSock Failed!";
    }
    QApplication a(argc, argv);
    farm_login fm;
    fm.show();
    return a.exec();
}
