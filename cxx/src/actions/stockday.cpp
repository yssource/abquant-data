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
/*******************
 * StockDayAction *
 *******************/

StockDayAction::StockDayAction(QStringList codes, const char* start, const char* end, FQ_TYPE xdxr)
    : pImpl{std::make_shared<impl>(*this, codes, start, end, xdxr)}
{
}

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

MyDataFramePtr StockDayAction::getDataFrame() const { return pImpl->getDataFrame(*this); }

MyDataFramePtr StockDayAction::toFq(FQ_TYPE fq) { return pImpl->toFq(*this, fq); }

QStringList StockDayAction::getCodes() const { return pImpl->getCodes(*this); }

QList<StockDay> StockDayAction::getStocks() const { return pImpl->getStocks(*this); };

QVector<const char*> StockDayAction::getColumns() const { return pImpl->getColumns(*this); }


/***********************
 * StockDayAction impl *
 **********************/

StockDayAction::impl::impl(StockDayAction& sa, QStringList codes, const char* start, const char* end, FQ_TYPE xdxr)
    : m_codes{codes}
{
    m_stockdays = sa.run<StockDay>(codes, start, end);
    if (m_stockdays.isEmpty()) {
        qDebug() << "No stock day data.\n"
                 << codes << "\n"
                 << "start: " << start << "\n"
                 << "end: " << end << "\n";
    }
    setDataFrame();
    if (xdxr != FQ_TYPE::NONE) {
        using action_t = typename std::decay<typename std::remove_pointer<decltype(sa)>::type>::type;
        // m_df = sa.toFq(xdxr);
        m_df = toFq(sa, xdxr);
    }
    // m_df->template write<std::ostream, index_t, double, int>(std::cout);
}

MyDataFramePtr StockDayAction::impl::getDataFrame(const StockDayAction&) const
{
    // m_df->template write<std::ostream, index_t, double, int>(std::cout);
    return m_df;
}

MyDataFramePtr StockDayAction::impl::toFq(const StockDayAction& sa, FQ_TYPE fq)
{
    auto x = Xdxr<StockDayAction>(sa);
    if (fq == FQ_TYPE::NONE || (m_df && !m_df->get_index().size())) {
        return m_df;
    }
    m_df = x.getXdxr(m_df, fq);
    return m_df;
}

void StockDayAction::impl::setDataFrame()
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
        series_no_cvp_t open;
        series_no_cvp_t close;
        series_no_cvp_t high;
        series_no_cvp_t low;
        series_no_cvp_t vol;
        series_no_cvp_t amount;
        std::vector<std::string> date;
        std::vector<std::string> code;
        series_no_cvp_t date_stamp;
        series_no_cvp_t if_trade;

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

    m_df = std::make_shared<MyDataFrame>(df);
};

} // namespace abq
