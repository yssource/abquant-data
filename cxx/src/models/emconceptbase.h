#ifndef EMCONCEPTBASE_H
#define EMCONCEPTBASE_H

#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>

class TModelObject;
class EmConceptBaseObject;
class QJsonArray;

class T_MODEL_EXPORT EmConceptBase : public TAbstractModel
{
public:
    EmConceptBase();
    EmConceptBase(const EmConceptBase& other);
    EmConceptBase(const EmConceptBaseObject& object);
    ~EmConceptBase();

    QString id() const;
    int f104BkRiseCnt() const;
    void setF104BkRiseCnt(int f104BkRiseCnt);
    int f105BkFallCnt() const;
    void setF105BkFallCnt(int f105BkFallCnt);
    QString f12Code() const;
    void setF12Code(QString f12Code);
    int f13Market() const;
    void setF13Market(int f13Market);
    QString f14Name() const;
    void setF14Name(QString f14Name);
    double f3BkRiseFallPct() const;
    void setF3BkRiseFallPct(double f3BkRiseFallPct);
    double f8Turnover() const;
    void setF8Turnover(double f8Turnover);
    EmConceptBase& operator=(const EmConceptBase& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static EmConceptBase create(int f104BkRiseCnt, int f105BkFallCnt, QString f12Code, int f13Market, QString f14Name,
                                double f3BkRiseFallPct, double f8Turnover);
    static EmConceptBase create(const QVariantMap& values);
    static EmConceptBase get(const QString& id);
    static QList<EmConceptBase> get_blocks(const QStringList& cst_codes);
    static int count();
    static QList<EmConceptBase> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<EmConceptBaseObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const EmConceptBase& model);
    friend QDataStream& operator>>(QDataStream& ds, EmConceptBase& model);
};

Q_DECLARE_METATYPE(EmConceptBase)
Q_DECLARE_METATYPE(QList<EmConceptBase>)

#endif // EMCONCEPTBASE_H
