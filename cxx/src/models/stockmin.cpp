#include "abquant/models/stockmin.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/stockminobject.h"

StockMin::StockMin() : TAbstractModel(), d(new StockMinObject())
{
    d->open       = 0;
    d->close      = 0;
    d->high       = 0;
    d->low        = 0;
    d->vol        = 0;
    d->amount     = 0;
    d->date_stamp = 0;
    d->time_stamp = 0;
}

StockMin::StockMin(const StockMin& other) : TAbstractModel(), d(other.d) {}

StockMin::StockMin(const StockMinObject& object) : TAbstractModel(), d(new StockMinObject(object)) {}

StockMin::~StockMin()
{
    // If the reference count becomes 0,
    // the shared data object 'StockMinObject' is deleted.
}

QString StockMin::id() const { return d->_id; }

double StockMin::open() const { return d->open; }

void StockMin::setOpen(double open) { d->open = open; }

double StockMin::close() const { return d->close; }

void StockMin::setClose(double close) { d->close = close; }

double StockMin::high() const { return d->high; }

void StockMin::setHigh(double high) { d->high = high; }

double StockMin::low() const { return d->low; }

void StockMin::setLow(double low) { d->low = low; }

double StockMin::vol() const { return d->vol; }

void StockMin::setVol(double vol) { d->vol = vol; }

double StockMin::amount() const { return d->amount; }

void StockMin::setAmount(double amount) { d->amount = amount; }

QString StockMin::datetime() const { return d->datetime; }

void StockMin::setDatetime(const QString& datetime) { d->datetime = datetime; }

QString StockMin::code() const { return d->code; }

void StockMin::setCode(const QString& code) { d->code = code; }

QString StockMin::date() const { return d->date; }

void StockMin::setDate(const QString& date) { d->date = date; }

double StockMin::dateStamp() const { return d->date_stamp; }

void StockMin::setDateStamp(double dateStamp) { d->date_stamp = dateStamp; }

double StockMin::timeStamp() const { return d->time_stamp; }

void StockMin::setTimeStamp(double timeStamp) { d->time_stamp = timeStamp; }

QString StockMin::type() const { return d->type; }

void StockMin::setType(const QString& type) { d->type = type; }

StockMin& StockMin::operator=(const StockMin& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool StockMin::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

StockMin StockMin::create(double open, double close, double high, double low, double vol, double amount,
                          const QString& datetime, const QString& code, const QString& date, double dateStamp,
                          double timeStamp, const QString& type)
{
    StockMinObject obj;
    obj.open       = open;
    obj.close      = close;
    obj.high       = high;
    obj.low        = low;
    obj.vol        = vol;
    obj.amount     = amount;
    obj.datetime   = datetime;
    obj.code       = code;
    obj.date       = date;
    obj.date_stamp = dateStamp;
    obj.time_stamp = timeStamp;
    obj.type       = type;
    if (!obj.create()) {
        return StockMin();
    }
    return StockMin(obj);
}

StockMin StockMin::create(const QVariantMap& values)
{
    StockMin model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

StockMin StockMin::get(const QString& id)
{
    TMongoODMapper<StockMinObject> mapper;
    return StockMin(mapper.findByObjectId(id));
}

QList<StockMin> StockMin::get_price(const QStringList codes, double start, double end)
{
    TMongoODMapper<StockMinObject> mapper;
    TCriteria cri;
    // QDateTime start_ = QDateTime::fromString(start, Qt::ISODate);
    cri.add(StockMinObject::Code, TMongo::In,
            codes); // AND add to the end operator

    cri.add(StockMinObject::TimeStamp, TMongo::GreaterThan,
            start); // AND add to the end operator

    // QDateTime end_ = QDateTime::fromString(end, Qt::ISODate);
    cri.add(StockMinObject::TimeStamp, TMongo::LessEqual,
            end); // AND add to the end operator
    return tfGetModelListByMongoCriteria<StockMin, StockMinObject>(cri);
}

int StockMin::count()
{
    TMongoODMapper<StockMinObject> mapper;
    return mapper.findCount();
}

QList<StockMin> StockMin::getAll() { return tfGetModelListByMongoCriteria<StockMin, StockMinObject>(TCriteria()); }

QJsonArray StockMin::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<StockMinObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(StockMin(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* StockMin::modelData() { return d.data(); }

const TModelObject* StockMin::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const StockMin& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, StockMin& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(StockMin)
