#ifndef INDEXDAYCONTROLLER_H
#define INDEXDAYCONTROLLER_H

#include "abquant/controllers/applicationcontroller.h"

class T_CONTROLLER_EXPORT IndexDayController : public ApplicationController
{
    Q_OBJECT
public:
    IndexDayController() : ApplicationController() {}

public slots:
    void index();
    void show(const QString& id);
    void create();
    void save(const QString& id);
    void remove(const QString& id);
};

#endif // INDEXDAYCONTROLLER_H
