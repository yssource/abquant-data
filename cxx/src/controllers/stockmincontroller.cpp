#include "abquant/controllers/stockmincontroller.h"

#include "abquant/models/stockmin.h"

void StockMinController::index()
{
    auto stockMinList = StockMin::getAll();
    texport(stockMinList);
    render();
}

void StockMinController::show(const QString& id)
{
    auto stockMin = StockMin::get(id);
    texport(stockMin);
    render();
}

void StockMinController::create()
{
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto stockMin = httpRequest().formItems("stockMin");
        auto model    = StockMin::create(stockMin);

        if (!model.isNull()) {
            QString notice = "Created successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            QString error = "Failed to create.";
            texport(error);
            texport(stockMin);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void StockMinController::save(const QString& id)
{
    switch (httpRequest().method()) {
    case Tf::Get: {
        auto model = StockMin::get(id);
        if (!model.isNull()) {
            auto stockMin = model.toVariantMap();
            texport(stockMin);
            render();
        }
        break;
    }

    case Tf::Post: {
        QString error;
        auto model = StockMin::get(id);

        if (model.isNull()) {
            error =
                "Original data not found. It may have been updated/removed by another "
                "transaction.";
            tflash(error);
            redirect(urla("save", id));
            break;
        }

        auto stockMin = httpRequest().formItems("stockMin");
        model.setProperties(stockMin);
        if (model.save()) {
            QString notice = "Updated successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            error = "Failed to update.";
            texport(error);
            texport(stockMin);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void StockMinController::remove(const QString& id)
{
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto stockMin = StockMin::get(id);
    stockMin.remove();
    redirect(urla("index"));
}

// Don't remove below this line
T_DEFINE_CONTROLLER(StockMinController)
