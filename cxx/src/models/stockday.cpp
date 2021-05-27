#include "abquant/models/stockday.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/stockdayobject.h"

StockDay::StockDay() : TAbstractModel(), d(new StockDayObject())
{
    d->open       = 0;
    d->close      = 0;
    d->high       = 0;
    d->low        = 0;
    d->vol        = 0;
    d->amount     = 0;
    d->date_stamp = 0;
}

StockDay::StockDay(const StockDay& other) : TAbstractModel(), d(other.d) {}

StockDay::StockDay(const StockDayObject& object) : TAbstractModel(), d(new StockDayObject(object)) {}

StockDay::~StockDay()
{
    // If the reference count becomes 0,
    // the shared data object 'StockDayObject' is deleted.
}

QString StockDay::id() const { return d->_id; }

double StockDay::open() const { return d->open; }

void StockDay::setOpen(double open) { d->open = open; }

double StockDay::close() const { return d->close; }

void StockDay::setClose(double close) { d->close = close; }

double StockDay::high() const { return d->high; }

void StockDay::setHigh(double high) { d->high = high; }

double StockDay::low() const { return d->low; }

void StockDay::setLow(double low) { d->low = low; }

double StockDay::vol() const { return d->vol; }

void StockDay::setVol(double vol) { d->vol = vol; }

double StockDay::amount() const { return d->amount; }

void StockDay::setAmount(double amount) { d->amount = amount; }

QString StockDay::date() const { return d->date; }

void StockDay::setDate(const QString& date) { d->date = date; }

QString StockDay::code() const { return d->code; }

void StockDay::setCode(const QString& code) { d->code = code; }

double StockDay::dateStamp() const { return d->date_stamp; }

void StockDay::setDateStamp(double dateStamp) { d->date_stamp = dateStamp; }

StockDay& StockDay::operator=(const StockDay& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool StockDay::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

StockDay StockDay::create(double open, double close, double high, double low, double vol, double amount,
                          const QString& date, const QString& code, double dateStamp)
{
    StockDayObject obj;
    obj.open       = open;
    obj.close      = close;
    obj.high       = high;
    obj.low        = low;
    obj.vol        = vol;
    obj.amount     = amount;
    obj.date       = date;
    obj.code       = code;
    obj.date_stamp = dateStamp;
    if (!obj.create()) {
        return StockDay();
    }
    return StockDay(obj);
}

StockDay StockDay::create(const QVariantMap& values)
{
    StockDay model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

StockDay StockDay::get(const QString& id)
{
    TMongoODMapper<StockDayObject> mapper;
    return StockDay(mapper.findByObjectId(id));
}

QList<StockDay> StockDay::get_price(const QStringList codes, double start, double end)
{
    TMongoODMapper<StockDayObject> mapper;
    TCriteria cri;
    // QDateTime start_ = QDateTime::fromString(start, Qt::ISODate);
    cri.add(StockDayObject::Code, TMongo::In,
            codes); // AND add to the end operator

    cri.add(StockDayObject::DateStamp, TMongo::GreaterEqual,
            start); // AND add to the end operator

    // QDateTime end_ = QDateTime::fromString(end, Qt::ISODate);
    cri.add(StockDayObject::DateStamp, TMongo::LessEqual,
            end); // AND add to the end operator
    return tfGetModelListByMongoCriteria<StockDay, StockDayObject>(cri);
}

int StockDay::count()
{
    TMongoODMapper<StockDayObject> mapper;
    return mapper.findCount();
}

QList<StockDay> StockDay::getAll() { return tfGetModelListByMongoCriteria<StockDay, StockDayObject>(TCriteria()); }

QJsonArray StockDay::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<StockDayObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(StockDay(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* StockDay::modelData() { return d.data(); }

const TModelObject* StockDay::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const StockDay& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, StockDay& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(StockDay)
