#ifndef INDEXLISTOBJECT_H
#define INDEXLISTOBJECT_H

#include <QSharedData>
#include <TMongoObject>

class T_MODEL_EXPORT IndexListObject : public TMongoObject, public QSharedData
{
public:
    QString _id;
    QString code;
    int volunit;
    int decimal_point;
    QString name;
    double pre_close;
    QString sse;
    QString sec;

    enum PropertyIndex {
        Id = 0,
        Code,
        Volunit,
        DecimalPoint,
        Name,
        PreClose,
        Sse,
        Sec,
    };

    virtual QString collectionName() const override { return QStringLiteral("index_list"); }
    virtual QString objectId() const override { return _id; }
    virtual QString& objectId() override { return _id; }

private:
    Q_OBJECT
    Q_PROPERTY(QString _id READ get_id WRITE set_id)
    T_DEFINE_PROPERTY(QString, _id)
    Q_PROPERTY(QString code READ getcode WRITE setcode)
    T_DEFINE_PROPERTY(QString, code)
    Q_PROPERTY(int volunit READ getvolunit WRITE setvolunit)
    T_DEFINE_PROPERTY(int, volunit)
    Q_PROPERTY(int decimal_point READ getdecimal_point WRITE setdecimal_point)
    T_DEFINE_PROPERTY(int, decimal_point)
    Q_PROPERTY(QString name READ getname WRITE setname)
    T_DEFINE_PROPERTY(QString, name)
    Q_PROPERTY(double pre_close READ getpre_close WRITE setpre_close)
    T_DEFINE_PROPERTY(double, pre_close)
    Q_PROPERTY(QString sse READ getsse WRITE setsse)
    T_DEFINE_PROPERTY(QString, sse)
    Q_PROPERTY(QString sec READ getsec WRITE setsec)
    T_DEFINE_PROPERTY(QString, sec)
};

#endif // INDEXLISTOBJECT_H
