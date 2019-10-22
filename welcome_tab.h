//
// welcome_tab.h
// Created by TheFatNinja 
// 23-09-2019
//

#ifndef REDIS_VIEWER_WELCOME_TAB_H
#define REDIS_VIEWER_WELCOME_TAB_H

#include "tab_document.h"
#include <QVBoxLayout>
#include <QLabel>

class WelcomeTab : public TabDocument {
    Q_OBJECT

private:
    QVBoxLayout layout;
    QLabel header;
    QLabel body;

public:
    WelcomeTab();
    bool CanClose();
    bool IsRedisDoc() override;

};

#endif //REDIS_VIEWER_WELCOME_TAB_H
