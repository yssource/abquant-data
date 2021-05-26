/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/indexday_p.hpp"
#include "abquant/actions/xdxr.hpp"

namespace abq
{
/*******************
 * IndexDayAction *
 *******************/

IndexDayAction::IndexDayAction(QStringList codes, const char* start, const char* end, FQ_TYPE xdxr)
    : pImpl{std::make_shared<impl>(*this, codes, start, end, xdxr)}
{
}

//! Destructor
IndexDayAction::~IndexDayAction() noexcept = default;

//! Move assignment operator
IndexDayAction& IndexDayAction::operator=(IndexDayAction&& other) noexcept
{
    if (&other == this) {
        return *this;
    }
    swap(pImpl, other.pImpl);

    return *this;
};

MyDataFramePtr IndexDayAction::get_dataframe() const { return pImpl->get_dataframe(*this); }

QStringList IndexDayAction::get_codes() const { return pImpl->get_codes(*this); }

QList<IndexDay> IndexDayAction::get_securities() const { return pImpl->get_securities(*this); };

QVector<const char*> IndexDayAction::get_columns() const { return pImpl->get_columns(*this); }

/***********************
 * IndexDayAction impl *
 **********************/

IndexDayAction::impl::impl(IndexDayAction& sa, QStringList codes, const char* start, const char* end, FQ_TYPE xdxr)
    : m_codes{codes}
{
    m_indexdays = sa.run<IndexDay>(codes, start, end);
    if (m_indexdays.isEmpty()) {
        qDebug() << "No index day data.\n"
                 << codes << "\n"
                 << "start: " << start << "\n"
                 << "end: " << end << "\n";
    }
    set_dataframe();
    // m_df->template write<std::ostream, index_type, double, int>(std::cout);
}

MyDataFramePtr IndexDayAction::impl::get_dataframe(const IndexDayAction&) const { return m_df; }

void IndexDayAction::impl::set_dataframe()
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

        std::vector<index_type> datetimeCodeIdx;
        series_no_cvp_type open;
        series_no_cvp_type close;
        series_no_cvp_type high;
        series_no_cvp_type low;
        series_no_cvp_type vol;
        series_no_cvp_type amount;
        std::vector<std::string> date;
        std::vector<std::string> code;
        series_no_cvp_type date_stamp;
        series_no_cvp_type if_trade;

        foreach (auto s, m_indexdays) {
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

        // df.write<std::ostream, std::string, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
    m_df = std::make_shared<MyDataFrame>(df);
};

} // namespace abq
