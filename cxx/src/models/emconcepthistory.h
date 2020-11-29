#ifndef EMCONCEPTHISTORY_H
#define EMCONCEPTHISTORY_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>

class TModelObject;
class EmConceptHistoryObject;
class QJsonArray;

class T_MODEL_EXPORT EmConceptHistory : public TAbstractModel
{
public:
    EmConceptHistory();
    EmConceptHistory(const EmConceptHistory& other);
    EmConceptHistory(const EmConceptHistoryObject& object);
    ~EmConceptHistory();

    QString id() const;
    QString date() const;
    void setDate(const QString& date);
    double open() const;
    void setOpen(double open);
    double close() const;
    void setClose(double close);
    double high() const;
    void setHigh(double high);
    double low() const;
    void setLow(double low);
    double volume() const;
    void setVolume(double volume);
    double amount() const;
    void setAmount(double amount);
    double amplitude() const;
    void setAmplitude(double amplitude);
    double riseFallPct() const;
    void setRiseFallPct(double riseFallPct);
    double riseFallAmt() const;
    void setRiseFallAmt(double riseFallAmt);
    double turnover() const;
    void setTurnover(double turnover);
    QString mktCode() const;
    void setMktCode(const QString& mktCode);
    double dateStamp() const;
    void setDateStamp(double dateStamp);
    EmConceptHistory& operator=(const EmConceptHistory& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static EmConceptHistory create(const QString& date, double open, double close, double high, double low,
                                   double volume, double amount, double amplitude, double riseFallPct,
                                   double riseFallAmt, double turnover, const QString& mktCode, double dateStamp);
    static EmConceptHistory create(const QVariantMap& values);
    static EmConceptHistory get(const QString& id);
    static QList<EmConceptHistory> get_price(const QStringList& codes, double start, double end);
    static int count();
    static QList<EmConceptHistory> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<EmConceptHistoryObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const EmConceptHistory& model);
    friend QDataStream& operator>>(QDataStream& ds, EmConceptHistory& model);
};

Q_DECLARE_METATYPE(EmConceptHistory)
Q_DECLARE_METATYPE(QList<EmConceptHistory>)

#endif // EMCONCEPTHISTORY_H
