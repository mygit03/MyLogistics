#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void operateSql();
    void createTable();

private slots:
    void replyFinished(QNetworkReply *reply);

    void on_btn_select_clicked();

private:
    Ui::Widget *ui;

    QSqlDatabase db;
    QNetworkAccessManager *manager;
};

#endif // WIDGET_H
