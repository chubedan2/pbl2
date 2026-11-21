#pragma once

#include "../Services/DiningSessionService.h"
#include "../Services/TableService.h"

#include <QWidget>

#include <functional>

class QTableWidget;

namespace restaurant {

class TableSessionPage : public QWidget {
public:
    TableSessionPage(TableService& tables,
                     DiningSessionService& sessions,
                     QWidget* parent = nullptr);

    void refresh();
    void setDataChangedCallback(std::function<void()> callback);

private:
    void refreshTables();
    void refreshSessions();
    void notifyDataChanged();

    TableService& tables_;
    DiningSessionService& sessions_;
    QTableWidget* tablesTable_;
    QTableWidget* sessionsTable_;
    std::function<void()> dataChangedCallback_;
};

}  // namespace restaurant
