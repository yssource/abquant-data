#include "abquant/controllers/indexmincontroller.h"

#include "abquant/models/indexmin.h"

void IndexMinController::index()
{
    auto indexMinList = IndexMin::getAll();
    texport(indexMinList);
    render();
}

void IndexMinController::show(const QString& id)
{
    auto indexMin = IndexMin::get(id);
    texport(indexMin);
    render();
}

void IndexMinController::create()
{
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto indexMin = httpRequest().formItems("indexMin");
        auto model    = IndexMin::create(indexMin);

        if (!model.isNull()) {
            QString notice = "Created successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            QString error = "Failed to create.";
            texport(error);
            texport(indexMin);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void IndexMinController::save(const QString& id)
{
    switch (httpRequest().method()) {
    case Tf::Get: {
        auto model = IndexMin::get(id);
        if (!model.isNull()) {
            auto indexMin = model.toVariantMap();
            texport(indexMin);
            render();
        }
        break;
    }

    case Tf::Post: {
        QString error;
        auto model = IndexMin::get(id);

        if (model.isNull()) {
            error =
                "Original data not found. It may have been updated/removed by another "
                "transaction.";
            tflash(error);
            redirect(urla("save", id));
            break;
        }

        auto indexMin = httpRequest().formItems("indexMin");
        model.setProperties(indexMin);
        if (model.save()) {
            QString notice = "Updated successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            error = "Failed to update.";
            texport(error);
            texport(indexMin);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void IndexMinController::remove(const QString& id)
{
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto indexMin = IndexMin::get(id);
    indexMin.remove();
    redirect(urla("index"));
}

// Don't remove below this line
T_DEFINE_CONTROLLER(IndexMinController)
