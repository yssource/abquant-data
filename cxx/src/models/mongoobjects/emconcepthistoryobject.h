#ifndef EMCONCEPTHISTORYOBJECT_H
#define EMCONCEPTHISTORYOBJECT_H

#include <QSharedData>
#include <TMongoObject>

class T_MODEL_EXPORT EmConceptHistoryObject : public TMongoObject, public QSharedData
{
public:
    QString _id;
    QString date;
    double open;
    double close;
    double high;
    double low;
    double volume;
    double amount;
    double amplitude;
    double rise_fall_pct;
    double rise_fall_amt;
    double turnover;
    QString mkt_code;
    double date_stamp;

    enum PropertyIndex {
        Id = 0,
        Date,
        Open,
        Close,
        High,
        Low,
        Volume,
        Amount,
        Amplitude,
        RiseFallPct,
        RiseFallAmt,
        Turnover,
        MktCode,
        DateStamp,
    };

    virtual QString collectionName() const override { return QStringLiteral("em_concept_hist"); }
    virtual QString objectId() const override { return _id; }
    virtual QString& objectId() override { return _id; }

private:
    Q_OBJECT
    Q_PROPERTY(QString _id READ get_id WRITE set_id)
    T_DEFINE_PROPERTY(QString, _id)
    Q_PROPERTY(QString date READ getdate WRITE setdate)
    T_DEFINE_PROPERTY(QString, date)
    Q_PROPERTY(double open READ getopen WRITE setopen)
    T_DEFINE_PROPERTY(double, open)
    Q_PROPERTY(double close READ getclose WRITE setclose)
    T_DEFINE_PROPERTY(double, close)
    Q_PROPERTY(double high READ gethigh WRITE sethigh)
    T_DEFINE_PROPERTY(double, high)
    Q_PROPERTY(double low READ getlow WRITE setlow)
    T_DEFINE_PROPERTY(double, low)
    Q_PROPERTY(double volume READ getvolume WRITE setvolume)
    T_DEFINE_PROPERTY(double, volume)
    Q_PROPERTY(double amount READ getamount WRITE setamount)
    T_DEFINE_PROPERTY(double, amount)
    Q_PROPERTY(double amplitude READ getamplitude WRITE setamplitude)
    T_DEFINE_PROPERTY(double, amplitude)
    Q_PROPERTY(double rise_fall_pct READ getrise_fall_pct WRITE setrise_fall_pct)
    T_DEFINE_PROPERTY(double, rise_fall_pct)
    Q_PROPERTY(double rise_fall_amt READ getrise_fall_amt WRITE setrise_fall_amt)
    T_DEFINE_PROPERTY(double, rise_fall_amt)
    Q_PROPERTY(double turnover READ getturnover WRITE setturnover)
    T_DEFINE_PROPERTY(double, turnover)
    Q_PROPERTY(QString mkt_code READ getmkt_code WRITE setmkt_code)
    T_DEFINE_PROPERTY(QString, mkt_code)
    Q_PROPERTY(double date_stamp READ getdate_stamp WRITE setdate_stamp)
    T_DEFINE_PROPERTY(double, date_stamp)
};

#endif // EMCONCEPTHISTORYOBJECT_H
