//
// main_window.h
// Created by TheFatNinja 
// 18-09-2019
//

#ifndef REDIS_VIEWER_MAIN_WINDOW_H
#define REDIS_VIEWER_MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSplitter>
#include <QToolBar>
#include <QComboBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QTabWidget>
#include <QLabel>
#include <QMenu>
#include "tab_document.h"
#include "settings.h"
#include "redis_connection.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Settings settings;
    QMenu recent_connections;
    QToolBar toolbar;
    QSplitter main_widget;
    QWidget left_panel;
    QVBoxLayout left_panel_layout;
    QToolBar list_toolbar;
    bool group_keys;
    QComboBox instance_select;
    QLineEdit filter_list;
    QTreeWidget redis_keys_tree;
    QTabWidget tabs;

    QLabel connection_status;
    RedisConnectionPtr redis = nullptr;

protected:
    void closeEvent(QCloseEvent *event);

public:
    MainWindow();
    void ShowStatusMessage(const std::string &message);
    void UpdateRecentConnections();
    void QuickConnect();
    void UpdateKeysList();
    void AddTab(const std::string &name, TabDocument *document);

    // List Actions
    void AddKey();
    void DeleteKey();
    void ToggleGrouping();
    void FilterList();
    void ReloadKeys();

public slots:
    void Exit();
    void ShowConnectionManager();
    void CloseTab(TabDocument *document);
    void ClosingTab(int index);




};

#endif //REDIS_VIEWER_MAIN_WINDOW_H
