#include <QCoreApplication>
#include <string>
#include <bits/stdc++.h>
#include <winsock2.h>
#include <windows.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <io.h>
#include <cstdio>
using namespace std;


#define LOGIN_FUC 1
#define REGIST_FUC 2
#define ISEARCH 3
#define IUPLOAD 4
#define IDELETE 5
#define IDOWNLOAD 6
#define INEWFOLDER 7
#define IRENAME 8

struct sendd
{
    _fsize_t fsize;
    char fname[_MAX_FNAME];
    int type;
};
const int DEFAULT_PORT = 8000;
SOCKADDR_IN addrSrv;
QSqlDatabase db;
void serve(SOCKET client);
void isearch(SOCKET ii);
void iupload(SOCKET ii);
void idelete(SOCKET ii);

void idownload(SOCKET ii);
void irename(SOCKET client);
void inewfolder(SOCKET client);
void regist_fuc(SOCKET client);
void login_fuc(SOCKET client);

void getFiles(const string &path, vector<sendd> &files);
string abpath = "C:/myApp/";
int main(int argc, char *argv[])
{
//    int ret = remove("D:/delete/");
//    int errr = GetLastError();
//    cout << errr << endl;
//    cout << EIO <<" " << ELOOP;
    int err, iLen;
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested	=	MAKEWORD(2,2);
    WSAStartup(wVersionRequested,&wsaData);
    SOCKET serSOCK = socket(AF_INET, SOCK_STREAM, 0);

    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);//Auto IP, byte sequence change
    addrSrv.sin_port = htons(DEFAULT_PORT);
    err = bind(serSOCK,(SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    if(err != 0) {
        qDebug() << "bind() failed" << WSAGetLastError() << endl;
    }
    err = listen(serSOCK, 10);
    if(err != 0) {
        qDebug() << "listen() failed" << WSAGetLastError() << endl;
    }
    db = QSqlDatabase::addDatabase("QODBC");//数据库驱动类型位sqlserver
    QString dsn = QString::fromLocal8Bit("test");
    db.setHostName("localhost");        //选择本地主机，或则127.0.0.1
    db.setDatabaseName(dsn);
       // db.setDatabaseName("qtsql");       //设置数据库名称
    db.setUserName("pyqjw");       //登录用户
    db.setPassword("123456");     //密码
    if(db.open()) {
        char *ch = (char*)QString("数据库打开成功").toLocal8Bit().constData();
        cout << ch << endl;
    } else {
        char *ch = (char*)QString("数据库打开失败").toLocal8Bit().constData();
        cout << ch << endl;
    }
    while(true) {
        SOCKADDR_IN addrClt;
        int len = sizeof(SOCKADDR_IN);
        SOCKET clientSOCK = accept(serSOCK, (SOCKADDR*)&addrClt, &len);
        thread *thd = new thread(serve, clientSOCK);
        thd->detach();
    }
    return 0;
}

void login_fuc(SOCKET client) {
    char *nameee;
    char * pwddd;
    int checkkk;
    struct LoginStr {
        char username[32];
        char password[32];
        int check;
    }loginstr;
    recv(client, (char*)&loginstr, sizeof(LoginStr), 0);
    nameee = loginstr.username;
    pwddd = loginstr.password;
    checkkk = loginstr.check;
    QString name = nameee;
    QString pwd = pwddd;
    int check=checkkk;
    //服务器端处理
    QString namee = "";
    QString pwdd = "";
    QSqlQuery query(db);
    query.exec("use teaching select * from [user] where username = '"+name+"'");
    int flag1=0;
    int flag2=0;
    while(query.next())
    {
       namee=query.value(0).toString();
       pwdd=query.value(1).toString();
       if(namee==name)
       {
          flag1=1;
          if(pwd==pwdd)
          {
              flag2=1;
          }
          break;
       }
     }
     //往客户端传flag1和flag2的两个值；
     int flags[3];
     flags[0] = flag1;
     flags[1] = flag2;
     send(client, (char*)flags, 2*sizeof(int), 0);
     mkdir((abpath + loginstr.username).c_str());
}

void regist_fuc(SOCKET client) {
    char *namee;
    char *pwdd;
    cout << "registing" << endl;
    struct RegistStr {
        char username[32];
        char password[32];
    }registstr;
    recv(client, (char*)&registstr, sizeof(RegistStr), 0);
    QString name=QString::fromUtf8(registstr.username);
    QString pwd=QString::fromUtf8(registstr.password);
    QSqlQuery query(db);
    query.exec("use teaching select * from [user] where username = '"+name+"'");
    int flag;
    if(query.next())
    {
        flag = 0;
    }
    else
    {
        query.exec("insert into [user] values('"+name+"','"+pwd+"')");
        flag = 1;
    }
    send(client, (char*)&flag, sizeof(int), 0);
}
void serve(SOCKET client) {
    int order;
    recv(client, (char*)&order, sizeof(int), 0);
    cout << order << endl;
    switch(order) {
    case LOGIN_FUC:
        login_fuc(client);
        break;
    case REGIST_FUC:
        regist_fuc(client);
        break;
    case ISEARCH:
        isearch(client);
        break;
    case IUPLOAD:
        iupload(client);
        break;
    case IDELETE:
        idelete(client);
        break;
    case IDOWNLOAD:
        idownload(client);
        break;
    case INEWFOLDER:
        inewfolder(client);
        break;
    case IRENAME:
        irename(client);
        break;
    }
    closesocket(client);
}
void isearch(SOCKET ii)
{
    const int bufferSize = 10010;
    char buffer[bufferSize] = {0};
    char buffer2[bufferSize] = {0};
    int readLen = 0, readLen2 = 0;
    readLen2 = recv(ii,buffer2,1024,0);
    string path(buffer2);
    path = abpath+path;
    cout << path << endl;
    vector<sendd> files;
    getFiles(path,files);
    int len = files.size();
    readLen = send(ii,(char*)&len, sizeof(int),0);//发送文件夹中有几个文件
    for(vector<sendd>::iterator it = files.begin();it != files.end();it++)
    {
        sendd a = (*it);
        send(ii,(char*)(&a),sizeof(sendd),0);//发送显示文件内容的结构体
    }
}
void idelete(SOCKET client)
{
    const int bufferSize = 1024;
    char buffer[bufferSize] = {0};
    int readLen = 0;
    int isDir;
    recv(client, (char*)&isDir, sizeof(int), 0);
    readLen = recv(client ,buffer,1024,0);

    string s = abpath;
    s += buffer;
    cout << s << endl;

    if(isDir) {
        rmdir(s.c_str());
    } else {
        remove(s.c_str());
    }

    int state = 1;
    send(client, (char*)&state, sizeof(int), 0);
}
void inewfolder(SOCKET client)
{
    const int bufferSize = 1024;
    char buffer2[bufferSize] = {0};
    recv(client,buffer2,1024,0);
    string path(buffer2);
    path = abpath + path;
    vector<sendd> files;
    getFiles(path,files);
    int len = files.size();
    int dir = 1;
    bool flag = true
    ;QString qs("新建文件夹");
    while(true)
    {
        if(dir > len)
            break;

        string jud(qs.toLocal8Bit().constData());
        if(dir != 1)
        {
            jud = jud+to_string(dir);
        }
        flag = true;
        for(vector<sendd>::iterator it = files.begin();it != files.end();it++)
        {
            string iit((*it).fname);
            if(iit == jud)
            {
                dir++;
                flag = false;
                break;
            }
        }
        if(flag != false)
        {
            break;
        }
    }

    if(dir == 1)
    {
        path = path+qs.toLocal8Bit().constData();
    }
    else
    {
        path = path+qs.toLocal8Bit().constData()+to_string(dir);
    }
    mkdir(path.c_str());   // 返回 0 表示创建成功，-1 表示失败
}
void iupload(SOCKET client)
{
    const int bufferSize = 1024;
    char buffer[bufferSize] = {0}, buffer2[bufferSize] = {0}, isize[bufferSize] = {0};
    int readLen = 0;
    recv(client,buffer,1024,0);
    string path(buffer);
    path = abpath+path;
    int size;
    readLen = recv(client,(char*)&size,sizeof(int),0);//接收文件大小
    FILE *file = fopen(path.c_str(), "wb+");
    while(size) {
        int xsize;
        if(size >= 1024) {   //分段 每段最大为1024字节
            xsize = 1024;
            size -= 1024;
        } else {
            xsize = size;
            size = 0;
        }
        recv(client, buffer, xsize, 0);
        fwrite(buffer, 1, xsize, file);
    }
    fclose(file);
}
void idownload(SOCKET client)
{
    const int bufferSize = 1024;
    char buffer[bufferSize] = {0}, selen[bufferSize] = {0};
    int readLen = 0;
    readLen = recv(client,buffer,bufferSize,0);//接收将要下载的路径
    string realpath = abpath+buffer;
    FILE *file = fopen(realpath.c_str(), "rb");
    int size = filelength(fileno(file));
    send(client, (char*)&size, sizeof(int), 0);
    while(size) {
        int xsize;
        if(size >= 1024) {
            xsize = 1024;
            size -= 1024;
        } else {
            xsize = size;
            size = 0;
        }
        fread(buffer, 1, xsize, file);
        send(client, buffer, xsize, 0);

    }
    fclose(file);
}
void getFiles(const string &path, vector<sendd> &files)
{
    long hFile = 0;
    _finddata_t fileinfo;
    string p;
    if((hFile = _findfirst(p.assign(path).append("*").c_str(),&fileinfo)) != -1)
    {
        do
        {
            if(fileinfo.attrib&_A_SUBDIR)
            {
                if(strcmp(fileinfo.name,".") != 0&& strcmp(fileinfo.name,"..") != 0)
                {
                    sendd skt;
                    skt.fsize = fileinfo.size;
                    skt.type = 1;
                    strcpy(skt.fname,fileinfo.name);
                    files.push_back(skt);
                }
            }
            else
            {
                sendd skt;
                skt.fsize = fileinfo.size;
                skt.type = 0;
                strcpy(skt.fname,fileinfo.name);
                files.push_back(skt);
            }
        }while(_findnext(hFile,&fileinfo) == 0);
        _findclose(hFile);
    }
}
void irename(SOCKET client) {
    char buffer[1024];
    recv(client, buffer, 1024, 0);
    char oldname[1024];
    strcpy(oldname, abpath.c_str());
    strcat(oldname, buffer);
    char newname[1024];
    recv(client, buffer, 1024, 0);
    strcpy(newname, abpath.c_str());
    strcat(newname, buffer);

    rename(oldname, newname);
    int ok = 1;
    send(client, (char*)&ok, sizeof(int), 0);
}
