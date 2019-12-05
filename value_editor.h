//
// value_editor.h
// Created by TheFatNinja 
// 01-11-2019
//

#ifndef REDIS_VIEWER_VALUE_EDITOR_H
#define REDIS_VIEWER_VALUE_EDITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <array>
#include "config.h"

class ValueEditor : public QWidget {
    Q_OBJECT;

private:

    typedef enum {
        EditorTypeString,
        EditorTypeJSON,
        EditorTypeXML,
#ifdef ENABLE_BASE64
        EditorTypeBase64String,
        EditorTypeBase64JSON,
        EditorTypeBase64XML,
#ifdef ENABLE_ZLIB
        EditorTypeBase64GZipString,
        EditorTypeBase64GZipJSON,
        EditorTypeBase64GZipXML,
#endif // ENABLE_ZLIB
#endif // ENABLE_BASE64
        EditorTypesTotal // Used to determine the amount of items in the enum since it depends on compile options
    } EditorType;

    const std::array<std::string, EditorTypesTotal> EditorTypeNameLookup = {
        "String"
        ,"JSON"
        ,"XML"
#ifdef ENABLE_BASE64
        ,"Base64 String"
        ,"Base64 JSON"
        ,"Base64 XML"
#ifdef ENABLE_ZLIB
        ,"Base64 GZip String"
        ,"Base64 GZip JSON"
        ,"Base64 GZip XML"
#endif // ENABLE_ZLIB
#endif // ENABLE_BASE64
    };

    QGridLayout layout;
    QTextEdit editor;
    QLabel type_label;
    QComboBox type_selector;

    std::string decoded_value;
    EditorType original_type;
    bool changed;

public:
    ValueEditor();
    void SetValue(const std::string &value);
    std::string GetValue();
    void TextChanged();
    bool HasChanges();

signals:
    void value_changed();
    void changes_undone();
};

#endif //REDIS_VIEWER_VALUE_EDITOR_H
