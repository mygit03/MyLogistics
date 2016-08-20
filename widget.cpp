#include "widget.h"
#include "ui_widget.h"

#include <QSqlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QScrollBar>
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    operateSql();

    manager = new QNetworkAccessManager;

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::operateSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if (!db.open()){
        qDebug() << "database open fail!";
    }else{
        qDebug() << "database open success!";

        createTable();

        QStringList typeList;
        QSqlQuery qry;
        qry.exec(QString("select * from information"));
        while(qry.next()){
            QString type = qry.value(0).toString();
            qDebug() << "type:" << type;
            typeList.append(type);
        }
        ui->comboBox->addItems(typeList);
    }
}

void Widget::createTable()
{
    QSqlQuery q;
    q.prepare("CREATE TABLE if not exists information (type VARCHAR PRIMARY KEY,postid VARCHAR)");
    if(!q.exec()){
        qDebug()<<"creater table error";
    }

    q.exec("insert into information values('申通', 'shentong')");
    q.exec("insert into information values('EMS', 'ems')");
    q.exec("insert into information values('顺丰', 'shunfeng')");
    q.exec("insert into information values('圆通', 'yuantong')");
    q.exec("insert into information values('中通', 'zhongtong')");
    q.exec("insert into information values('韵达', 'yunda')");
    q.exec("insert into information values('天天', 'tiantian')");
    q.exec("insert into information values('汇通', 'huitongkuaidi')");
    q.exec("insert into information values('全峰', 'quanfengkuaidi')");
    q.exec("insert into information values('德邦', 'debangwuliu')");
    q.exec("insert into information values('宅急送', 'zhaijisong')");
}

void Widget::replyFinished(QNetworkReply *reply)
{
    ui->textBrowser->clear();

    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    qDebug() << "data:" << data;
    int value = data.value("status").toString().toInt();
    qDebug() << "value:" << value;
    if(value != 1){
        ui->textBrowser->setTextColor(Qt::red);
        ui->textBrowser->append(QString(":( 该单号暂无物流进展，请稍后再试，或检查公司和单号是否有误。"));
        return;
    }
    QJsonArray array = data.value("data").toArray();
    int size = array.size();
    for(int i = 0; i < size; i++){
        if(i == 0){
            ui->textBrowser->setTextColor(Qt::red);
        }else{
            ui->textBrowser->setTextColor(Qt::black);
        }
        QString time = array.at(i).toObject().value("time").toString();
        QString info = array.at(i).toObject().value("context").toString();
        qDebug() << "info:" << time << info;
        ui->textBrowser->append(time);
        ui->textBrowser->append(info + "\n");
    }
    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->minimum());
}

void Widget::on_btn_select_clicked()
{
    QNetworkRequest request;
    QString type = ui->comboBox->currentText();
    QString postid;
    QSqlQuery qry;
    qry.exec(QString("select postid from information where type='%1'").arg(type));
    while(qry.next()){
        postid = qry.value(0).toByteArray();
        qDebug() << "postId:" << postid;
    }
    QString number = ui->lineEdit->text();
    qDebug() << "ddd:" << type << postid << number;

    if(number.isEmpty()){
        QMessageBox::warning(this, tr("警告"), tr("请输入快递单号!"));
        return;
    }

    request.setUrl(QString("http://api.kuaidi100.com/api?id=29833628d495d7a5&com=%1&nu=%2&show=0&muti=1&order=desc")
                   .arg(postid).arg(number));

    manager->get(request);
}
