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
#include "welcome_tab.h"
#include "task_queue.h"
#include "quick_connect_dialog.h"

MainWindow::MainWindow() {
    // Load Settings
    settings.Load();
    task_queue = std::make_shared<TaskQueue>();
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
     AddTab("Welcome", welcome_tab);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    for (auto i = tabs.count() - 1; i >= 0; --i) {
        ClosingTab(i);
    }
    if (tabs.count()) {
        event->ignore();
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
    if (group_keys) {
        struct leaf {
            std::string label;
            std::vector<leaf> children;
        };
        leaf master = leaf();
        char group_char = settings.GroupChar();
        for (auto &key: keys_list) {
            // TODO: Apply grouping logic
            int start = 0;
            leaf *current = &master;
            int end = key.find(group_char, start);
            while (end != std::string::npos) {
                std::string key_part = key.substr(start, end);
                bool exists = false;
                for (leaf &child: current->children) {
                    if (child.label == key_part) {
                        current = &child;
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    leaf child;
                    child.label = key_part;
                    current->children.emplace_back(child);
                    current = &current->children.back();
                }
                start = end + 1;
                end = key.find(group_char, start);
            }
            std::string key_part = key.substr(start);
            leaf child;
            child.label = key_part;
            current->children.emplace_back(child);
        }
        std::function<QList<QTreeWidgetItem*> (const leaf&)> build_child_items = [this, &build_child_items](const leaf &level) {
            QList<QTreeWidgetItem*> children;
            for (const leaf &child: level.children) {
                QString key_string = QString::fromStdString(child.label);
                auto *item = new QTreeWidgetItem({key_string});
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
        redis_keys_tree.expandAll();
    }
    else {
        for (auto &key: keys_list) {
            QString key_string = QString::fromStdString(key);
            auto *item = new QTreeWidgetItem(&redis_keys_tree, {key_string});
            redis_keys_tree.addTopLevelItem(item);
        }
    }
}

void MainWindow::AddTab(const std::string &name, TabDocument *document) {
    tabs.addTab(document, QString::fromStdString(name));
    connect(document, &TabDocument::CloseTab, this, &MainWindow::CloseTab);
}

void MainWindow::ChangeRedisConnection(RedisConnectionPtr &new_connection) {
    if (redis != nullptr) {
        int non_doc_tabs = 0;
        for (auto i = tabs.count() - 1; i >= 0; --i) {
            auto *tab = dynamic_cast<TabDocument*>(tabs.widget(i));
            if (tab->IsRedisDoc()) {
                ClosingTab(i);
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
    }
    redis = new_connection;
    redis->Connect();
    connection_status.setText("Connected");
    ReloadKeys();
}

void MainWindow::AddKey() {
    // TODO: Show AddKey dialog
}

void MainWindow::DeleteKey() {
    QList<QTreeWidgetItem *> selected = redis_keys_tree.selectedItems();
    std::list<std::string> keys_to_delete;
    for (const auto &item: selected) {
        if (item->childCount()) {
            // TODO: Find end children and add to list
        }
        else {
            // TODO: Get full key name (probably have to use parents)
        }
    }
    // TODO: Confirm deleting key(s)
    // TODO: Start a pipeline to delete all at once
    for (const auto &key: keys_to_delete) {
        // TODO: Send Redis DEL command and remove item from list
    }
    // TODO: Execute pipeline
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
        }, [this](std::vector<std::string> &keys){
            this->keys_list = keys;
            std::sort(this->keys_list.begin(), this->keys_list.end());
            UpdateKeysList();
            // TODO: Hide the spinner on the keys list
        });
    }
}

void MainWindow::SelectKey(QTreeWidgetItem *item, int column) {
    if (!item->childCount()) { // We only care about bottom level items
        std::string full_key;
        std::string join_character = std::string(1, settings.GroupChar());
        QTreeWidgetItem *current_item = item;
        do {
            if (full_key.length()) {
                full_key = current_item->text(0).toStdString() + join_character + full_key;
            }
            else {
                full_key = current_item->text(0).toStdString();
            }
            current_item = current_item->parent();
        } while (current_item);
        ShowStatusMessage(full_key);
    }
}

void MainWindow::Exit() {
    close();
}

void MainWindow::ShowConnectionManager() {
    // TODO: Show Connection Manager
}

void MainWindow::CloseTab(TabDocument *document) {
    ClosingTab(tabs.indexOf(document));
}

void MainWindow::ClosingTab(int index) {
    auto *tab = dynamic_cast<TabDocument*>(tabs.widget(index));
    if (tab->CanClose()) {
        delete tab;
    }
}
