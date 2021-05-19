#ifndef STOCKLIST_H
#define STOCKLIST_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>

class TModelObject;
class StockListObject;
class QJsonArray;

class T_MODEL_EXPORT StockList : public TAbstractModel
{
public:
    StockList();
    StockList(const StockList& other);
    StockList(const StockListObject& object);
    ~StockList();

    QString id() const;
    QString code() const;
    void setCode(const QString& code);
    int volunit() const;
    void setVolunit(int volunit);
    int decimalPoint() const;
    void setDecimalPoint(int decimalPoint);
    QString name() const;
    void setName(const QString& name);
    double preClose() const;
    void setPreClose(double preClose);
    QString sse() const;
    void setSse(const QString& sse);
    QString sec() const;
    void setSec(const QString& sec);
    StockList& operator=(const StockList& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static StockList create(const QString& code, int volunit, int decimalPoint, const QString& name, double preClose,
                            const QString& sse, const QString& sec);
    static StockList create(const QVariantMap& values);
    static StockList get(const QString& id);
    static QList<StockList> get_all_securities(double end = 0);
    static QList<StockList> get_all_securities(const QStringList codes, double end = 0);
    static int count();
    static QList<StockList> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<StockListObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const StockList& model);
    friend QDataStream& operator>>(QDataStream& ds, StockList& model);
};

Q_DECLARE_METATYPE(StockList)
Q_DECLARE_METATYPE(QList<StockList>)

#endif // STOCKLIST_H
