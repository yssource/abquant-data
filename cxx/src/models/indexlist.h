#ifndef INDEXLIST_H
#define INDEXLIST_H

#include <QDateTime>
#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>
#include <TAbstractModel>
#include <TGlobal>

class TModelObject;
class IndexListObject;
class QJsonArray;

class T_MODEL_EXPORT IndexList : public TAbstractModel
{
public:
    IndexList();
    IndexList(const IndexList& other);
    IndexList(const IndexListObject& object);
    ~IndexList();

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
    IndexList& operator=(const IndexList& other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool upsert(const QVariantMap& criteria);
    bool save() override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static IndexList create(const QString& code, int volunit, int decimalPoint, const QString& name, double preClose,
                            const QString& sse, const QString& sec);
    static IndexList create(const QVariantMap& values);
    static IndexList get(const QString& id);
    static QList<IndexList> get_all_securities(double end = 0);
    static QList<IndexList> get_all_securities(const QStringList codes, double end = 0);
    static int count();
    static QList<IndexList> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<IndexListObject> d;

    TModelObject* modelData() override;
    const TModelObject* modelData() const override;
    friend QDataStream& operator<<(QDataStream& ds, const IndexList& model);
    friend QDataStream& operator>>(QDataStream& ds, IndexList& model);
};

Q_DECLARE_METATYPE(IndexList)
Q_DECLARE_METATYPE(QList<IndexList>)

#endif // INDEXLIST_H
