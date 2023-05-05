#ifndef ETFLIST_H
#define ETFLIST_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>

class TModelObject;
class EtfListObject;
class QJsonArray;

class T_MODEL_EXPORT EtfList : public TAbstractModel
{
public:
    EtfList();
    EtfList(const EtfList& other);
    EtfList(const EtfListObject& object);
    ~EtfList();

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
    EtfList& operator=(const EtfList& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static EtfList create(const QString& code, int volunit, int decimalPoint, const QString& name, double preClose,
                            const QString& sse, const QString& sec);
    static EtfList create(const QVariantMap& values);
    static EtfList get(const QString& id);
    static QList<EtfList> get_all_securities(double end = 0);
    static QList<EtfList> get_all_securities(const QStringList codes, double end = 0);
    static int count();
    static QList<EtfList> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<EtfListObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const EtfList& model);
    friend QDataStream& operator>>(QDataStream& ds, EtfList& model);
};

Q_DECLARE_METATYPE(EtfList)
Q_DECLARE_METATYPE(QList<EtfList>)

#endif // ETFLIST_H
