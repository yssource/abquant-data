#include "abquant/models/stockxdxr.h"

#include <TreeFrogModel>

#include "abquant/models/mongoobjects/stockxdxrobject.h"

StockXdxr::StockXdxr() : TAbstractModel(), d(new StockXdxrObject())
{
    d->category         = 0;
    d->fenhong          = 0;
    d->peigujia         = 0;
    d->songzhuangu      = 0;
    d->peigu            = 0;
    d->suogu            = 0;
    d->liquidity_before = 0;
    d->liquidity_after  = 0;
    d->shares_before    = 0;
    d->shares_after     = 0;
    d->fenshu           = 0;
    d->xingquanjia      = 0;
}

StockXdxr::StockXdxr(const StockXdxr& other) : TAbstractModel(), d(other.d) {}

StockXdxr::StockXdxr(const StockXdxrObject& object) : TAbstractModel(), d(new StockXdxrObject(object)) {}

StockXdxr::~StockXdxr()
{
    // If the reference count becomes 0,
    // the shared data object 'StockXdxrObject' is deleted.
}

QString StockXdxr::id() const { return d->_id; }

int StockXdxr::category() const { return d->category; }

void StockXdxr::setCategory(int category) { d->category = category; }

QString StockXdxr::name() const { return d->name; }

void StockXdxr::setName(const QString& name) { d->name = name; }

double StockXdxr::fenhong() const { return d->fenhong; }

void StockXdxr::setFenhong(double fenhong) { d->fenhong = fenhong; }

double StockXdxr::peigujia() const { return d->peigujia; }

void StockXdxr::setPeigujia(double peigujia) { d->peigujia = peigujia; }

double StockXdxr::songzhuangu() const { return d->songzhuangu; }

void StockXdxr::setSongzhuangu(double songzhuangu) { d->songzhuangu = songzhuangu; }

double StockXdxr::peigu() const { return d->peigu; }

void StockXdxr::setPeigu(double peigu) { d->peigu = peigu; }

double StockXdxr::suogu() const { return d->suogu; }

void StockXdxr::setSuogu(double suogu) { d->suogu = suogu; }

double StockXdxr::liquidityBefore() const { return d->liquidity_before; }

void StockXdxr::setLiquidityBefore(double liquidityBefore) { d->liquidity_before = liquidityBefore; }

double StockXdxr::liquidityAfter() const { return d->liquidity_after; }

void StockXdxr::setLiquidityAfter(double liquidityAfter) { d->liquidity_after = liquidityAfter; }

double StockXdxr::sharesBefore() const { return d->shares_before; }

void StockXdxr::setSharesBefore(double sharesBefore) { d->shares_before = sharesBefore; }

double StockXdxr::sharesAfter() const { return d->shares_after; }

void StockXdxr::setSharesAfter(double sharesAfter) { d->shares_after = sharesAfter; }

double StockXdxr::fenshu() const { return d->fenshu; }

void StockXdxr::setFenshu(double fenshu) { d->fenshu = fenshu; }

double StockXdxr::xingquanjia() const { return d->xingquanjia; }

void StockXdxr::setXingquanjia(double xingquanjia) { d->xingquanjia = xingquanjia; }

QString StockXdxr::date() const { return d->date; }

void StockXdxr::setDate(const QString& date) { d->date = date; }

QString StockXdxr::categoryMeaning() const { return d->category_meaning; }

void StockXdxr::setCategoryMeaning(const QString& categoryMeaning) { d->category_meaning = categoryMeaning; }

QString StockXdxr::code() const { return d->code; }

void StockXdxr::setCode(const QString& code) { d->code = code; }

StockXdxr& StockXdxr::operator=(const StockXdxr& other)
{
    d = other.d; // increments the reference count of the data
    return *this;
}

bool StockXdxr::upsert(const QVariantMap& criteria)
{
    auto* obj = dynamic_cast<TMongoObject*>(modelData());
    return (obj) ? obj->upsert(criteria) : false;
}

StockXdxr StockXdxr::create(int category, const QString& name, double fenhong, double peigujia, double songzhuangu,
                            double peigu, double suogu, double liquidityBefore, double liquidityAfter,
                            double sharesBefore, double sharesAfter, double fenshu, double xingquanjia,
                            const QString& date, const QString& categoryMeaning, const QString& code)
{
    StockXdxrObject obj;
    obj.category         = category;
    obj.name             = name;
    obj.fenhong          = fenhong;
    obj.peigujia         = peigujia;
    obj.songzhuangu      = songzhuangu;
    obj.peigu            = peigu;
    obj.suogu            = suogu;
    obj.liquidity_before = liquidityBefore;
    obj.liquidity_after  = liquidityAfter;
    obj.shares_before    = sharesBefore;
    obj.shares_after     = sharesAfter;
    obj.fenshu           = fenshu;
    obj.xingquanjia      = xingquanjia;
    obj.date             = date;
    obj.category_meaning = categoryMeaning;
    obj.code             = code;
    if (!obj.create()) {
        return StockXdxr();
    }
    return StockXdxr(obj);
}

StockXdxr StockXdxr::create(const QVariantMap& values)
{
    StockXdxr model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

StockXdxr StockXdxr::get(const QString& id)
{
    TMongoODMapper<StockXdxrObject> mapper;
    return StockXdxr(mapper.findByObjectId(id));
}

QList<StockXdxr> StockXdxr::get_price(const QStringList codes, int category)
{
    TMongoODMapper<StockXdxrObject> mapper;
    TCriteria cri;
    cri.add(StockXdxrObject::Code, TMongo::In,
            codes); // AND add to the end operator
    cri.add(StockXdxrObject::Category, TMongo::Equal,
            category); // AND add to the end operator
    return tfGetModelListByMongoCriteria<StockXdxr, StockXdxrObject>(cri);
}

int StockXdxr::count()
{
    TMongoODMapper<StockXdxrObject> mapper;
    return mapper.findCount();
}

QList<StockXdxr> StockXdxr::getAll() { return tfGetModelListByMongoCriteria<StockXdxr, StockXdxrObject>(TCriteria()); }

QJsonArray StockXdxr::getAllJson()
{
    QJsonArray array;
    TMongoODMapper<StockXdxrObject> mapper;

    if (mapper.find()) {
        while (mapper.next()) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(StockXdxr(mapper.value()).toVariantMap())));
        }
    }
    return array;
}

TModelObject* StockXdxr::modelData() { return d.data(); }

const TModelObject* StockXdxr::modelData() const { return d.data(); }

QDataStream& operator<<(QDataStream& ds, const StockXdxr& model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, StockXdxr& model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(StockXdxr)
