#ifndef INDEXMIN_H
#define INDEXMIN_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>
// #include "abquant/helpers/applicationhelper.h"
#include "abquant/actions/utils.hpp"

class TModelObject;
class IndexMinObject;
class QJsonArray;

class T_MODEL_EXPORT IndexMin : public TAbstractModel
{
public:
    IndexMin();
    IndexMin(const IndexMin& other);
    IndexMin(const IndexMinObject& object);
    ~IndexMin();

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
    QString datetime() const;
    void setDatetime(const QString& datetime);
    qint32 upCount() const;
    void setUpCount(const qint32 upCount);
    qint32 downCount() const;
    void setDownCount(const qint32 downCount);
    QString code() const;
    void setCode(const QString& code);
    QString date() const;
    void setDate(const QString& date);
    double dateStamp() const;
    void setDateStamp(double dateStamp);
    double timeStamp() const;
    void setTimeStamp(double timeStamp);
    QString type() const;
    void setType(const QString& type);
    IndexMin& operator=(const IndexMin& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static IndexMin create(double open, double close, double high, double low, double vol, double amount,
                           const QString& datetime, qint32 upCount, qint32 downCount, const QString& code,
                           const QString& date, double dateStamp, double timeStamp, const QString& type);
    static IndexMin create(const QVariantMap& values);
    static IndexMin get(const QString& id);
    static QList<IndexMin> get_price(const QStringList codes, double start, double end,
                                     abq::MIN_FREQ freq = abq::MIN_FREQ::ONE);
    static int count();
    static QList<IndexMin> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<IndexMinObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const IndexMin& model);
    friend QDataStream& operator>>(QDataStream& ds, IndexMin& model);
};

Q_DECLARE_METATYPE(IndexMin)
Q_DECLARE_METATYPE(QList<IndexMin>)

#endif // INDEXMIN_H
