#include "stock.hpp"

namespace abq
{
template <class SA>
class StockActionPrivate
{
public:
    StockActionPrivate();
    virtual ~StockActionPrivate();

    static StockActionPrivate<SA>* get(StockAction<SA>* s) { return s->d_func(); }
    static const StockActionPrivate<SA>* get(const StockAction<SA>* s) { return s->d_func(); }

    QList<S> run(const QStringList codes, const char* start, const char* end);
    QList<S> run(const QStringList codes, int category = 1);

    QList<S> get_price(const QStringList codes, const char* start, const char* end);
    QList<S> get_price(const QStringList codes, int category);

private:
    ABQ_DECLARE_PUBLIC(StockAction)
};

template <class SA>
QList<S> StockAction<SA>::run(const QStringList codes, const char* start, const char* end)
{
    QFuture<QList<S>> future = QtConcurrent::run(this, &StockAction<SA>::get_price, codes, start, end);

    QList<S> stocks = future.result();
    return stocks;
}

template <class SA>
QList<S> StockAction<SA>::run(const QStringList codes, int category)
{
    QFuture<QList<S>> future = QtConcurrent::run(this, &StockAction<SA>::get_price, codes, category);

    QList<S> stocks = future.result();
    return stocks;
}

template <class SA>
QList<S> StockAction<SA>::get_price(const QStringList codes, const char* start, const char* end)
{
    double start_d = QDateTime::fromString(QString::fromUtf8(start), Qt::ISODate).toSecsSinceEpoch();
    double end_d   = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch();
    // auto sa = static_cast<SA *>(this);
    auto sa = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(sa);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<S> stocks = S::get_price(codes, start_d, end_d);
    commitTransactions();
    return stocks;
}

template <class SA>
QList<S> StockAction<SA>::get_price(const QStringList codes, int category)
{
    // auto sa = static_cast<SA *>(this);
    auto sa = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(sa);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<S> stocks = S::get_price(codes, category);
    commitTransactions();
    return stocks;
}

} // namespace abq
