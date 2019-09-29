//
// settings.h
// Created by TheFatNinja 
// 26-09-2019
//

#ifndef REDIS_VIEWER_SETTINGS_H
#define REDIS_VIEWER_SETTINGS_H

#include <QObject>
#include <QDir>

class Settings : public QObject {
    Q_OBJECT

private:
    int main_window_x;
    int main_window_y;
    int main_window_width;
    int main_window_height;
    bool group_keys;
    char group_char;

public:
    void Load();
    void LoadDefaults();
    void Save();
    static QDir GetSettingsDir();

    // Property Accessors
    int MainWindowX();
    int MainWindowY();
    int MainWindowWidth();
    int MainWindowHeight();
    bool GroupKeys();

    // Property Setters
    void SetMainWindowX(int value);
    void SetMainWindowY(int value);
    void SetMainWindowWidth(int value);
    void SetMainWindowHeight(int value);
    void SetGroupKeys(bool value);

signals:
    void SettingsChanged();

};

#endif //REDIS_VIEWER_SETTINGS_H
