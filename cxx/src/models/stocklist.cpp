#include "abquant/models/stocklist.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/stocklistobject.h"

StockList::StockList() : TAbstractModel(), d(new StockListObject())
{
    d->volunit       = 0;
    d->decimal_point = 0;
    d->pre_close     = 0;
}

StockList::StockList(const StockList& other) : TAbstractModel(), d(other.d) {}

StockList::StockList(const StockListObject& object) : TAbstractModel(), d(new StockListObject(object)) {}

StockList::~StockList()
{
    // If the reference count becomes 0,
    // the shared data object 'StockListObject' is deleted.
}

QString StockList::id() const { return d->_id; }

QString StockList::code() const { return d->code; }

void StockList::setCode(const QString& code) { d->code = code; }

int StockList::volunit() const { return d->volunit; }

void StockList::setVolunit(int volunit) { d->volunit = volunit; }

int StockList::decimalPoint() const { return d->decimal_point; }

void StockList::setDecimalPoint(int decimal_point) { d->decimal_point = decimal_point; }

QString StockList::name() const { return d->name; }

void StockList::setName(const QString& name) { d->name = name; }

double StockList::preClose() const { return d->pre_close; }

void StockList::setPreClose(double pre_close) { d->pre_close = pre_close; }

QString StockList::sse() const { return d->sse; }

void StockList::setSse(const QString& sse) { d->sse = sse; }

QString StockList::sec() const { return d->sec; }

void StockList::setSec(const QString& sec) { d->sec = sec; }

StockList& StockList::operator=(const StockList& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool StockList::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

StockList StockList::create(const QString& code, int volunit, int decimal_point, const QString& name, double pre_close,
                            const QString& sse, const QString& sec)
{
    StockListObject obj;
    obj.code          = code;
    obj.volunit       = volunit;
    obj.decimal_point = decimal_point;
    obj.name          = name;
    obj.pre_close     = pre_close;
    obj.sse           = sse;
    obj.sec           = sec;
    if (!obj.create()) {
        return StockList();
    }
    return StockList(obj);
}

StockList StockList::create(const QVariantMap& values)
{
    StockList model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

StockList StockList::get(const QString& id)
{
    TMongoODMapper<StockListObject> mapper;
    return StockList(mapper.findByObjectId(id));
}

QList<StockList> StockList::get_all_securities(double end)
{
    return tfGetModelListByMongoCriteria<StockList, StockListObject>(TCriteria());
}

QList<StockList> StockList::get_all_securities(const QStringList codes, double end)
{
    TMongoODMapper<StockListObject> mapper;
    TCriteria cri;
    // QDateTime start_ = QDateTime::fromString(start, Qt::ISODate);
    cri.add(StockListObject::Code, TMongo::In,
            codes); // AND add to the end operator

    // QDateTime end_ = QDateTime::fromString(end, Qt::ISODate);
    // cri.add(StockListObject::DateStamp, TMongo::LessEqual,
    //         end); // AND add to the end operator
    return tfGetModelListByMongoCriteria<StockList, StockListObject>(cri);
}

int StockList::count()
{
    TMongoODMapper<StockListObject> mapper;
    return mapper.findCount();
}

QList<StockList> StockList::getAll() { return tfGetModelListByMongoCriteria<StockList, StockListObject>(TCriteria()); }

QJsonArray StockList::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<StockListObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(StockList(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* StockList::modelData() { return d.data(); }

const TModelObject* StockList::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const StockList& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, StockList& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(StockList)
