//
// welcome_tab.cpp
// Created by TheFatNinja 
// 23-09-2019
//

#include "welcome_tab.h"

WelcomeTab::WelcomeTab() {
    setLayout(&layout);
    layout.addWidget(&header);
    layout.addWidget(&body);
    layout.addStretch(1);
    header.setText("Welcome to the Redis Viewer");
    QFont heading_font;
    heading_font.setBold(true);
    heading_font.setPixelSize(30);
    header.setFont(heading_font);
    header.setWordWrap(true);
    body.setText("Start by opening the Connection Manager with the first toolbar button or in the File menu.");
    body.setWordWrap(true);
}

bool WelcomeTab::CanClose() {
    return true;
}

bool WelcomeTab::IsRedisDoc() {
    return false;
}
