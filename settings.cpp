//
// settings.cpp
// Created by TheFatNinja 
// 26-09-2019
//

#include "settings.h"

#define REDIS_VIEWER_DIRECTORY ".redisviewer"
#define REDIS_VIEWER_CONF_FILE "redisviewer.conf"

void Settings::Load() {
    QDir dir = GetSettingsDir();
    if (dir.exists(REDIS_VIEWER_CONF_FILE)) {

    }
    else {
        LoadDefaults();
    }
}

void Settings::LoadDefaults() {
    main_window_x = 50;
    main_window_y = 50;
    main_window_width = 800;
    main_window_height = 600;
    group_keys = true;
}

void Settings::Save() {

}

QDir Settings::GetSettingsDir() {
    QDir dir = QDir::home();
    if (!dir.exists(REDIS_VIEWER_DIRECTORY)) {
        dir.mkdir(REDIS_VIEWER_DIRECTORY);
    }
    dir.cd(REDIS_VIEWER_DIRECTORY);
    return dir;
}

int Settings::MainWindowX() {
    return main_window_x;
}

int Settings::MainWindowY() {
    return main_window_y;
}

int Settings::MainWindowWidth() {
    return main_window_width;
}

int Settings::MainWindowHeight() {
    return main_window_height;
}

bool Settings::GroupKeys() {
    return group_keys;
}

void Settings::SetMainWindowX(int value) {
    main_window_x = value;
}

void Settings::SetMainWindowY(int value) {
    main_window_y = value;
}

void Settings::SetMainWindowWidth(int value) {
    main_window_width = value;
}

void Settings::SetMainWindowHeight(int value) {
    main_window_height = value;
}

void Settings::SetGroupKeys(bool value) {
    group_keys = value;
    emit SettingsChanged();
}
