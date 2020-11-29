#include "abquant/controllers/emconcepthistorycontroller.h"

#include "abquant/models/emconcepthistory.h"

void EmConceptHistoryController::index()
{
    auto emconcepthistoryList = EmConceptHistory::getAll();
    texport(emconcepthistoryList);
    render();
}

void EmConceptHistoryController::show(const QString& id)
{
    auto emconcepthistory = EmConceptHistory::get(id);
    texport(emconcepthistory);
    render();
}

void EmConceptHistoryController::create()
{
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto emconcepthistory = httpRequest().formItems("emconcepthistory");
        auto model            = EmConceptHistory::create(emconcepthistory);

        if (!model.isNull()) {
            QString notice = "Created successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            QString error = "Failed to create.";
            texport(error);
            texport(emconcepthistory);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void EmConceptHistoryController::save(const QString& id)
{
    switch (httpRequest().method()) {
    case Tf::Get: {
        auto model = EmConceptHistory::get(id);
        if (!model.isNull()) {
            auto emconcepthistory = model.toVariantMap();
            texport(emconcepthistory);
            render();
        }
        break;
    }

    case Tf::Post: {
        QString error;
        auto model = EmConceptHistory::get(id);

        if (model.isNull()) {
            error =
                "Original data not found. It may have been updated/removed by another "
                "transaction.";
            tflash(error);
            redirect(urla("save", id));
            break;
        }

        auto emconcepthistory = httpRequest().formItems("emconcepthistory");
        model.setProperties(emconcepthistory);
        if (model.save()) {
            QString notice = "Updated successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            error = "Failed to update.";
            texport(error);
            texport(emconcepthistory);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void EmConceptHistoryController::remove(const QString& id)
{
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto emconcepthistory = EmConceptHistory::get(id);
    emconcepthistory.remove();
    redirect(urla("index"));
}

// Don't remove below this line
T_DEFINE_CONTROLLER(EmConceptHistoryController)
