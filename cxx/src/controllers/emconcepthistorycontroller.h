#ifndef EMCONCEPTHISTORYCONTROLLER_H
#define EMCONCEPTHISTORYCONTROLLER_H

#include "abquant/controllers/applicationcontroller.h"

class T_CONTROLLER_EXPORT EmConceptHistoryController : public ApplicationController
{
    Q_OBJECT
public:
    EmConceptHistoryController() : ApplicationController() {}

public slots:
    void index();
    void show(const QString& id);
    void create();
    void save(const QString& id);
    void remove(const QString& id);
};

#endif // EMCONCEPTHISTORYCONTROLLER_H
