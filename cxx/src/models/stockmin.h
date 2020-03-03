#ifndef STOCKMIN_H
#define STOCKMIN_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>
// #include "abquant/helpers/applicationhelper.h"
#include "abquant/actions/utils.hpp"

class TModelObject;
class StockMinObject;
class QJsonArray;

class T_MODEL_EXPORT StockMin : public TAbstractModel
{
public:
    StockMin();
    StockMin(const StockMin& other);
    StockMin(const StockMinObject& object);
    ~StockMin();

    QString id() const;
    double open() const;
    void setOpen(double open);
    double close() const;
    void setClose(double close);
    double high() const;
    void setHigh(double high);
    double low() const;
    void setLow(double low);
    double vol() const;
    void setVol(double vol);
    double amount() const;
    void setAmount(double amount);
    QString datetime() const;
    void setDatetime(const QString& datetime);
    QString code() const;
    void setCode(const QString& code);
    QString date() const;
    void setDate(const QString& date);
    double dateStamp() const;
    void setDateStamp(double dateStamp);
    double timeStamp() const;
    void setTimeStamp(double timeStamp);
    QString type() const;
    void setType(const QString& type);
    StockMin& operator=(const StockMin& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static StockMin create(double open, double close, double high, double low, double vol, double amount,
                           const QString& datetime, const QString& code, const QString& date, double dateStamp,
                           double timeStamp, const QString& type);
    static StockMin create(const QVariantMap& values);
    static StockMin get(const QString& id);
    static QList<StockMin> get_price(const QStringList codes, double start, double end, abq::MIN_FREQ freq = abq::MIN_FREQ::ONE);
    static int count();
    static QList<StockMin> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<StockMinObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const StockMin& model);
    friend QDataStream& operator>>(QDataStream& ds, StockMin& model);
};

Q_DECLARE_METATYPE(StockMin)
Q_DECLARE_METATYPE(QList<StockMin>)

#endif // STOCKMIN_H
