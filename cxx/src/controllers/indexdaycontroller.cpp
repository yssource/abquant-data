#include "abquant/controllers/indexdaycontroller.h"

#include "abquant/models/indexday.h"

void IndexDayController::index()
{
    auto indexDayList = IndexDay::getAll();
    texport(indexDayList);
    render();
}

void IndexDayController::show(const QString& id)
{
    auto indexDay = IndexDay::get(id);
    texport(indexDay);
    render();
}

void IndexDayController::create()
{
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto indexDay = httpRequest().formItems("indexDay");
        auto model    = IndexDay::create(indexDay);

        if (!model.isNull()) {
            QString notice = "Created successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            QString error = "Failed to create.";
            texport(error);
            texport(indexDay);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void IndexDayController::save(const QString& id)
{
    switch (httpRequest().method()) {
    case Tf::Get: {
        auto model = IndexDay::get(id);
        if (!model.isNull()) {
            auto indexDay = model.toVariantMap();
            texport(indexDay);
            render();
        }
        break;
    }

    case Tf::Post: {
        QString error;
        auto model = IndexDay::get(id);

        if (model.isNull()) {
            error =
                "Original data not found. It may have been updated/removed by another "
                "transaction.";
            tflash(error);
            redirect(urla("save", id));
            break;
        }

        auto indexDay = httpRequest().formItems("indexDay");
        model.setProperties(indexDay);
        if (model.save()) {
            QString notice = "Updated successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            error = "Failed to update.";
            texport(error);
            texport(indexDay);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void IndexDayController::remove(const QString& id)
{
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto indexDay = IndexDay::get(id);
    indexDay.remove();
    redirect(urla("index"));
}

// Don't remove below this line
T_DEFINE_CONTROLLER(IndexDayController)
