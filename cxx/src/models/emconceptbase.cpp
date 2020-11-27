#include "abquant/models/emconceptbase.h"

#include <TreeFrogModel>
// #include <algorithm>

#include "abquant/models/mongoobjects/emconceptbaseobject.h"

EmConceptBase::EmConceptBase() : TAbstractModel(), d(new EmConceptBaseObject())
{
    d->f104_bk_rise_cnt = 0;
    d->f105_bk_fall_cnt = 0;
    // d->f12_code            = 0;
    d->f13_market = 0;
    // d->f14_name            = 0;
    d->f3_bk_rise_fall_pct = 0;
    d->f8_turnover         = 0;
}

EmConceptBase::EmConceptBase(const EmConceptBase& other) : TAbstractModel(), d(other.d) {}

EmConceptBase::EmConceptBase(const EmConceptBaseObject& object) : TAbstractModel(), d(new EmConceptBaseObject(object))
{
}

EmConceptBase::~EmConceptBase()
{
    // If the reference count becomes 0,
    // the shared data object 'EmConceptBaseObject' is deleted.
}

QString EmConceptBase::id() const { return d->_id; }

int EmConceptBase::f104BkRiseCnt() const { return d->f104_bk_rise_cnt; }

void EmConceptBase::setF104BkRiseCnt(int f104_bk_rise_cnt) { d->f104_bk_rise_cnt = f104_bk_rise_cnt; }

int EmConceptBase::f105BkFallCnt() const { return d->f105_bk_fall_cnt; }

void EmConceptBase::setF105BkFallCnt(int f105_bk_fall_cnt) { d->f105_bk_fall_cnt = f105_bk_fall_cnt; }

QString EmConceptBase::f12Code() const { return d->f12_code; }

void EmConceptBase::setF12Code(QString f12_code) { d->f12_code = f12_code; }

int EmConceptBase::f13Market() const { return d->f13_market; }

void EmConceptBase::setF13Market(int f13_market) { d->f13_market = f13_market; }

QString EmConceptBase::f14Name() const { return d->f14_name; }

void EmConceptBase::setF14Name(QString f14_name) { d->f14_name = f14_name; }

double EmConceptBase::f3BkRiseFallPct() const { return d->f3_bk_rise_fall_pct; }

void EmConceptBase::setF3BkRiseFallPct(double f3_bk_rise_fall_pct) { d->f3_bk_rise_fall_pct = f3_bk_rise_fall_pct; }

double EmConceptBase::f8Turnover() const { return d->f8_turnover; }

void EmConceptBase::setF8Turnover(double f8Turnover) { d->f8_turnover = f8Turnover; }

EmConceptBase& EmConceptBase::operator=(const EmConceptBase& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool EmConceptBase::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

EmConceptBase EmConceptBase::create(int f104BkRiseCnt, int f105BkFallCnt, QString f12Code, int f13Market,
                                    QString f14Name, double f3BkRiseFallPct, double f8Turnover)
{
    EmConceptBaseObject obj;
    obj.f104_bk_rise_cnt    = f104BkRiseCnt;
    obj.f105_bk_fall_cnt    = f105BkFallCnt;
    obj.f12_code            = f12Code;
    obj.f13_market          = f13Market;
    obj.f14_name            = f14Name;
    obj.f3_bk_rise_fall_pct = f3BkRiseFallPct;
    obj.f8_turnover         = f8Turnover;
    if (!obj.create()) {
        return EmConceptBase();
    }
    return EmConceptBase(obj);
}

EmConceptBase EmConceptBase::create(const QVariantMap& values)
{
    EmConceptBase model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

EmConceptBase EmConceptBase::get(const QString& id)
{
    TMongoODMapper<EmConceptBaseObject> mapper;
    return EmConceptBase(mapper.findByObjectId(id));
}

QList<EmConceptBase> EmConceptBase::get_blocks(QStringList& codes)
{
    foreach (QString code, codes)
        codes << code.toUpper();
    TMongoODMapper<EmConceptBaseObject> mapper;
    // mapper.setSortOrder(EmConceptBaseObject::F12Code, Tf::DescendingOrder);
    TCriteria cri;
    cri.add(EmConceptBaseObject::F12Code, TMongo::In,
            codes); // AND add to the end operator
    return tfGetModelListByMongoCriteria<EmConceptBase, EmConceptBaseObject>(cri);
}

int EmConceptBase::count()
{
    TMongoODMapper<EmConceptBaseObject> mapper;
    return mapper.findCount();
}

QList<EmConceptBase> EmConceptBase::getAll()
{
    return tfGetModelListByMongoCriteria<EmConceptBase, EmConceptBaseObject>(TCriteria());
}

QJsonArray EmConceptBase::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<EmConceptBaseObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(EmConceptBase(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* EmConceptBase::modelData() { return d.data(); }

const TModelObject* EmConceptBase::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const EmConceptBase& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, EmConceptBase& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(EmConceptBase)
