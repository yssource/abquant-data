#include "abquant/models/etflist.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/etflistobject.h"
EtfList::EtfList() : TAbstractModel(), d(new EtfListObject())
{
    d->volunit       = 0;
    d->decimal_point = 0;
    d->pre_close     = 0;
}

EtfList::EtfList(const EtfList& other) : TAbstractModel(), d(other.d) {}

EtfList::EtfList(const EtfListObject& object) : TAbstractModel(), d(new EtfListObject(object)) {}

EtfList::~EtfList()
{
    // If the reference count becomes 0,
    // the shared data object 'EtfListObject' is deleted.
}

QString EtfList::id() const { return d->_id; }

QString EtfList::code() const { return d->code; }

void EtfList::setCode(const QString& code) { d->code = code; }

int EtfList::volunit() const { return d->volunit; }

void EtfList::setVolunit(int volunit) { d->volunit = volunit; }

int EtfList::decimalPoint() const { return d->decimal_point; }

void EtfList::setDecimalPoint(int decimal_point) { d->decimal_point = decimal_point; }

QString EtfList::name() const { return d->name; }

void EtfList::setName(const QString& name) { d->name = name; }

double EtfList::preClose() const { return d->pre_close; }

void EtfList::setPreClose(double pre_close) { d->pre_close = pre_close; }

QString EtfList::sse() const { return d->sse; }

void EtfList::setSse(const QString& sse) { d->sse = sse; }

QString EtfList::sec() const { return d->sec; }

void EtfList::setSec(const QString& sec) { d->sec = sec; }

EtfList& EtfList::operator=(const EtfList& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool EtfList::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

EtfList EtfList::create(const QString& code, int volunit, int decimal_point, const QString& name, double pre_close,
                            const QString& sse, const QString& sec)
{
    EtfListObject obj;
    obj.code          = code;
    obj.volunit       = volunit;
    obj.decimal_point = decimal_point;
    obj.name          = name;
    obj.pre_close     = pre_close;
    obj.sse           = sse;
    obj.sec           = sec;
    if (!obj.create()) {
        return EtfList();
    }
    return EtfList(obj);
}

EtfList EtfList::create(const QVariantMap& values)
{
    EtfList model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

EtfList EtfList::get(const QString& id)
{
    TMongoODMapper<EtfListObject> mapper;
    return EtfList(mapper.findByObjectId(id));
}

QList<EtfList> EtfList::get_all_securities(double end)
{
    return tfGetModelListByMongoCriteria<EtfList, EtfListObject>(TCriteria());
}

QList<EtfList> EtfList::get_all_securities(const QStringList codes, double end)
{
    TMongoODMapper<EtfListObject> mapper;
    TCriteria cri;
    // QDateTime start_ = QDateTime::fromString(start, Qt::ISODate);
    cri.add(EtfListObject::Code, TMongo::In,
            codes); // AND add to the end operator

    // QDateTime end_ = QDateTime::fromString(end, Qt::ISODate);
    // cri.add(EtfListObject::DateStamp, TMongo::LessEqual,
    //         end); // AND add to the end operator
    return tfGetModelListByMongoCriteria<EtfList, EtfListObject>(cri);
}

int EtfList::count()
{
    TMongoODMapper<EtfListObject> mapper;
    return mapper.findCount();
}

QList<EtfList> EtfList::getAll() { return tfGetModelListByMongoCriteria<EtfList, EtfListObject>(TCriteria()); }

QJsonArray EtfList::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<EtfListObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(EtfList(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* EtfList::modelData() { return d.data(); }

const TModelObject* EtfList::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const EtfList& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, EtfList& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(EtfList)
