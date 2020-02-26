#ifndef STOCKDAY_H
#define STOCKDAY_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>

class TModelObject;
class StockDayObject;
class QJsonArray;

class T_MODEL_EXPORT StockDay : public TAbstractModel
{
public:
    StockDay();
    StockDay(const StockDay& other);
    StockDay(const StockDayObject& object);
    ~StockDay();

    QString id() const;
    double open() const;
    void setOpen(double open);
    double close() const;
    void setClose(double close);
    double high() const;
    void setHigh(double high);
    double low() const;
    void setLow(double low);
    double vol() const;
    void setVol(double vol);
    double amount() const;
    void setAmount(double amount);
    QString date() const;
    void setDate(const QString& date);
    QString code() const;
    void setCode(const QString& code);
    double dateStamp() const;
    void setDateStamp(double dateStamp);
    StockDay& operator=(const StockDay& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static StockDay create(double open, double close, double high, double low, double vol, double amount,
                           const QString& date, const QString& code, double dateStamp);
    static StockDay create(const QVariantMap& values);
    static StockDay get(const QString& id);
    static QList<StockDay> get_price(const QStringList codes, double start, double end);
    static int count();
    static QList<StockDay> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<StockDayObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const StockDay& model);
    friend QDataStream& operator>>(QDataStream& ds, StockDay& model);
};

Q_DECLARE_METATYPE(StockDay)
Q_DECLARE_METATYPE(QList<StockDay>)

#endif // STOCKDAY_H
