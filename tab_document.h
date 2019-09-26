//
// tab_document.h
// Created by TheFatNinja 
// 25-09-2019
//

#ifndef REDIS_VIEWER_TAB_DOCUMENT_H
#define REDIS_VIEWER_TAB_DOCUMENT_H

#include <QWidget>

class TabDocument : public QWidget {
    Q_OBJECT

private:
    bool has_changes;

public:
    virtual bool CanClose() = 0;

signals:
    void CloseTab(TabDocument *doc);

};

#endif //REDIS_VIEWER_TAB_DOCUMENT_H
