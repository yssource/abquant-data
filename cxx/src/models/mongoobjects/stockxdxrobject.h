#ifndef STOCKXDXROBJECT_H
#define STOCKXDXROBJECT_H

#include <QSharedData>
#include <TMongoObject>

class T_MODEL_EXPORT StockXdxrObject : public TMongoObject, public QSharedData
{
public:
    QString _id;
    int category;
    QString name;
    double fenhong;
    double peigujia;
    double songzhuangu;
    double peigu;
    double suogu;
    double liquidity_before;
    double liquidity_after;
    double shares_before;
    double shares_after;
    double fenshu;
    double xingquanjia;
    QString date;
    QString category_meaning;
    QString code;

    enum PropertyIndex {
        Id = 0,
        Category,
        Name,
        Fenhong,
        Peigujia,
        Songzhuangu,
        Peigu,
        Suogu,
        LiquidityBefore,
        LiquidityAfter,
        SharesBefore,
        SharesAfter,
        Fenshu,
        Xingquanjia,
        Date,
        CategoryMeaning,
        Code,
    };

    virtual QString collectionName() const override { return QStringLiteral("stock_xdxr"); }
    virtual QString objectId() const override { return _id; }
    virtual QString& objectId() override { return _id; }

private:
    Q_OBJECT
    Q_PROPERTY(QString _id READ get_id WRITE set_id)
    T_DEFINE_PROPERTY(QString, _id)
    Q_PROPERTY(int category READ getcategory WRITE setcategory)
    T_DEFINE_PROPERTY(int, category)
    Q_PROPERTY(QString name READ getname WRITE setname)
    T_DEFINE_PROPERTY(QString, name)
    Q_PROPERTY(double fenhong READ getfenhong WRITE setfenhong)
    T_DEFINE_PROPERTY(double, fenhong)
    Q_PROPERTY(double peigujia READ getpeigujia WRITE setpeigujia)
    T_DEFINE_PROPERTY(double, peigujia)
    Q_PROPERTY(double songzhuangu READ getsongzhuangu WRITE setsongzhuangu)
    T_DEFINE_PROPERTY(double, songzhuangu)
    Q_PROPERTY(double peigu READ getpeigu WRITE setpeigu)
    T_DEFINE_PROPERTY(double, peigu)
    Q_PROPERTY(double suogu READ getsuogu WRITE setsuogu)
    T_DEFINE_PROPERTY(double, suogu)
    Q_PROPERTY(double liquidity_before READ getliquidity_before WRITE setliquidity_before)
    T_DEFINE_PROPERTY(double, liquidity_before)
    Q_PROPERTY(double liquidity_after READ getliquidity_after WRITE setliquidity_after)
    T_DEFINE_PROPERTY(double, liquidity_after)
    Q_PROPERTY(double shares_before READ getshares_before WRITE setshares_before)
    T_DEFINE_PROPERTY(double, shares_before)
    Q_PROPERTY(double shares_after READ getshares_after WRITE setshares_after)
    T_DEFINE_PROPERTY(double, shares_after)
    Q_PROPERTY(double fenshu READ getfenshu WRITE setfenshu)
    T_DEFINE_PROPERTY(double, fenshu)
    Q_PROPERTY(double xingquanjia READ getxingquanjia WRITE setxingquanjia)
    T_DEFINE_PROPERTY(double, xingquanjia)
    Q_PROPERTY(QString date READ getdate WRITE setdate)
    T_DEFINE_PROPERTY(QString, date)
    Q_PROPERTY(QString category_meaning READ getcategory_meaning WRITE setcategory_meaning)
    T_DEFINE_PROPERTY(QString, category_meaning)
    Q_PROPERTY(QString code READ getcode WRITE setcode)
    T_DEFINE_PROPERTY(QString, code)
};

#endif // STOCKXDXROBJECT_H
