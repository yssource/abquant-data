#ifndef STOCKMINCONTROLLER_H
#define STOCKMINCONTROLLER_H

#include "abquant/controllers/applicationcontroller.h"

class T_CONTROLLER_EXPORT StockMinController : public ApplicationController
{
    Q_OBJECT
public:
    StockMinController() : ApplicationController() {}

public slots:
    void index();
    void show(const QString& id);
    void create();
    void save(const QString& id);
    void remove(const QString& id);
};

#endif // STOCKMINCONTROLLER_H
