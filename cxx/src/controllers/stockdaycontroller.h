#ifndef STOCKDAYCONTROLLER_H
#define STOCKDAYCONTROLLER_H

#include "abquant/controllers/applicationcontroller.h"

class T_CONTROLLER_EXPORT StockDayController : public ApplicationController
{
    Q_OBJECT
public:
    StockDayController() : ApplicationController() {}

public slots:
    void index();
    void show(const QString& id);
    void create();
    void save(const QString& id);
    void remove(const QString& id);
};

#endif // STOCKDAYCONTROLLER_H
