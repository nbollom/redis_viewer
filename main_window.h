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
#include <QTimer>
#include "tab_document.h"
#include "settings.h"
#include "redis_connection.h"
#include "task_queue.h"

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

    QTimer task_queue_timer;

    QLabel connection_status;
    TaskQueuePtr task_queue;
    RedisConnectionPtr redis = nullptr;

    std::vector<std::string> keys_list;

protected:
    void closeEvent(QCloseEvent *event) override;
    void UpdateKeysList();
    void ShowStatusMessage(const std::string &message);
    void UpdateRecentConnections();
    void AddTab(const std::string &name, TabDocument *document);
    void ChangeRedisConnection(RedisConnectionPtr &new_connection);

public:
    MainWindow();

public slots:
    void ProcessTasks();
    void QuickConnect();

    // List Actions
    void AddKey();
    void DeleteKey();
    void ToggleGrouping();
    void FilterList();
    void ReloadKeys();
    void SelectKey(QTreeWidgetItem *item, int column);

    void Exit();
    void ShowConnectionManager();
    void CloseTab(TabDocument *document);
    void ClosingTab(int index);




};

#endif //REDIS_VIEWER_MAIN_WINDOW_H
