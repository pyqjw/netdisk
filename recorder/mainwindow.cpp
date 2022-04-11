#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <QTextCodec>
#include <QInputDialog>
#include <main.h>

void MainWindow::init() {
    ui->frate->setText(QString("50"));
    ui->LeftTopX->setText(QString("0"));
    ui->LeftTopY->setText(QString("0"));
    ui->width->setText(QString("1920"));
    ui->height->setText(QString("1080"));
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));
    ui->setupUi(this);
    init();
    path = userName;
    path += '/';
    vpaths.push_back(new QString(userName));
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    layout = new QVBoxLayout();
    ui->scrollAreaWidgetContents->setLayout(layout);
    QString tmp = QString::fromUtf8(userName);
    ui->nameLabel->setText("用户:" + tmp);
    getFiles();
    ui->currentPath->setText(path);
}
void MainWindow::openFolder(int index) {
    vpaths.push_back(new QString(vfiles[index]->fname));

    path.append(vfiles[index]->fname);
    path.append("/");
    getFiles();
}

void MainWindow::deleteFileOrFolder(int index)
{
    char buffer[1024];


    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    ::connect(client,(SOCKADDR*)&addrSrv, sizeof(addrSrv));
    int order = IDELETE;
    send(client, (char*)&order, sizeof(int), 0);

    strcpy(buffer, path.toLocal8Bit().constData());
    strcat(buffer, vfiles[index]->fname.toLocal8Bit().constData());
    int isDir;

    if(vfiles[index]->isDir) {
        isDir = 1;
    } else isDir = 0;
    send(client, (char*)&isDir, sizeof(int), 0);
    send(client, buffer, 1024, 0);
    recv(client, buffer, sizeof(int), 0);
    getFiles();
}

void MainWindow::downloadFile(int index)
{
    char buffer[1024];
    QString savePath = QFileDialog::getSaveFileName(this, tr("Open Directory"),
                                "",
                                 "*"
                                  );
    if(savePath == "") {
        return ;
    }
    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    ::connect(client,(SOCKADDR*)&addrSrv, sizeof(addrSrv));
    int order = IDOWNLOAD;
    send(client, (char*)&order, sizeof(int), 0);
    strcpy(buffer, path.toLocal8Bit().constData());
    strcat(buffer, "/");
    strcat(buffer, vfiles[index]->fname.toLocal8Bit().constData());
    send(client, buffer, 1024, 0);
    int size;
    FILE *file = fopen(savePath.toLocal8Bit().constData(), "wb");
    recv(client, (char*)&size, sizeof(int), 0);
    while(size) {
        int xsize;
        if(size >= 1024) {
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
    closesocket(client);
}

void MainWindow::renameFile(int index)
{
    char buffer[1024];
    bool isOK;
    QString text = QInputDialog::getText(NULL, "重命名",
                                                       "请输入文件名字",
                                                       QLineEdit::Normal,
                                                       vfiles[index]->fname,
                                                       &isOK);
    if(isOK) {

    } else {
        return ;
    }
    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    ::connect(client,(SOCKADDR*)&addrSrv, sizeof(addrSrv));
    int order = IRENAME;
    send(client, (char*)&order, sizeof(int), 0);


    strcpy(buffer, path.toLocal8Bit().constData());
    strcat(buffer, vfiles[index]->fname.toLocal8Bit().constData());
    send(client, buffer, 1024, 0);

    strcpy(buffer, path.toLocal8Bit().constData());

    strcat(buffer, text.toLocal8Bit().constData());

    send(client, buffer, 1024, 0);

    recv(client, buffer, sizeof(int), 0);
    getFiles();
}
void MainWindow::getFiles()
{

    vfiles.clear();
    for(int i = layout->count()-1; i >= 0; i--) {
        delete layout->itemAt(i)->widget();
    }
    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    ::connect(client,(SOCKADDR*)&addrSrv, sizeof(addrSrv));
    int order = ISEARCH;
    send(client, (char*)&order, sizeof(int), 0);
    send(client, path.toLocal8Bit(), 1024, 0);
    int fnum;
    recv(client, (char*)&fnum, sizeof(int), 0);
    for(int i = 0; i < fnum; i++) {
        struct sendd
        {
            _fsize_t fsize;
            char fname[_MAX_FNAME];
            int type;
        }file;
        recv(client, (char*)&file, sizeof(sendd), 0);
        Form *form = new Form();
        form->Init(QString::fromLocal8Bit(file.fname), file.type, i);
        vfiles.push_back(form);
        layout->addWidget(form);
    }
    ui->currentPath->setText(path);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::start_rec() {
    this->showMinimized();   //窗口最小化
    Sleep(250);
    char tmpfnbuf[MAX_PATHLEN*4];
    QString qs = ui->path->text();
    sprintf(tmpfnbuf, "%s", qs.toStdString().c_str());
    if (strcmp(tmpfnbuf, "") == 0) {
        time(&rawtime);
        strftime(outputname, 500, "/Records/Record_%Y-%m-%d_%H-%M-%S.mpg", localtime(&rawtime));
        ui->path->setText(outputname);
    } else {
        sprintf(outputname, "%s", tmpfnbuf);
    }

    QRect re = this->rect();
    int w = re.right(), h = re.bottom();
    string xx, yy, ww, hh, frate;
    xx = ui->LeftTopX->text().toStdString();
    yy = ui->LeftTopY->text().toStdString();
    ww = ui->width->text().toStdString();
    hh = ui->height->text().toStdString();
    frate = ui->frate->text().toStdString();
    string exe = "ffmpeg.exe -f gdigrab  -framerate ";
    exe += frate;
    exe += " -offset_x ";
    exe += xx;//获取的左上角x
    exe += " -offset_y ";
    exe += yy;//获取的左上角y
    exe += " -video_size ";
    exe += ww;
    exe += "x";
    exe += hh;
    exe += " -i desktop -b:v 19500k \"";
    exe += outputname;
    exe += "\"";
    cout << exe << endl;
    //system(exe.c_str());//会出现控制台
    STARTUPINFO si2 = { sizeof(si2) };
    PROCESS_INFORMATION  pi2;
    si2.wShowWindow = FALSE;
    si2.dwFlags = STARTF_USESHOWWINDOW;
    char* execstr = (char*)exe.c_str();
    clockstartt = clock();
    LPWSTR lpStr = (LPWSTR) QString(execstr).toStdWString().c_str();
    QProcess *qp = new QProcess();
    qp->start(QString::fromStdString(exe));
    return 0;
}
void MainWindow::on_action_recorder_triggered() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_action_disk_triggered() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_action_about_triggered()
{
    char aboutbuf[2000];
    sprintf(aboutbuf, "tyzq录屏软件（软工大作业），一个录屏软件，由C++编写。\n版本：1.0.0.0 语言：汉语（中国）\n"
"作者（排名不分先后，均为第一作者）：齐敬文、张泽坤、闫秀泽、汤云灿。如有任何意见或建议，请联系作者邮箱: tyc18265475128@163.com。"
"\n\n约束条件: 路径中不可带有空格，尽量不要有中文，否则可能无法录制。输入框是为您填入保存位置用的，若留空，则默认保存至Records文件夹，"
"以开始拍摄的时间命名。您也可以通过“选择保存路径”来选择位置。具体保存位置由开始拍摄前指定的为准。拍摄一开始，就会转最小化，您可以从任务栏"
"唤醒本软件来结束录制。由于制作者能力有限，仅支持mpg与avi两种格式。您可以通过EFChange来转换为您想要的格式。若您是通过强制方式关闭的本进"
"程，请务必运行本文件夹下的clean.bat批处理文件来清理进程，否则可能导致内存大量消耗！\n 更多功能敬请期待tyzq录屏软件（软工大作业）版本2"
".0.0.0！！！！");

    QMessageBox message(QMessageBox::Information,"Information",

                                                     aboutbuf,QMessageBox::Yes|QMessageBox::No,NULL);
    message.exec();
//       {
//          // qDebug()<<"clicked yes\n";
//       }
//       else
//       {
//           //qDebug()<<"clicked no\n";
//       }
}


void MainWindow::on_recordButton_clicked()
{
    if(isrecording == 0) {
        isrecording = 1;
        ui->recordButton->setText("结束录制");
        start_rec();
    } else {
        string s=("taskkill /F /im ffmpeg.exe");
        char* szCommandLine = (char*)s.c_str();
        QProcess *qp = new QProcess();
        qp->start(QString::fromStdString(s));
        isrecording=0;
        Sleep(500);
        ui->recordButton->setText("开始录制");
        char recordsuccess[200];
        sprintf(recordsuccess, "录制成功\n录制文件已存储至%s", outputname);
        ui->path->setText("");
        QMessageBox message(QMessageBox::Information,"Information",
                                                                recordsuccess,QMessageBox::Yes|QMessageBox::No,NULL);
        message.exec();
    }

}


void MainWindow::on_MainWindow_destroyed()
{
    char szCommandLine2[] = ("taskkill /F /im ffmpeg.exe");
    QProcess *qp = new QProcess();
    qp->start(QString::fromUtf8(szCommandLine2));
    Sleep(200);
}
int xx = 0;

void MainWindow::on_savePath_clicked()
{
    recPath = QFileDialog::getSaveFileName(this, tr("Open Directory"),
                                       "",
                                    "*"
                                      );
    ui->path->setText(recPath);
}


void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_uploadButton_clicked()
{
    QString name = QFileDialog::getOpenFileName(this,"选择文件", ".", "文件(*.*)",NULL,  0);
    char sendbuffer[2048];
    char tmp[2048];
    strcpy(sendbuffer, path.toLocal8Bit().constData());
    strcpy(tmp, name.toLocal8Bit().constData());
    FILE *file = fopen(tmp, "rb");
    for(int i = name.size()-1; i >= 0; i--){
        if(tmp[i] == '\\' || tmp[i] == '/') {
            strcat(sendbuffer, tmp+i);
            break;
        }
    }
    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    ::connect(client,(SOCKADDR*)&addrSrv, sizeof(addrSrv));
    int order = IUPLOAD;
    send(client, (char*)&order, sizeof(int), 0);
    send(client, sendbuffer, 1024, 0);
    int size = filelength(fileno(file));
    send(client, (char*)&size, sizeof(int), 0);
    while(size) {
        int xsize;
        if(size >= 1024) {  // 分段 每段最大为1024字节
            xsize = 1024;
            size -= 1024;
        } else {
            xsize = size;
            size = 0;
        }
        fread(sendbuffer, 1, xsize, file);
        send(client, sendbuffer, xsize, 0);
    }
    fclose(file);
    closesocket(client);
    getFiles();
}


void MainWindow::on_newDir_clicked()
{
    char buff[1024];
    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    ::connect(client,(SOCKADDR*)&addrSrv, sizeof(addrSrv));
    int order = INEWFOLDER;
    strcpy(buff, path.toLocal8Bit().constData());
    send(client, (char*)&order, sizeof(int), 0);
    send(client, buff, 1024, 0);
    getFiles();
}


void MainWindow::on_fresh_clicked()
{
    getFiles();
}


void MainWindow::on_upperFolder_clicked()
{
    QString newPath;
    if(vpaths.size() == 1) return ;
    delete vpaths.back();
    vpaths.pop_back();
    for(int i = 0; i < vpaths.size(); i++){
        newPath.append(vpaths[i]);
        newPath.append("/");
    }
    path = newPath;

    getFiles();
}

