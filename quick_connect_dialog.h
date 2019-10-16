//
// quick_connect_dialog.h
// Created by TheFatNinja 
// 15-10-2019
//

#ifndef REDIS_VIEWER_QUICK_CONNECT_DIALOG_H
#define REDIS_VIEWER_QUICK_CONNECT_DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include "redis_connection.h"

class QuickConnectDialog : public QDialog {
    Q_OBJECT;

private:
    QGridLayout main_layout;
    QLabel host_label;
    QLineEdit host_value;
    QLabel port_label;
    QLineEdit port_value;
    QHBoxLayout button_layout;
    QPushButton cancel_button;
    QPushButton ok_button;

    RedisConnectionPtr connection;

public:
    QuickConnectDialog();
    RedisConnectionPtr GetRedisConnection();

public slots:
    void OKClicked();

};

#endif //REDIS_VIEWER_QUICK_CONNECT_DIALOG_H
