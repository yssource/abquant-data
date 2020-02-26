#ifndef INDEXDAY_H
#define INDEXDAY_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>

class TModelObject;
class IndexDayObject;
class QJsonArray;

class T_MODEL_EXPORT IndexDay : public TAbstractModel
{
public:
    IndexDay();
    IndexDay(const IndexDay& other);
    IndexDay(const IndexDayObject& object);
    ~IndexDay();

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
    int upCount() const;
    void setUpCount(int upCount);
    int downCount() const;
    void setDownCount(int downCount);
    QString date() const;
    void setDate(const QString& date);
    QString code() const;
    void setCode(const QString& code);
    double dateStamp() const;
    void setDateStamp(double dateStamp);
    IndexDay& operator=(const IndexDay& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static IndexDay create(double open, double close, double high, double low, double vol, double amount, int upCount,
                           int downCount, const QString& date, const QString& code, double dateStamp);
    static IndexDay create(const QVariantMap& values);
    static IndexDay get(const QString& id);
    static QList<IndexDay> get_price(const QStringList& codes, double start, double end);
    static int count();
    static QList<IndexDay> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<IndexDayObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const IndexDay& model);
    friend QDataStream& operator>>(QDataStream& ds, IndexDay& model);
};

Q_DECLARE_METATYPE(IndexDay)
Q_DECLARE_METATYPE(QList<IndexDay>)

#endif // INDEXDAY_H
