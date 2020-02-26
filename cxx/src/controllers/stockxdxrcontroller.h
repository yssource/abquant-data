#ifndef STOCKXDXRCONTROLLER_H
#define STOCKXDXRCONTROLLER_H

#include "abquant/controllers/applicationcontroller.h"

class T_CONTROLLER_EXPORT StockXdxrController : public ApplicationController
{
    Q_OBJECT
public:
    StockXdxrController() : ApplicationController() {}

public slots:
    void index();
    void show(const QString& id);
    void create();
    void save(const QString& id);
    void remove(const QString& id);
};

#endif // STOCKXDXRCONTROLLER_H
