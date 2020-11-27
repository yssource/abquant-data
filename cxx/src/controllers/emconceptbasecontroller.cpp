#include "abquant/controllers/emconceptbasecontroller.h"

#include "abquant/models/emconceptbase.h"

void EmConceptBaseController::index()
{
    auto emconceptbaseList = EmConceptBase::getAll();
    texport(emconceptbaseList);
    render();
}

void EmConceptBaseController::show(const QString& id)
{
    auto emconceptbase = EmConceptBase::get(id);
    texport(emconceptbase);
    render();
}

void EmConceptBaseController::create()
{
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto emconceptbase = httpRequest().formItems("emconceptbase");
        auto model         = EmConceptBase::create(emconceptbase);

        if (!model.isNull()) {
            QString notice = "Created successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            QString error = "Failed to create.";
            texport(error);
            texport(emconceptbase);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void EmConceptBaseController::save(const QString& id)
{
    switch (httpRequest().method()) {
    case Tf::Get: {
        auto model = EmConceptBase::get(id);
        if (!model.isNull()) {
            auto emconceptbase = model.toVariantMap();
            texport(emconceptbase);
            render();
        }
        break;
    }

    case Tf::Post: {
        QString error;
        auto model = EmConceptBase::get(id);

        if (model.isNull()) {
            error =
                "Original data not found. It may have been updated/removed by another "
                "transaction.";
            tflash(error);
            redirect(urla("save", id));
            break;
        }

        auto emconceptbase = httpRequest().formItems("emconceptbase");
        model.setProperties(emconceptbase);
        if (model.save()) {
            QString notice = "Updated successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            error = "Failed to update.";
            texport(error);
            texport(emconceptbase);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void EmConceptBaseController::remove(const QString& id)
{
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto emconceptbase = EmConceptBase::get(id);
    emconceptbase.remove();
    redirect(urla("index"));
}

// Don't remove below this line
T_DEFINE_CONTROLLER(EmConceptBaseController)
