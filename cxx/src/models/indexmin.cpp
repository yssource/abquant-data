#include "abquant/models/indexmin.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/indexminobject.h"

IndexMin::IndexMin() : TAbstractModel(), d(new IndexMinObject())
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
    d->time_stamp = 0;
}

IndexMin::IndexMin(const IndexMin& other) : TAbstractModel(), d(other.d) {}

IndexMin::IndexMin(const IndexMinObject& object) : TAbstractModel(), d(new IndexMinObject(object)) {}

IndexMin::~IndexMin()
{
    // the shared data object 'IndexMinObject' is deleted.
}

QString IndexMin::id() const { return d->_id; }

double IndexMin::open() const { return d->open; }

double IndexMin::close() const { return d->close; }

void IndexMin::setClose(double close) { d->close = close; }

double IndexMin::high() const { return d->high; }

void IndexMin::setHigh(double high) { d->high = high; }

double IndexMin::low() const { return d->low; }

void IndexMin::setLow(double low) { d->low = low; }

double IndexMin::vol() const { return d->vol; }

void IndexMin::setVol(double vol) { d->vol = vol; }

double IndexMin::amount() const { return d->amount; }

void IndexMin::setAmount(double amount) { d->amount = amount; }

QString IndexMin::datetime() const { return d->datetime; }

void IndexMin::setDatetime(const QString& datetime) { d->datetime = datetime; }

qint32 IndexMin::upCount() const { return d->up_count; }

void IndexMin::setUpCount(const qint32 up_count) { d->up_count = up_count; }

qint32 IndexMin::downCount() const { return d->down_count; }

void IndexMin::setDownCount(const qint32 down_count) { d->down_count = down_count; }

QString IndexMin::code() const { return d->code; }

void IndexMin::setCode(const QString& code) { d->code = code; }

QString IndexMin::date() const { return d->date; }

void IndexMin::setDate(const QString& date) { d->date = date; }

double IndexMin::dateStamp() const { return d->date_stamp; }

void IndexMin::setDateStamp(double dateStamp) { d->date_stamp = dateStamp; }

double IndexMin::timeStamp() const { return d->time_stamp; }

void IndexMin::setTimeStamp(double timeStamp) { d->time_stamp = timeStamp; }

QString IndexMin::type() const { return d->type; }

void IndexMin::setType(const QString& type) { d->type = type; }

IndexMin& IndexMin::operator=(const IndexMin& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool IndexMin::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

IndexMin IndexMin::create(double open, double close, double high, double low, double vol, double amount,
                          const QString& datetime, qint32 upCount, qint32 downCount, const QString& code,
                          const QString& date, double dateStamp, double timeStamp, const QString& type)
{
    IndexMinObject obj;
    obj.open       = open;
    obj.close      = close;
    obj.high       = high;
    obj.low        = low;
    obj.vol        = vol;
    obj.amount     = amount;
    obj.datetime   = datetime;
    obj.up_count   = upCount;
    obj.down_count = downCount;
    obj.code       = code;
    obj.date       = date;
    obj.date_stamp = dateStamp;
    obj.time_stamp = timeStamp;
    obj.type       = type;
    if (!obj.create()) {
        return IndexMin();
    }
    return IndexMin(obj);
}

IndexMin IndexMin::create(const QVariantMap& values)
{
    IndexMin model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

IndexMin IndexMin::get(const QString& id)
{
    TMongoODMapper<IndexMinObject> mapper;
    return IndexMin(mapper.findByObjectId(id));
}

QList<IndexMin> IndexMin::get_price(const QStringList codes, double start, double end, abq::MIN_FREQ freq)
{
    TMongoODMapper<IndexMinObject> mapper;
    TCriteria cri;
    // QDateTime start_ = QDateTime::fromString(start, Qt::ISODate);
    cri.add(IndexMinObject::Code, TMongo::In,
            codes); // AND add to the end operator

    cri.add(IndexMinObject::TimeStamp, TMongo::GreaterThan,
            start); // AND add to the end operator

    // QDateTime end_ = QDateTime::fromString(end, Qt::ISODate);
    cri.add(IndexMinObject::TimeStamp, TMongo::LessEqual,
            end); // AND add to the end operator

    QString qsFreq = "";
    switch (freq) {
    case abq::MIN_FREQ::ONE:
        qsFreq = QString("1min");
        break;
    case abq::MIN_FREQ::FIVE:
        qsFreq = QString("5min");
        break;
    case abq::MIN_FREQ::FIFTEEN:
        qsFreq = QString("15min");
        break;
    case abq::MIN_FREQ::THIRTY:
        qsFreq = QString("30min");
        break;
    case abq::MIN_FREQ::SIXTY:
        qsFreq = QString("60min");
        break;
    default:
        qsFreq = QString("1min");
        break;
    }
    cri.add(IndexMinObject::Type, TMongo::Equal,
            qsFreq); // AND add to the end operator
    return tfGetModelListByMongoCriteria<IndexMin, IndexMinObject>(cri);
}

int IndexMin::count()
{
    TMongoODMapper<IndexMinObject> mapper;
    return mapper.findCount();
}

QList<IndexMin> IndexMin::getAll() { return tfGetModelListByMongoCriteria<IndexMin, IndexMinObject>(TCriteria()); }

QJsonArray IndexMin::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<IndexMinObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(IndexMin(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* IndexMin::modelData() { return d.data(); }

const TModelObject* IndexMin::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const IndexMin& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, IndexMin& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(IndexMin)
