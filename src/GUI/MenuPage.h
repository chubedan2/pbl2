#pragma once

#include "../Services/MenuService.h"

#include <QWidget>

#include <functional>

class QTableWidget;

namespace restaurant {

class MenuPage : public QWidget {
public:
    explicit MenuPage(MenuService& menu, QWidget* parent = nullptr);

    void refresh();
    void setDataChangedCallback(std::function<void()> callback);

private:
    void notifyDataChanged();

    MenuService& menu_;
    QTableWidget* menuTable_;
    std::function<void()> dataChangedCallback_;
};

}  // namespace restaurant
