/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/stockday_p.hpp"

namespace abq
{
StockDayAction::StockDayAction(QStringList codes, const char* start, const char* end, FQ_TYPE xdxr)
    : pImpl{std::make_shared<impl>(*this, codes, start, end, xdxr)}, m_xdxr{xdxr}
{
}

MyDataFramePtr StockDayAction::impl::getDataFrame(const StockDayAction& sa) const
{
    // m_df->template write<std::ostream, index_t, double, int>(std::cout);
    return m_df;
}

MyDataFramePtr StockDayAction::getDataFrame() const { return pImpl->getDataFrame(*this); }

MyDataFramePtr StockDayAction::impl::toFq(const StockDayAction& sa, FQ_TYPE fq)
{
    // qDebug() << fq << "\n";
    auto x = Xdxr<StockDayAction>(sa);
    if (fq == FQ_TYPE::NONE || (m_df && !m_df->get_index().size())) {
        return m_df;
    }
    m_df = x.getXdxr(m_df, fq);
    // m_df->template write<std::ostream, index_t, double, int>(std::cout);
    return m_df;
}

MyDataFramePtr StockDayAction::toFq(FQ_TYPE fq) { return pImpl->toFq(*this, fq); }

series_no_cvr_t StockDayAction::impl::getOpen(const StockDayAction& sa) const
{
    series_no_cvr_t open;
    if (m_df != nullptr) {
        try {
            // df->write<std::ostream, std::string, double, int>(std::cout);
            open = m_df->template get_column<double>("open");
        } catch (const std::exception& e) {
            std::cout << e.what();
        }
    }
    return open;
}

series_no_cvr_t StockDayAction::getOpen() const { return pImpl->getOpen(*this); }

// void StockDayAction::impl::getName(const StockDayAction& sa) const
// {
//     if (m_name == nullptr) {
//     }
//     std::cout << *(m_name.get()) << "\n";
//     // return m_name;
// }

// void StockDayAction::getName() const { return pImpl->getName(*this); }

QStringList StockDayAction::getCodes() const { return pImpl->getCodes(*this); }

QStringList StockDayAction::impl::getCodes(const StockDayAction& sa) const { return m_codes; }

inline QList<StockDay> StockDayAction::getStocks() const { return pImpl->getStocks(*this); };

QVector<const char*> StockDayAction::getColumns() const { return pImpl->getColumns(*this); }

//! Destructor
StockDayAction::~StockDayAction() noexcept = default;

//! Move assignment operator

StockDayAction& StockDayAction::operator=(StockDayAction&& other) noexcept
{
    if (&other == this) {
        return *this;
    }
    swap(pImpl, other.pImpl);
    return *this;
};

void StockDayAction::impl::setDataFrame(const StockDayAction& sa)
{
    MyDataFrame df;
    try {
        // open" : 49.0,
        // close" : 49.0,
        // high" : 49.0,
        // low" : 49.0,
        // vol" : 32768.5,
        // amount" : 5000.0,
        // date" : "1991-04-03",
        // code" : "000001",
        // date_stamp" : 670608000.0

        std::vector<index_t> datetimeCodeIdx;
        std::vector<double> open;
        std::vector<double> close;
        std::vector<double> high;
        std::vector<double> low;
        std::vector<double> vol;
        std::vector<double> amount;
        std::vector<std::string> date;
        std::vector<std::string> code;
        std::vector<double> date_stamp;
        std::vector<double> if_trade;

        foreach (auto s, m_stockdays) {
            datetimeCodeIdx.push_back((s.date() + QString(" 00:00:00_") + s.code()).toStdString());
            open.push_back(s.open());
            close.push_back(s.close());
            high.push_back(s.high());
            low.push_back(s.low());
            vol.push_back(s.vol());
            amount.push_back(s.amount());
            date.push_back(s.date().toStdString());
            code.push_back(s.code().toStdString());
            date_stamp.push_back(s.dateStamp());
            if_trade.push_back(1);
        }

        df.load_data(std::move(datetimeCodeIdx), std::make_pair("open", open), std::make_pair("close", close),
                     std::make_pair("high", high), std::make_pair("low", low), std::make_pair("vol", vol),
                     std::make_pair("amount", amount), std::make_pair("date", date), std::make_pair("code", code),
                     std::make_pair("date_stamp", date_stamp), std::make_pair("if_trade", if_trade));

        // df.template write<std::ostream, index_t, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }

    // m_df = std::make_shared<MyDataFrame>(std::move(df));
    m_df = std::make_shared<MyDataFrame>(df);
};

series_no_cvr_t StockDayAction::get_pyseries(const char* col) const noexcept { return pImpl->get_pyseries(*this, col); }

series_no_cvr_t StockDayAction::impl::get_pyseries(const StockDayAction& sa, const char* col) const noexcept
{
    vector<double> series;
    auto cols = getColumns(sa);
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    if (m_xdxr == FQ_TYPE::PRE || m_xdxr == FQ_TYPE::POST) {
        try {
            // m_df->write<std::ostream, index_t, double, int>(std::cout);
            const char* colname;
            if (QString(col) == QString("code")) {
                colname = "lhs.code";
            } else if (QString(col) == QString("date")) {
                colname = "lhs.date";
            } else {
                colname = col;
            }
            series = m_df->get_column<double>(colname);
        } catch (...) {
            std::cout << " error ... "
                      << "\n";
        }
    }
    return series;
}

} // namespace abq
