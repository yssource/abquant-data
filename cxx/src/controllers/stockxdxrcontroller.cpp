#include "abquant/controllers/stockxdxrcontroller.h"

#include "abquant/models/stockxdxr.h"

void StockXdxrController::index()
{
    auto stockXdxrList = StockXdxr::getAll();
    texport(stockXdxrList);
    render();
}

void StockXdxrController::show(const QString& id)
{
    auto stockXdxr = StockXdxr::get(id);
    texport(stockXdxr);
    render();
}

void StockXdxrController::create()
{
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto stockXdxr = httpRequest().formItems("stockXdxr");
        auto model     = StockXdxr::create(stockXdxr);

        if (!model.isNull()) {
            QString notice = "Created successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            QString error = "Failed to create.";
            texport(error);
            texport(stockXdxr);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void StockXdxrController::save(const QString& id)
{
    switch (httpRequest().method()) {
    case Tf::Get: {
        auto model = StockXdxr::get(id);
        if (!model.isNull()) {
            auto stockXdxr = model.toVariantMap();
            texport(stockXdxr);
            render();
        }
        break;
    }

    case Tf::Post: {
        QString error;
        auto model = StockXdxr::get(id);

        if (model.isNull()) {
            error =
                "Original data not found. It may have been updated/removed by another "
                "transaction.";
            tflash(error);
            redirect(urla("save", id));
            break;
        }

        auto stockXdxr = httpRequest().formItems("stockXdxr");
        model.setProperties(stockXdxr);
        if (model.save()) {
            QString notice = "Updated successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            error = "Failed to update.";
            texport(error);
            texport(stockXdxr);
            render();
        }
        break;
    }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void StockXdxrController::remove(const QString& id)
{
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto stockXdxr = StockXdxr::get(id);
    stockXdxr.remove();
    redirect(urla("index"));
}

// Don't remove below this line
T_DEFINE_CONTROLLER(StockXdxrController)
