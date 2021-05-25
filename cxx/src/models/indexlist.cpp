#include "abquant/models/indexlist.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/indexlistobject.h"

IndexList::IndexList() : TAbstractModel(), d(new IndexListObject())
{
    d->volunit       = 0;
    d->decimal_point = 0;
    d->pre_close     = 0;
}

IndexList::IndexList(const IndexList& other) : TAbstractModel(), d(other.d) {}

IndexList::IndexList(const IndexListObject& object) : TAbstractModel(), d(new IndexListObject(object)) {}

IndexList::~IndexList()
{
    // If the reference count becomes 0,
    // the shared data object 'IndexListObject' is deleted.
}

QString IndexList::id() const { return d->_id; }

QString IndexList::code() const { return d->code; }

void IndexList::setCode(const QString& code) { d->code = code; }

int IndexList::volunit() const { return d->volunit; }

void IndexList::setVolunit(int volunit) { d->volunit = volunit; }

int IndexList::decimalPoint() const { return d->decimal_point; }

void IndexList::setDecimalPoint(int decimal_point) { d->decimal_point = decimal_point; }

QString IndexList::name() const { return d->name; }

void IndexList::setName(const QString& name) { d->name = name; }

double IndexList::preClose() const { return d->pre_close; }

void IndexList::setPreClose(double pre_close) { d->pre_close = pre_close; }

QString IndexList::sse() const { return d->sse; }

void IndexList::setSse(const QString& sse) { d->sse = sse; }

QString IndexList::sec() const { return d->sec; }

void IndexList::setSec(const QString& sec) { d->sec = sec; }

IndexList& IndexList::operator=(const IndexList& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool IndexList::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

IndexList IndexList::create(const QString& code, int volunit, int decimal_point, const QString& name, double pre_close,
                            const QString& sse, const QString& sec)
{
    IndexListObject obj;
    obj.code          = code;
    obj.volunit       = volunit;
    obj.decimal_point = decimal_point;
    obj.name          = name;
    obj.pre_close     = pre_close;
    obj.sse           = sse;
    obj.sec           = sec;
    if (!obj.create()) {
        return IndexList();
    }
    return IndexList(obj);
}

IndexList IndexList::create(const QVariantMap& values)
{
    IndexList model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

IndexList IndexList::get(const QString& id)
{
    TMongoODMapper<IndexListObject> mapper;
    return IndexList(mapper.findByObjectId(id));
}

QList<IndexList> IndexList::get_all_securities(double end)
{
    return tfGetModelListByMongoCriteria<IndexList, IndexListObject>(TCriteria());
}

QList<IndexList> IndexList::get_all_securities(const QStringList codes, double end)
{
    TMongoODMapper<IndexListObject> mapper;
    TCriteria cri;
    // QDateTime start_ = QDateTime::fromString(start, Qt::ISODate);
    cri.add(IndexListObject::Code, TMongo::In,
            codes); // AND add to the end operator

    // QDateTime end_ = QDateTime::fromString(end, Qt::ISODate);
    // cri.add(IndexListObject::DateStamp, TMongo::LessEqual,
    //         end); // AND add to the end operator
    return tfGetModelListByMongoCriteria<IndexList, IndexListObject>(cri);
}

int IndexList::count()
{
    TMongoODMapper<IndexListObject> mapper;
    return mapper.findCount();
}

QList<IndexList> IndexList::getAll() { return tfGetModelListByMongoCriteria<IndexList, IndexListObject>(TCriteria()); }

QJsonArray IndexList::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<IndexListObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(IndexList(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* IndexList::modelData() { return d.data(); }

const TModelObject* IndexList::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const IndexList& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, IndexList& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(IndexList)
