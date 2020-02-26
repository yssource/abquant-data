#include "abquant/controllers/stockdaycontroller.h"

#include "abquant/models/stockday.h"

void StockDayController::index()
{
    auto stockDayList = StockDay::getAll();
    texport(stockDayList);
    render();
}

void StockDayController::show(const QString& id)
{
    auto stockDay = StockDay::get(id);
    texport(stockDay);
    render();
}

void StockDayController::create()
{
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto stockDay = httpRequest().formItems("stockDay");
        auto model    = StockDay::create(stockDay);

        if (!model.isNull()) {
            QString notice = "Created successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            QString error = "Failed to create.";
            texport(error);
            texport(stockDay);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void StockDayController::save(const QString& id)
{
    switch (httpRequest().method()) {
    case Tf::Get: {
        auto model = StockDay::get(id);
        if (!model.isNull()) {
            auto stockDay = model.toVariantMap();
            texport(stockDay);
            render();
        }
        break;
    }

    case Tf::Post: {
        QString error;
        auto model = StockDay::get(id);

        if (model.isNull()) {
            error =
                "Original data not found. It may have been updated/removed by another "
                "transaction.";
            tflash(error);
            redirect(urla("save", id));
            break;
        }

        auto stockDay = httpRequest().formItems("stockDay");
        model.setProperties(stockDay);
        if (model.save()) {
            QString notice = "Updated successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            error = "Failed to update.";
            texport(error);
            texport(stockDay);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void StockDayController::remove(const QString& id)
{
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto stockDay = StockDay::get(id);
    stockDay.remove();
    redirect(urla("index"));
}

// Don't remove below this line
T_DEFINE_CONTROLLER(StockDayController)
