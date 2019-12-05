//
// value_editor.cpp
// Created by TheFatNinja 
// 01-11-2019
//

#include "value_editor.h"
#include <QFontDatabase>
#ifdef ENABLE_BASE64
#include <cppcodec/base64_rfc4648.hpp>
#include <stack>
#ifdef ENABLE_ZLIB
#include "gzip.h"
#endif // ENABLE_ZLIB
#endif // ENABLE_BASE64
#include "json.h"


ValueEditor::ValueEditor() {
    decoded_value = "";
    original_type = EditorTypeString;
    changed = false;
    layout.setContentsMargins(0, 0, 0, 0);

    editor.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    setLayout(&layout);
    layout.addWidget(&editor, 0, 0, 1, 2);
    layout.addWidget(&type_label, 1, 0);
    layout.addWidget(&type_selector, 1, 1);

    type_label.setText("Format");
    type_selector.setEditable(false);
    for (int i = 0; i < EditorTypesTotal - 1; ++i) {
        QString option = QString::fromStdString(EditorTypeNameLookup[i]);
        type_selector.addItem(option);
    }

    connect(&editor, &QTextEdit::textChanged, this, &ValueEditor::TextChanged);
}

void ValueEditor::SetValue(const std::string &value) {
    changed = false;
    decoded_value = value;
    // TODO: determine what type it is
#ifdef ENABLE_BASE64
    // First try to base64 decode it
    bool is_bas64 = false;
    try {
        std::vector<unsigned char> b64decoded = cppcodec::base64_rfc4648::decode(decoded_value);
        decoded_value = std::string(b64decoded.begin(), b64decoded.end());
        is_bas64 = true;
    }
    catch (cppcodec::parse_error &error) {
        std::cout << error.what() << std::endl;
    }
#ifdef ENABLE_ZLIB
    // Secondly check if it zipped
    bool is_zipped = false;
    try {
        decoded_value = gzip::decompress_string(decoded_value);
        is_zipped = true;
    }
    catch (std::runtime_error &error) {

    }
#endif // ENABLE_ZLIB
#endif // ENABLE_BASE64
    // Find first non-whitespace character
    bool is_json = false;
    bool is_xml = false;
    size_t pos = decoded_value.find_first_not_of(" \t\n");
    char current = decoded_value.at(pos);
    if (current == '{' || current == '[') {
        try {
            decoded_value = json::pretty_string(decoded_value);
            is_json = true;
        }
        catch (std::runtime_error &error) {

        }
    }
    else if (current == '<') {
        try {
            // TODO: Format XML
//            is_xml = true;
        }
        catch (std::runtime_error &error) {

        }
    }
#ifdef ENABLE_BASE64
#ifdef ENABLE_ZLIB
    if (is_bas64 && is_zipped) {
        if (is_json) {
            original_type = EditorTypeBase64GZipJSON;
        }
        else if (is_xml) {
            original_type = EditorTypeBase64GZipXML;
        }
        else {
            original_type = EditorTypeBase64GZipString;
        }
    }
    else
#endif // ENABLE_ZLIB
    if (is_bas64) {
        if (is_json) {
            original_type = EditorTypeBase64JSON;
        }
        else if (is_xml) {
            original_type = EditorTypeBase64XML;
        }
        else {
            original_type = EditorTypeBase64String;
        }
    }
    else
#endif // ENABLE_BASE64
    if (is_json) {
        original_type = EditorTypeJSON;
    }
    else if (is_xml) {
        original_type = EditorTypeXML;
    }
    else {
        original_type = EditorTypeString;
    }
    type_selector.setCurrentIndex(original_type);
    QString string_value = QString::fromStdString(decoded_value);
    editor.setPlainText(string_value);
}

std::string ValueEditor::GetValue() {
    std::string new_value = editor.toPlainText().toStdString();
    // TODO: convert output to selected type
    return new_value;
}

void ValueEditor::TextChanged() {
    QString val = editor.toPlainText();
    std::string editor_value = val.toStdString();
    EditorType current_type = static_cast<EditorType>(type_selector.currentIndex());
    if (editor_value == decoded_value && current_type == original_type) {
        if (changed) {
            changed = false;
            emit changes_undone();
        }
    }
    else {
        if (!changed) {
            changed = true;
            emit value_changed();
        }
    }
}

bool ValueEditor::HasChanges() {
    return changed;
}
