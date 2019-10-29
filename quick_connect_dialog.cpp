//
// quick_connect_dialog.cpp
// Created by TheFatNinja 
// 15-10-2019
//

#include <QMessageBox>
#include "quick_connect_dialog.h"
#include "standard_redis_connection.h"

QuickConnectDialog::QuickConnectDialog() {
    setWindowTitle("Quick Connect");
    setWindowIcon(QIcon(":/images/quick_connect.svg"));
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    // Main layout
    setLayout(&main_layout);
    main_layout.addWidget(&host_label, 0, 0);
    main_layout.addWidget(&host_value, 0, 1);
    main_layout.addWidget(&port_label, 1, 0);
    main_layout.addWidget(&port_value, 1, 1);
    main_layout.addLayout(&button_layout, 2, 1);

    // Setup labels and helpers
    host_label.setText("Host:");
    host_label.setToolTip("Host Name/IP Address");
    host_value.setPlaceholderText("Host Name/IP Address");
    // TODO: Temporary dev hack
    host_value.setText("127.0.0.1");

    port_label.setText("Port:");
    port_label.setToolTip("Port (default 6379)");
    port_value.setPlaceholderText("Port (default 6379)");

    // Button layout
    button_layout.addWidget(&cancel_button);
    button_layout.addWidget(&ok_button);

    // Buttons
    cancel_button.setText("Cancel");
    cancel_button.setDefault(false);
    cancel_button.setAutoDefault(false);
    ok_button.setText("Connect");
    ok_button.setDefault(true);
    ok_button.setAutoDefault(true);
    connect(&cancel_button, &QPushButton::pressed, this, &QuickConnectDialog::reject);
    connect(&ok_button, &QPushButton::pressed, this, &QuickConnectDialog::OKClicked);
}

void QuickConnectDialog::OKClicked() {
    // TODO: Setup connection
    QString host = host_value.text();
    QString port_str = port_value.text();
    if (host.isEmpty()) {
        QMessageBox::critical(this, "Missing Host", "Host Name/IP Address required");
        return;
    }
    int port = 6379;
    if (!port_str.isEmpty()) {
        try {
            port = std::stoi(port_str.toStdString());
        }
        catch (std::invalid_argument& err) {
            QMessageBox::critical(this, "Invalid Port", "Port must be a number");
            return;
        }
    }
    connection = std::make_shared<StandardRedisConnection>(host.toStdString(), port);
    accept();
}

RedisConnectionPtr QuickConnectDialog::GetRedisConnection() {
    return connection;
}
