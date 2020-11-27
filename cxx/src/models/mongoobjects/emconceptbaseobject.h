#ifndef EMCONCEPTBASEOBJECT_H
#define EMCONCEPTBASEOBJECT_H

#include <QSharedData>
#include <TMongoObject>

class T_MODEL_EXPORT EmConceptBaseObject : public TMongoObject, public QSharedData
{
public:
    QString _id;
    int f104_bk_rise_cnt;
    int f105_bk_fall_cnt;
    QString f12_code;
    int f13_market;
    QString f14_name;
    double f3_bk_rise_fall_pct;
    double f8_turnover;

    enum PropertyIndex {
        Id = 0,
        F104BkRiseCnt,
        F105BkFallCnt,
        F12Code,
        F13Market,
        F14Name,
        F3BkRiseFallPct,
        F8Turnover,
    };

    virtual QString collectionName() const override { return QStringLiteral("em_concept_base"); }
    virtual QString objectId() const override { return _id; }
    virtual QString& objectId() override { return _id; }

private:
    Q_OBJECT
    Q_PROPERTY(QString _id READ get_id WRITE set_id)
    T_DEFINE_PROPERTY(QString, _id)
    Q_PROPERTY(int f104_bk_rise_cnt READ getf104_bk_rise_cnt WRITE setf104_bk_rise_cnt)
    T_DEFINE_PROPERTY(int, f104_bk_rise_cnt)
    Q_PROPERTY(int f105_bk_fall_cnt READ getf105_bk_fall_cnt WRITE setf105_bk_fall_cnt)
    T_DEFINE_PROPERTY(int, f105_bk_fall_cnt)
    Q_PROPERTY(QString f12_code READ getf12_code WRITE setf12_code)
    T_DEFINE_PROPERTY(QString, f12_code)
    Q_PROPERTY(int f13_market READ getf13_market WRITE setf13_market)
    T_DEFINE_PROPERTY(int, f13_market)
    Q_PROPERTY(QString f14_name READ getf14_name WRITE setf14_name)
    T_DEFINE_PROPERTY(QString, f14_name)
    Q_PROPERTY(double f3_bk_rise_fall_pct READ getf3_bk_rise_fall_pct WRITE setf3_bk_rise_fall_pct)
    T_DEFINE_PROPERTY(double, f3_bk_rise_fall_pct)
    Q_PROPERTY(double f8_turnover READ getf8_turnover WRITE setf8_turnover)
    T_DEFINE_PROPERTY(double, f8_turnover)
};

#endif // EMCONCEPTBASEOBJECT_H
