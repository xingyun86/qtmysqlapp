#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qdebug.h>
#include <qmessagebox.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlQuery query;

    //打印Qt支持的数据库驱动
    qDebug() << QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setConnectOptions();//"SSL_KEY=client-key.pem;SSL_CERT=client-cert.pem;SSL_CA=ca-cert.pem;CLIENT_IGNORE_SPACE=1");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("testdb");
    db.setUserName("test");
    db.setPassword("123456");
    if (!db.open())
    {
        qDebug()<<"open failed!" << db.lastError().text() << "\n";
        printf("open failed!\n");
        QMessageBox::warning(this,
                           QString::fromLocal8Bit("Message"),
                           db.lastError().text(),
                           QMessageBox::Ok,
                           QMessageBox::Ok);
    }
    else
    {
        qDebug()<<"open success!";
        printf("open success!\n");
        QMessageBox::information(this,
                           QString::fromLocal8Bit("Message"),
                           QString::fromLocal8Bit("open success"),
                           QMessageBox::Ok,
                           QMessageBox::Ok);
        //创建表
        query.exec("create table student(id int primary key auto_increment, name varchar(255), age int, score int);");

        //插入
        query.exec("insert into student(id, name, age, score) values(1, 'xiaoMing', 18, 59);");

        //批量插入
        //oracle风格
        //占位符 ： + 自定义名字
        query.prepare("insert into student(name, age, score) values(:name, :age, :score)");
        //给字段设置内容 list
        QVariantList nameList;
        nameList << "xiaod" << "xiaoe" << "xiaof";
        QVariantList ageList;
        ageList << 13 << 16 << 45;
        QVariantList scoreList;
        scoreList << 85 << 16 << 66;
        //给字段绑定
        query.bindValue(":name",nameList);
        query.bindValue(":age",ageList);
        query.bindValue(":score",scoreList);

        //执行预处理命令
        if(!query.execBatch()){
           qDebug() << query.lastError();
        }

       //遍历数据库
       query.exec("select * from student");
       while(query.next()){
           //一行一行遍历
           qDebug() << query.value(0).toInt()
                    << query.value(1).toString()
                    << query.value("age").toInt()
                    << query.value("score").toInt();
       }
       db.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

