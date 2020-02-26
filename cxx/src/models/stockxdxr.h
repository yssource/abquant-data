#ifndef STOCKXDXR_H
#define STOCKXDXR_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>

class TModelObject;
class StockXdxrObject;
class QJsonArray;

class T_MODEL_EXPORT StockXdxr : public TAbstractModel
{
public:
    StockXdxr();
    StockXdxr(const StockXdxr& other);
    StockXdxr(const StockXdxrObject& object);
    ~StockXdxr();

    QString id() const;
    int category() const;
    void setCategory(int category);
    QString name() const;
    void setName(const QString& name);
    double fenhong() const;
    void setFenhong(double fenhong);
    double peigujia() const;
    void setPeigujia(double peigujia);
    double songzhuangu() const;
    void setSongzhuangu(double songzhuangu);
    double peigu() const;
    void setPeigu(double peigu);
    double suogu() const;
    void setSuogu(double suogu);
    double liquidityBefore() const;
    void setLiquidityBefore(double liquidityBefore);
    double liquidityAfter() const;
    void setLiquidityAfter(double liquidityAfter);
    double sharesBefore() const;
    void setSharesBefore(double sharesBefore);
    double sharesAfter() const;
    void setSharesAfter(double sharesAfter);
    double fenshu() const;
    void setFenshu(double fenshu);
    double xingquanjia() const;
    void setXingquanjia(double xingquanjia);
    QString date() const;
    void setDate(const QString& date);
    QString categoryMeaning() const;
    void setCategoryMeaning(const QString& categoryMeaning);
    QString code() const;
    void setCode(const QString& code);
    StockXdxr& operator=(const StockXdxr& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static StockXdxr create(int category, const QString& name, double fenhong, double peigujia, double songzhuangu,
                            double peigu, double suogu, double liquidityBefore, double liquidityAfter,
                            double sharesBefore, double sharesAfter, double fenshu, double xingquanjia,
                            const QString& date, const QString& categoryMeaning, const QString& code);
    static StockXdxr create(const QVariantMap& values);
    static StockXdxr get(const QString& id);
    static QList<StockXdxr> get_price(const QStringList codes, int category);
    static int count();
    static QList<StockXdxr> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<StockXdxrObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const StockXdxr& model);
    friend QDataStream& operator>>(QDataStream& ds, StockXdxr& model);
};

Q_DECLARE_METATYPE(StockXdxr)
Q_DECLARE_METATYPE(QList<StockXdxr>)

#endif // STOCKXDXR_H
