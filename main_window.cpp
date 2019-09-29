//
// main_window.cpp
// Created by TheFatNinja 
// 18-09-2019
//

#include "main_window.h"
#include <QMenuBar>
#include <QStatusBar>
#include "welcome_tab.h"

MainWindow::MainWindow() {
    // Load Settings
    settings.Load();

    // Window Properties
    setWindowTitle("Redis Viewer");
    setMinimumSize(400, 400);
    addToolBar(Qt::ToolBarArea::TopToolBarArea, &toolbar);
    setCentralWidget(&main_widget);
    // Make sure status bar is already in the layout otherwise it pops in with a message
    QStatusBar *status = statusBar();
    status->show();
    status->setSizeGripEnabled(true);
    status->addPermanentWidget(&connection_status);
    connection_status.setText("Disconnected");
    move(settings.MainWindowX(), settings.MainWindowY());
    setFixedSize(settings.MainWindowWidth(), settings.MainWindowHeight());

    // Menus
    QMenu *file_menu = menuBar()->addMenu("File");
    file_menu->addAction(QIcon::fromTheme("document-properties"), "Connection Manager", this, &MainWindow::ShowConnectionManager);
    file_menu->addMenu(&recent_connections);
    recent_connections.setTitle("Recent Connections");
    recent_connections.setIcon(QIcon::fromTheme("document-open-recent"));
    UpdateRecentConnections();
    UpdateRecentConnections();
    file_menu->addAction(QIcon::fromTheme("document-new"), "Quick Connect", this, &MainWindow::QuickConnect);
    file_menu->addAction(QIcon::fromTheme("application-exit"), "Exit", this, &MainWindow::Exit);

    // Toolbars
    toolbar.setMovable(false);
    toolbar.setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
    toolbar.addAction(QIcon::fromTheme("document-properties"), "Connect", this, &MainWindow::ShowConnectionManager);
    toolbar.addAction(QIcon::fromTheme("document-new"), "Quick Connect", this, &MainWindow::QuickConnect);

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
    list_toolbar.addAction(QIcon::fromTheme("list-add"), "Add", this, &MainWindow::AddKey);
    list_toolbar.addAction(QIcon::fromTheme("list-remove"), "Remove", this, &MainWindow::DeleteKey);
    QAction *grouping = list_toolbar.addAction(QIcon::fromTheme("insert-link"), "Toggle Grouping", this, &MainWindow::ToggleGrouping);
    QAction *filter = list_toolbar.addAction(QIcon::fromTheme("edit-find"), "Filter", this, &MainWindow::FilterList);
    list_toolbar.addSeparator();
    list_toolbar.addAction(QIcon::fromTheme("loading"), "Reload", this, &MainWindow::ReloadKeys);
    grouping->setCheckable(true);
    group_keys = settings.GroupKeys();
    grouping->setChecked(group_keys);
    filter->setCheckable(true);

    // Tabs Panel
     tabs.setTabsClosable(true);
     connect(&tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::ClosingTab);
     auto *welcome_tab = new WelcomeTab();
     AddTab("Welcome", welcome_tab);
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

void MainWindow::QuickConnect() {
    // TODO: Show quick connect dialog
}

void MainWindow::UpdateKeysList() {
    if (group_keys) {
        // TODO: Apply grouping logic
    }
    else {
        // TODO: Standard list of keys
    }
}

void MainWindow::AddTab(const std::string &name, TabDocument *document) {
    tabs.addTab(document, QString::fromStdString(name));
    connect(document, &TabDocument::CloseTab, this, &MainWindow::CloseTab);
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
    // TODO: Reload Keys
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