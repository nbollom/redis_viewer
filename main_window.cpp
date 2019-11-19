//
// main_window.cpp
// Created by TheFatNinja 
// 18-09-2019
//

#include "main_window.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <algorithm>
#include "welcome_tab.h"
#include "task_queue.h"
#include "quick_connect_dialog.h"
#include "redis_exceptions.h"
#include "string_tab_document.h"

MainWindow::MainWindow() {
    // Load Settings
    settings.Load();
    task_queue = GetTaskQueue();
    connect(&task_queue_timer, &QTimer::timeout, this, &MainWindow::ProcessTasks);
    task_queue_timer.start(100);

    // Window Properties
    setWindowTitle("Redis Viewer");
    setMinimumSize(400, 400);
    setWindowIcon(QIcon(":/images/quick_connect.svg"));
    addToolBar(Qt::ToolBarArea::TopToolBarArea, &toolbar);
    setCentralWidget(&main_widget);
    // Make sure status bar is already in the layout otherwise it pops in with a message
    QStatusBar *status = statusBar();
    status->show();
    status->setSizeGripEnabled(true);
    status->addPermanentWidget(&connection_status);
    connection_status.setText("Disconnected");
    move(settings.MainWindowX(), settings.MainWindowY());
    resize(settings.MainWindowWidth(), settings.MainWindowHeight());

    // Menus
    QMenu *file_menu = menuBar()->addMenu("File");
    file_menu->addAction(QIcon(":/images/connections.svg"), "Connection Manager", this, &MainWindow::ShowConnectionManager);
    file_menu->addMenu(&recent_connections);
    recent_connections.setTitle("Recent Connections");
    recent_connections.setIcon(QIcon(":/images/recent2.svg"));
    UpdateRecentConnections();
    file_menu->addAction(QIcon(":/images/quick_connect.svg"), "Quick Connect", this, &MainWindow::QuickConnect);
    file_menu->addAction(QIcon(":/images/exit.svg"), "Exit", this, &MainWindow::Exit);

    // Toolbars
    toolbar.setMovable(false);
    toolbar.setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
    toolbar.addAction(QIcon(":/images/connections.svg"), "Connect", this, &MainWindow::ShowConnectionManager);
    toolbar.addAction(QIcon(":/images/quick_connect.svg"), "Quick Connect", this, &MainWindow::QuickConnect);

    // Main Layouts
    main_widget.addWidget(&left_panel);
    main_widget.addWidget(&tabs);
    main_widget.setStretchFactor(0, 3);
    main_widget.setStretchFactor(1, 6);
    main_widget.setChildrenCollapsible(false);

    // Left Panel
    left_panel.setLayout(&left_panel_layout);
    left_panel_layout.addWidget(&instance_select);
    left_panel_layout.addWidget(&list_toolbar);
    left_panel_layout.addWidget(&filter_list);
    left_panel_layout.addWidget(&redis_keys_tree);
    left_panel_layout.setMargin(0);
    instance_select.hide(); // Hide until we know there is a need for
    filter_list.hide(); // Toolbar button shows it

    // List Toolbar
    list_toolbar.addAction(QIcon(":/images/add.svg"), "Add", this, &MainWindow::AddKey);
    list_toolbar.addAction(QIcon(":/images/remove.svg"), "Remove", this, &MainWindow::DeleteKey);
    QAction *grouping = list_toolbar.addAction(QIcon(":/images/group.svg"), "Toggle Grouping", this, &MainWindow::ToggleGrouping);
    QAction *filter = list_toolbar.addAction(QIcon(":/images/search.svg"), "Filter", this, &MainWindow::FilterList);
    list_toolbar.addSeparator();
    QAction *refresh = list_toolbar.addAction(QIcon(":/images/refresh.svg"), "Reload", this, &MainWindow::ReloadKeys);
    grouping->setCheckable(true);
    group_keys = settings.GroupKeys();
    grouping->setChecked(group_keys);
    filter->setCheckable(true);

    // Redis Keys Tree
    redis_keys_tree.setHeaderHidden(false);
    redis_keys_tree.setHeaderLabel("Key");
    redis_keys_tree.setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    redis_keys_tree.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    connect(&redis_keys_tree, &QTreeWidget::itemActivated, this, &MainWindow::SelectKey);

    // Tabs Panel
     tabs.setTabsClosable(true);
     connect(&tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::ClosingTab);
     auto *welcome_tab = new WelcomeTab();
     AddTab(welcome_tab);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (tabs.count()) {
        event->ignore();
        int count = tabs.count();
        int *closed = new int(0);
        for (auto i = tabs.count() - 1; i >= 0; --i) {
            AsyncCloseTab(i, [this, closed, count] (bool success) {
                if (success) {
                    (*closed)++;
                    if (*closed == count) {
                        // All done, retry closing the window
                        delete closed;
                        this->close();
                    }
                }
            });
        }
    }
    else {
        settings.SetMainWindowX(x());
        settings.SetMainWindowY(y());
        settings.SetMainWindowWidth(width());
        settings.SetMainWindowHeight(height());
        settings.Save();
    }
}

void MainWindow::ShowStatusMessage(const std::string &message) {
    statusBar()->showMessage(QString::fromStdString(message), 2000);
}

void MainWindow::UpdateRecentConnections() {
    std::list<std::string> recent;
    bool has_recent = recent.size();
    recent_connections.setEnabled(has_recent);
    recent_connections.clear();
    for (auto connection: recent) {
        // TODO: populate recent connections menu
    }
}

void MainWindow::ProcessTasks() {
    task_queue->ProcessCallbacks();
}

void MainWindow::QuickConnect() {
    QuickConnectDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        RedisConnectionPtr new_connection = dialog.GetRedisConnection();
        ChangeRedisConnection(new_connection);
    }
}

void MainWindow::UpdateKeysList() {
    redis_keys_tree.clear();
    QIcon key_icon = QIcon(":/images/done.svg");
    if (group_keys) {
        struct Leaf {
            std::string label;
            std::vector<Leaf> children;
            std::string full_key;
        };
        Leaf master = Leaf();
        char group_char = settings.GroupChar();
        for (auto &key: keys_list) {
            // TODO: Apply grouping logic
            int start_pos = 0;
            Leaf *current = &master;
            int end_pos = key.find(group_char, start_pos);
            while (end_pos != std::string::npos) {
                std::string key_part = key.substr(start_pos, end_pos - start_pos);
                if (!current->children.empty() && current->children.back().label == key_part) {
                    current = &current->children.back();
                }
                else {
                    Leaf child;
                    child.label = key_part;
                    current->children.emplace_back(child);
                    current = &current->children.back();
                }
                start_pos = end_pos + 1;
                end_pos = key.find(group_char, start_pos);
            }
            std::string key_part = key.substr(start_pos);
            Leaf child;
            child.label = key_part;
            child.full_key = key;
            current->children.emplace_back(child);
        }
        std::function<QList<QTreeWidgetItem*> (const Leaf&)> build_child_items = [this, key_icon, &build_child_items](const Leaf &level) {
            QList<QTreeWidgetItem*> children;
            for (const Leaf &child: level.children) {
                QString key_string = QString::fromStdString(child.label);
                auto *item = new QTreeWidgetItem({key_string});
                if (!child.full_key.empty()) {
                    QString full_key = QString::fromStdString(child.full_key);
                    item->setToolTip(0, full_key);
                    item->setIcon(0, key_icon);
                }
                QList<QTreeWidgetItem*> child_items = build_child_items(child);
                if (child_items.count()) {
                    item->addChildren(child_items);
                }
                children.append(item);
            }
            return children;
        };
        QList<QTreeWidgetItem*> top_level_items = build_child_items(master);
        if (top_level_items.count()) {
            redis_keys_tree.addTopLevelItems(top_level_items);
        }
    }
    else {
        for (auto &key: keys_list) {
            QString key_string = QString::fromStdString(key);
            auto *item = new QTreeWidgetItem(&redis_keys_tree, {key_string});
            item->setToolTip(0, key_string);
            item->setIcon(0, key_icon);
            redis_keys_tree.addTopLevelItem(item);
        }
    }
    ShowStatusMessage("Done");
    redis_keys_tree.setFocus();
}

bool MainWindow::HasOpenTab(const std::string &key, bool is_redis) {
    for (auto i = 0; i < tabs.count(); ++i) {
        auto *tab = dynamic_cast<TabDocument*>(tabs.widget(i));
        if (tab->Name() == key && tab->IsRedisDoc() == is_redis) {
            return true;
        }
    }
    return false;
}

void MainWindow::SelectOpenTab(const std::string &key, bool is_redis) {
    for (auto i = 0; i < tabs.count(); ++i) {
        auto *tab = dynamic_cast<TabDocument*>(tabs.widget(i));
        if (tab->Name() == key && tab->IsRedisDoc() == is_redis) {
            tabs.setCurrentIndex(i);
            break;
        }
    }
}

void MainWindow::AddTab(TabDocument *document) {
    int index = tabs.addTab(document, QString::fromStdString(document->Name()));
    tabs.setCurrentIndex(index);
}

void MainWindow::ChangeRedisConnection(RedisConnectionPtr &new_connection) {
    if (redis != nullptr) {
        int non_doc_tabs = 0;
        for (auto i = tabs.count() - 1; i >= 0; --i) {
            auto *tab = dynamic_cast<TabDocument*>(tabs.widget(i));
            if (tab->IsRedisDoc()) {
                AsyncCloseTab(i, [](bool success){
                    // TODO: move logic below in here
                });
            }
            else {
                non_doc_tabs++;
            }
        }
        if (tabs.count() > non_doc_tabs) {
            QMessageBox::critical(this, "Error", "Cannot close connection while there are unsaved changes");
            return;
        }
        redis->Disconnect();
        this->keys_list.clear();
    }
    redis = new_connection;
    try {
        redis->Connect();
        connection_status.setText("Connected");
    }
    catch (RedisConnectionException &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
    ReloadKeys();
}

void MainWindow::AsyncCloseTab(int index, const std::function<void(bool)>& callback) {
    auto *tab = dynamic_cast<TabDocument*>(tabs.widget(index));
    if (tab->CanClose()) {
        delete tab;
        task_queue->RunLater([callback](){
            callback(true);
        });
    }
    else {
        tab->Save([callback, tab](bool success){
            if (success) {
                delete tab;
            }
            callback(success);
        });
    }
}

void MainWindow::AddKey() {
    // TODO: Show AddKey dialog
}

void MainWindow::DeleteKey() {
    QList<QTreeWidgetItem *> selected = redis_keys_tree.selectedItems();
    std::vector<std::string> keys_to_delete;
    std::function<void(const QTreeWidgetItem *)> get_keys_to_delete;
    get_keys_to_delete = [&keys_to_delete, &get_keys_to_delete] (const QTreeWidgetItem *item) {
        std::string tooltip = item->toolTip(0).toStdString();
        if (!tooltip.empty()) {
            keys_to_delete.emplace_back(tooltip);
        }
        for (int i = 0; i < item->childCount(); ++i) {
            get_keys_to_delete(item->child(i));
        }
    };
    for (const auto &item: selected) {
        get_keys_to_delete(item);
    }
    std::function<void(long long int&, const std::string&)> done_callback = [this, keys_to_delete, selected](long long int &value, const std::string &error){
        if (error.length()) {
            QMessageBox::critical(this, "Error", QString::fromStdString(error));
            connection_status.setText("Disconnected");
        }
        else {
            keys_list.erase(std::remove_if(keys_list.begin(), keys_list.end(), [&keys_to_delete, &selected](const std::string& val){
                return std::find(keys_to_delete.begin(), keys_to_delete.end(), val) != keys_to_delete.end();
            }));
            for (QTreeWidgetItem *item: selected) {
                delete item;
            }
        }
    };
    if (keys_to_delete.size() > 1) {
        std::string message = "Are you sure you want to delete " + std::to_string(keys_to_delete.size()) + " keys?";
        QMessageBox question("Redis Viewer", QString::fromStdString(message), QMessageBox::Question, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton);
        if (question.exec() == QMessageBox::Yes) {
            task_queue->RunTask<long long int>([this, keys_to_delete](){
                return redis->DEL(keys_to_delete);
            }, done_callback);
        }
    }
    else {
        std::string key = keys_to_delete[0];
        task_queue->RunTask<long long int>([this, key](){
            return redis->DEL(key);
        }, done_callback);
    }
}

void MainWindow::ToggleGrouping() {
    group_keys = !group_keys;
    UpdateKeysList();
}

void MainWindow::FilterList() {
    bool filter = filter_list.isVisible();
    filter_list.setVisible(!filter);
    if (!filter) {
        filter_list.setText(""); // clear filter for next time
        // TODO: Remove any current filter
    }
}

void MainWindow::ReloadKeys() {
    if (redis && redis->IsConnected()) {
        ShowStatusMessage("Loading Keys from DB");
        // TODO: Show a spinner on the keys list
        std::string filter = "*";
        if (filter_list.isVisible() && filter_list.text().length()) {
            filter = filter_list.text().toStdString();
        }
        task_queue->RunTask<std::vector<std::string>>([filter, this](){
            return this->redis->SCAN(filter, KeyTypeAny);
        }, [this](std::vector<std::string> &keys, const std::string &error){
            if (error.length()) {
                QMessageBox::critical(this, "Error", QString::fromStdString(error));
                connection_status.setText("Disconnected");
            }
            else {
                this->keys_list = keys;
                std::sort(this->keys_list.begin(), this->keys_list.end());
                UpdateKeysList();
            }
            // TODO: Hide the spinner on the keys list
        });
    }
}

void MainWindow::SelectKey(QTreeWidgetItem *item, int column) {
    std::string key = item->toolTip(0).toStdString();
    if (!HasOpenTab(key, true)) {
        if (!key.empty()) {
            ShowStatusMessage(key);
            task_queue->RunTask<KeyType>([key, this]() {
                return redis->TYPE(key);
            }, [key, this](KeyType &type, const std::string &error) {
                TabDocument *doc;
                switch (type) {
                    case KeyTypeString:
                        doc = new StringTabDocument(key, redis);
                        break;
                    case KeyTypeList:
                        ShowStatusMessage("List");
                        return;
                        break;
                    case KeyTypeSet:
                        ShowStatusMessage("Set");
                        return;
                        break;
                    case KeyTypeZSet:
                        ShowStatusMessage("Zset");
                        return;
                        break;
                    case KeyTypeHash:
                        ShowStatusMessage("Hash");
                        return;
                        break;
                    default:
                        throw std::runtime_error("Encountered unknown type");
                }
                AddTab(doc);
            });
        }
    }
    else {
        SelectOpenTab(key, true);
    }
}

void MainWindow::Exit() {
    close();
}

void MainWindow::ShowConnectionManager() {
    // TODO: Show Connection Manager
}

void MainWindow::ClosingTab(int index) {
    AsyncCloseTab(index, [](bool success){

    });
}
