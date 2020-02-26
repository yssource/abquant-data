#include "abquant/models/indexday.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/indexdayobject.h"

IndexDay::IndexDay() : TAbstractModel(), d(new IndexDayObject())
{
    d->open       = 0;
    d->close      = 0;
    d->high       = 0;
    d->low        = 0;
    d->vol        = 0;
    d->amount     = 0;
    d->up_count   = 0;
    d->down_count = 0;
    d->date_stamp = 0;
}

IndexDay::IndexDay(const IndexDay& other) : TAbstractModel(), d(other.d) {}

IndexDay::IndexDay(const IndexDayObject& object) : TAbstractModel(), d(new IndexDayObject(object)) {}

IndexDay::~IndexDay()
{
    // If the reference count becomes 0,
    // the shared data object 'IndexDayObject' is deleted.
}

QString IndexDay::id() const { return d->_id; }

double IndexDay::open() const { return d->open; }

void IndexDay::setOpen(double open) { d->open = open; }

double IndexDay::close() const { return d->close; }

void IndexDay::setClose(double close) { d->close = close; }

double IndexDay::high() const { return d->high; }

void IndexDay::setHigh(double high) { d->high = high; }

double IndexDay::low() const { return d->low; }

void IndexDay::setLow(double low) { d->low = low; }

double IndexDay::vol() const { return d->vol; }

void IndexDay::setVol(double vol) { d->vol = vol; }

double IndexDay::amount() const { return d->amount; }

void IndexDay::setAmount(double amount) { d->amount = amount; }

int IndexDay::upCount() const { return d->up_count; }

void IndexDay::setUpCount(int upCount) { d->up_count = upCount; }

int IndexDay::downCount() const { return d->down_count; }

void IndexDay::setDownCount(int downCount) { d->down_count = downCount; }

QString IndexDay::date() const { return d->date; }

void IndexDay::setDate(const QString& date) { d->date = date; }

QString IndexDay::code() const { return d->code; }

void IndexDay::setCode(const QString& code) { d->code = code; }

double IndexDay::dateStamp() const { return d->date_stamp; }

void IndexDay::setDateStamp(double dateStamp) { d->date_stamp = dateStamp; }

IndexDay& IndexDay::operator=(const IndexDay& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool IndexDay::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

IndexDay IndexDay::create(double open, double close, double high, double low, double vol, double amount, int upCount,
                          int downCount, const QString& date, const QString& code, double dateStamp)
{
    IndexDayObject obj;
    obj.open       = open;
    obj.close      = close;
    obj.high       = high;
    obj.low        = low;
    obj.vol        = vol;
    obj.amount     = amount;
    obj.up_count   = upCount;
    obj.down_count = downCount;
    obj.date       = date;
    obj.code       = code;
    obj.date_stamp = dateStamp;
    if (!obj.create()) {
        return IndexDay();
    }
    return IndexDay(obj);
}

IndexDay IndexDay::create(const QVariantMap& values)
{
    IndexDay model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

IndexDay IndexDay::get(const QString& id)
{
    TMongoODMapper<IndexDayObject> mapper;
    return IndexDay(mapper.findByObjectId(id));
}

QList<IndexDay> IndexDay::get_price(const QStringList& codes, double start, double end)
{
    TMongoODMapper<IndexDayObject> mapper;
    // mapper.setSortOrder(IndexDayObject::Date, Tf::DescendingOrder);
    TCriteria cri;
    // QDateTime start_ = QDateTime::fromString(start, Qt::ISODate);
    cri.add(IndexDayObject::Code, TMongo::In,
            codes); // AND add to the end operator

    cri.add(IndexDayObject::DateStamp, TMongo::GreaterThan,
            start); // AND add to the end operator

    // QDateTime end_ = QDateTime::fromString(end, Qt::ISODate);
    cri.add(IndexDayObject::DateStamp, TMongo::LessEqual,
            end); // AND add to the end operator
    return tfGetModelListByMongoCriteria<IndexDay, IndexDayObject>(cri);
}

int IndexDay::count()
{
    TMongoODMapper<IndexDayObject> mapper;
    return mapper.findCount();
}

QList<IndexDay> IndexDay::getAll() { return tfGetModelListByMongoCriteria<IndexDay, IndexDayObject>(TCriteria()); }

QJsonArray IndexDay::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<IndexDayObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(IndexDay(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* IndexDay::modelData() { return d.data(); }

const TModelObject* IndexDay::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const IndexDay& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, IndexDay& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(IndexDay)
