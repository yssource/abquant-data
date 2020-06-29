/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the Apache License 2.0 License.           *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/stockday.hpp"

namespace abq
{
StockDayAction::StockDayAction(QStringList codes, const char* start, const char* end, FQ_TYPE xdxr)
    : StockAction(codes), m_codes{codes}, m_start{start}, m_end{end}, m_xdxr{xdxr}
{
    m_stockdays = run<StockDay>(codes, start, end);
    if (m_stockdays.isEmpty()) {
        std::cout << "No stock day data.\n";
    }
}

const MyDataFrame StockDayAction::toFq(FQ_TYPE fq) const
{
    auto x  = Xdxr<StockDayAction>(*this);
    auto df = toDataFrame();
    if (fq == FQ_TYPE::NONE || !df.get_index().size()) {
        return df;
    }
    return x.getXdxr(df, fq);
}

MyDataFrame StockDayAction::toDataFrame() const
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

        std::vector<std::string> datetimeCodeIdx;
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

        int rc =
            df.load_data(std::move(datetimeCodeIdx), std::make_pair("open", open), std::make_pair("close", close),
                         std::make_pair("high", high), std::make_pair("low", low), std::make_pair("vol", vol),
                         std::make_pair("amount", amount), std::make_pair("date", date), std::make_pair("code", code),
                         std::make_pair("date_stamp", date_stamp), std::make_pair("if_trade", if_trade));

        // df.write<std::ostream, std::string, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
    return df;
};

std::shared_ptr<MyDataFrame> StockDayAction::getDataFrame() const { return m_df; }

vector<double> StockDayAction::getOpen() const
{
    vector<double> open;
    if (m_df != nullptr) {
        try {
            // m_df->write<std::ostream, std::string, double, int>(std::cout);
            open = m_df->template get_column<double>("open");
        } catch (const std::exception& e) {
            std::cout << e.what();
        }
    }
    return open;
}

void StockDayAction::setDataFrame()
{
    MyDataFrame df = toFq(m_xdxr);
    m_df           = std::make_shared<MyDataFrame>(df);
    // m_df->write<std::ostream, std::string, double, int>(std::cout);
}

vector<double> StockDayAction::get_pyseries(const char* col) const noexcept
{
    vector<double> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    if (m_xdxr == FQ_TYPE::PRE || m_xdxr == FQ_TYPE::POST) {
        std::shared_ptr<MyDataFrame> df;

        try {
            df = getDataFrame();
            // df->write<std::ostream, std::string, double, int>(std::cout);
            const char* colname;
            if (QString(col) == QString("code")) {
                colname = "lhs.code";
            } else if (QString(col) == QString("date")) {
                colname = "lhs.date";
            } else {
                colname = col;
            }
            series = df->get_column<double>(colname);
        } catch (...) {
            std::cout << " error ... "
                      << "\n";
        }
    }
    return series;
}

} // namespace abq
