//
// settings.cpp
// Created by TheFatNinja 
// 26-09-2019
//

#include "settings.h"
#include <QStandardPaths>

#define REDIS_VIEWER_CONF_FILE "redisviewer.conf"

Settings::Settings() : Serialisable("Settings") {
    SerialisableProperty("Window", "MainWindowX", IntegerType, main_window_x, 50);
    SerialisableProperty("Window", "MainWindowY", IntegerType, main_window_y, 50);
    SerialisableProperty("Window", "MainWindowWidth", IntegerType, main_window_width, 800);
    SerialisableProperty("Window", "MainWindowHeight", IntegerType, main_window_height, 600);
    SerialisableProperty("Grouping", "GroupKeys", BooleanType, group_keys, true);
    SerialisableProperty("Grouping", "GroupCharacter", CharacterType, group_char, ':');
}

void Settings::Load() {
    QDir dir = GetSettingsDir();
    if (dir.exists(REDIS_VIEWER_CONF_FILE)) {
        std::string path = dir.absoluteFilePath(REDIS_VIEWER_CONF_FILE).toStdString();
        Deserialise(path);
    }
}

void Settings::Save() {
    QDir dir = GetSettingsDir();
    std::string path = dir.absoluteFilePath(REDIS_VIEWER_CONF_FILE).toStdString();
    Serialise(path);
}

QDir Settings::GetSettingsDir() {
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists(dir.absolutePath())) {
        dir.mkdir(dir.absolutePath());
    }
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
