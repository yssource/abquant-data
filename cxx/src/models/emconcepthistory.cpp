#include "abquant/models/emconcepthistory.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/emconcepthistoryobject.h"
#include "iostream"

EmConceptHistory::EmConceptHistory() : TAbstractModel(), d(new EmConceptHistoryObject())
{
    d->open          = 0;
    d->close         = 0;
    d->high          = 0;
    d->low           = 0;
    d->volume        = 0;
    d->amount        = 0;
    d->amplitude     = 0;
    d->rise_fall_pct = 0;
    d->rise_fall_amt = 0;
    d->turnover      = 0;
    d->date_stamp    = 0;
}

EmConceptHistory::EmConceptHistory(const EmConceptHistory& other) : TAbstractModel(), d(other.d) {}

EmConceptHistory::EmConceptHistory(const EmConceptHistoryObject& object)
    : TAbstractModel(), d(new EmConceptHistoryObject(object))
{
}

EmConceptHistory::~EmConceptHistory()
{
    // If the reference count becomes 0,
    // the shared data object 'EmConceptHistoryObject' is deleted.
}

QString EmConceptHistory::id() const { return d->_id; }

QString EmConceptHistory::date() const { return d->date; }

void EmConceptHistory::setDate(const QString& date) { d->date = date; }

double EmConceptHistory::open() const { return d->open; }

void EmConceptHistory::setOpen(double open) { d->open = open; }

double EmConceptHistory::close() const { return d->close; }

void EmConceptHistory::setClose(double close) { d->close = close; }

double EmConceptHistory::high() const { return d->high; }

void EmConceptHistory::setHigh(double high) { d->high = high; }

double EmConceptHistory::low() const { return d->low; }

void EmConceptHistory::setLow(double low) { d->low = low; }

double EmConceptHistory::volume() const { return d->volume; }

void EmConceptHistory::setVolume(double volume) { d->volume = volume; }

double EmConceptHistory::amount() const { return d->amount; }

void EmConceptHistory::setAmount(double amount) { d->amount = amount; }

double EmConceptHistory::amplitude() const { return d->amplitude; }

void EmConceptHistory::setAmplitude(double amplitude) { d->amplitude = amplitude; }

double EmConceptHistory::riseFallPct() const { return d->rise_fall_pct; }

void EmConceptHistory::setRiseFallPct(double riseFallPct) { d->rise_fall_pct = riseFallPct; }

double EmConceptHistory::riseFallAmt() const { return d->rise_fall_amt; }

void EmConceptHistory::setRiseFallAmt(double riseFallAmt) { d->rise_fall_amt = riseFallAmt; }

double EmConceptHistory::turnover() const { return d->turnover; }

void EmConceptHistory::setTurnover(double turnover) { d->turnover = turnover; }

QString EmConceptHistory::mktCode() const { return d->mkt_code; }

void EmConceptHistory::setMktCode(const QString& mktCode) { d->mkt_code = mktCode; }

double EmConceptHistory::dateStamp() const { return d->date_stamp; }

void EmConceptHistory::setDateStamp(double dateStamp) { d->date_stamp = dateStamp; }

EmConceptHistory& EmConceptHistory::operator=(const EmConceptHistory& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool EmConceptHistory::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

EmConceptHistory EmConceptHistory::create(const QString& date, double open, double close, double high, double low,
                                          double volume, double amount, double amplitude, double riseFallPct,
                                          double riseFallAmt, double turnover, const QString& mktCode, double dateStamp)
{
    EmConceptHistoryObject obj;
    obj.date          = date;
    obj.open          = open;
    obj.close         = close;
    obj.high          = high;
    obj.low           = low;
    obj.volume        = volume;
    obj.amount        = amount;
    obj.amplitude     = amplitude;
    obj.rise_fall_pct = riseFallPct;
    obj.rise_fall_amt = riseFallAmt;
    obj.turnover      = turnover;
    obj.mkt_code      = mktCode;
    obj.date_stamp    = dateStamp;
    if (!obj.create()) {
        return EmConceptHistory();
    }
    return EmConceptHistory(obj);
}

EmConceptHistory EmConceptHistory::create(const QVariantMap& values)
{
    EmConceptHistory model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

EmConceptHistory EmConceptHistory::get(const QString& id)
{
    TMongoODMapper<EmConceptHistoryObject> mapper;
    return EmConceptHistory(mapper.findByObjectId(id));
}

QList<EmConceptHistory> EmConceptHistory::get_price(const QStringList& cst_codes, double start, double end)
{
    QStringList codes{};
    foreach (QString code, cst_codes)
        codes << code.toUpper();
    TMongoODMapper<EmConceptHistoryObject> mapper;
    // mapper.setSortOrder(EmConceptHistoryObject::Date, Tf::DescendingOrder);
    TCriteria cri;
    // QDateTime start_ = QDateTime::fromString(start, Qt::ISODate);
    cri.add(EmConceptHistoryObject::MktCode, TMongo::In,
            codes); // AND add to the end operator
    cri.add(EmConceptHistoryObject::DateStamp, TMongo::GreaterEqual,
            start); // AND add to the end operator

    // QDateTime end_ = QDateTime::fromString(end, Qt::ISODate);
    cri.add(EmConceptHistoryObject::DateStamp, TMongo::LessEqual,
            end); // AND add to the end operator
    return tfGetModelListByMongoCriteria<EmConceptHistory, EmConceptHistoryObject>(cri);
}

int EmConceptHistory::count()
{
    TMongoODMapper<EmConceptHistoryObject> mapper;
    return mapper.findCount();
}

QList<EmConceptHistory> EmConceptHistory::getAll()
{
    return tfGetModelListByMongoCriteria<EmConceptHistory, EmConceptHistoryObject>(TCriteria());
}

QJsonArray EmConceptHistory::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<EmConceptHistoryObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(EmConceptHistory(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* EmConceptHistory::modelData() { return d.data(); }

const TModelObject* EmConceptHistory::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const EmConceptHistory& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, EmConceptHistory& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(EmConceptHistory)
