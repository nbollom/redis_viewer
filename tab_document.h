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

protected:
    bool has_changes;

public:
    virtual std::string Name() = 0;
    virtual bool CanClose() = 0;
    virtual bool IsRedisDoc() = 0;

signals:
    void CloseTab(TabDocument *doc);

};

#endif //REDIS_VIEWER_TAB_DOCUMENT_H
