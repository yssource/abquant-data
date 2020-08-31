#ifndef INDEXMINCONTROLLER_H
#define INDEXMINCONTROLLER_H

#include "abquant/controllers/applicationcontroller.h"

class T_CONTROLLER_EXPORT IndexMinController : public ApplicationController
{
    Q_OBJECT
public:
    IndexMinController() : ApplicationController() {}

public slots:
    void index();
    void show(const QString& id);
    void create();
    void save(const QString& id);
    void remove(const QString& id);
};

#endif // INDEXMINCONTROLLER_H
