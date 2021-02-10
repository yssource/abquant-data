/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/xdxr.hpp"
#include "abquant/actions/stockmin_p.hpp"

namespace abq
{
/*******************
 * StockMinAction *
 *******************/

StockMinAction::StockMinAction(QStringList codes, const char* start, const char* end, MIN_FREQ freq, FQ_TYPE xdxr)
    : pImpl{std::make_shared<impl>(*this, codes, start, end, freq, xdxr)}
{
}

//! Destructor
StockMinAction::~StockMinAction() noexcept = default;

//! Move assignment operator
StockMinAction& StockMinAction::operator=(StockMinAction&& other) noexcept
{
    if (&other == this) {
        return *this;
    }
    swap(pImpl, other.pImpl);

    return *this;
};

MyDataFramePtr StockMinAction::getDataFrame() const { return pImpl->getDataFrame(*this); }

MyDataFramePtr StockMinAction::toFq(FQ_TYPE fq) { return pImpl->toFq(*this, fq); }

QStringList StockMinAction::getCodes() const { return pImpl->getCodes(*this); }

QList<StockMin> StockMinAction::getStocks() const { return pImpl->getStocks(*this); };

QVector<const char*> StockMinAction::getColumns() const { return pImpl->getColumns(*this); }

/***********************
 * StockMinAction impl *
 **********************/

StockMinAction::impl::impl(StockMinAction& sa, QStringList codes, const char* start, const char* end, MIN_FREQ freq,
                           FQ_TYPE xdxr)
    : m_codes{codes}
{
    m_stockmins = sa.run<StockMin>(codes, start, end, freq);
    if (m_stockmins.isEmpty()) {
        qDebug() << "No stock min data.\n"
                 << codes << "\n"
                 << "start: " << start << "\n"
                 << "freq: " << freq << "\n"
                 << "end: " << end << "\n";
    }
    setDataFrame();
    if (xdxr != FQ_TYPE::NONE) {
        m_df = toFq(sa, xdxr);
    }
    // m_df->template write<std::ostream, index_t, double, int>(std::cout);
}

MyDataFramePtr StockMinAction::impl::getDataFrame(const StockMinAction&) const
{
    // m_df->template write<std::ostream, index_t, double, int>(std::cout);
    return m_df;
}

MyDataFramePtr StockMinAction::impl::toFq(const StockMinAction& sa, FQ_TYPE fq)
{
    auto x = Xdxr(sa);
    if (fq == FQ_TYPE::NONE || (m_df && !m_df->get_index().size())) {
        return m_df;
    }
    m_df = x.getXdxr(m_df, fq);
    return m_df;
}

void StockMinAction::impl::setDataFrame()
{
    MyDataFrame df;
    try {
        // open" : 49.0,
        // close" : 49.0,
        // high" : 49.0,
        // low" : 49.0,
        // vol" : 32768.5,
        // amount" : 5000.0,
        // datetime" : "2018-08-24 09:31:00",
        // date" : "1991-04-03",
        // code" : "000001",
        // date_stamp" : 670608000.0
        // time_stamp" : 670608000.0
        // type" : "1min",

        std::vector<index_t> datetimeCodeIdx;
        series_no_cvp_t open;
        series_no_cvp_t close;
        series_no_cvp_t high;
        series_no_cvp_t low;
        series_no_cvp_t vol;
        series_no_cvp_t amount;
        std::vector<std::string> datetime;
        std::vector<std::string> date;
        std::vector<std::string> code;
        series_no_cvp_t date_stamp;
        series_no_cvp_t time_stamp;
        std::vector<std::string> type;
        series_no_cvp_t if_trade;

        foreach (auto s, m_stockmins) {
            datetimeCodeIdx.push_back((s.datetime() + QString("_") + s.code()).toStdString());
            open.push_back(s.open());
            close.push_back(s.close());
            high.push_back(s.high());
            low.push_back(s.low());
            vol.push_back(s.vol());
            amount.push_back(s.amount());
            datetime.push_back(s.datetime().toStdString());
            date.push_back(s.date().toStdString());
            code.push_back(s.code().toStdString());
            date_stamp.push_back(s.dateStamp());
            time_stamp.push_back(s.timeStamp());
            type.push_back(s.type().toStdString());
            if_trade.push_back(1);
        }

        df.load_data(std::move(datetimeCodeIdx), std::make_pair("open", open), std::make_pair("close", close),
                     std::make_pair("high", high), std::make_pair("low", low), std::make_pair("vol", vol),
                     std::make_pair("amount", amount), std::make_pair("date", date),
                     std::make_pair("datetime", datetime), std::make_pair("code", code),
                     std::make_pair("date_stamp", date_stamp), std::make_pair("time_stamp", time_stamp),
                     std::make_pair("type", type), std::make_pair("if_trade", if_trade));
        // df.write<std::ostream, std::string, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
    m_df = std::make_shared<MyDataFrame>(df);
};

} // namespace abq
