#ifndef EMCONCEPTBASECONTROLLER_H
#define EMCONCEPTBASECONTROLLER_H

#include "abquant/controllers/applicationcontroller.h"

class T_CONTROLLER_EXPORT EmConceptBaseController : public ApplicationController
{
    Q_OBJECT
public:
    EmConceptBaseController() : ApplicationController() {}

public slots:
    void index();
    void show(const QString& id);
    void create();
    void save(const QString& id);
    void remove(const QString& id);
};

#endif // EMCONCEPTBASECONTROLLER_H
