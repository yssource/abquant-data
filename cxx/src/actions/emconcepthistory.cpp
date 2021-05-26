/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/emconcepthistory.hpp"

namespace abq
{
EmConceptHistoryAction::EmConceptHistoryAction(QStringList codes, const char* start, const char* end)
    : EmConceptAction(codes), m_codes{codes}, m_start{start}, m_end{end}
{
    m_emconcepthistorys = run<EmConceptHistory>(codes, start, end);
    if (m_emconcepthistorys.isEmpty()) {
        qDebug() << "No index day data.\n"
                 << codes << "\n"
                 << "start: " << start << "\n"
                 << "end: " << end << "\n";
    }
}

MyDataFrame EmConceptHistoryAction::toDataFrame() const
{
    MyDataFrame df;
    try {
        // date: "2016-10-24",
        // open: 1000.60,
        // close: 1009.33,
        // high: 1012.49,
        // low: 994.84,
        // volume: 4672192,
        // amount: 6728219904.00,
        // amplitude: 1.76,
        // rise_fall_pct: 0.93,
        // rise_fall_amt: 9.33,
        // turnover: 1.08,
        // mkt_code: "90.BK0990",
        // date_stamp: 1477267200000

        std::vector<std::string> datetimeCodeIdx;
        std::vector<std::string> date;
        std::vector<double> open;
        std::vector<double> close;
        std::vector<double> high;
        std::vector<double> low;
        std::vector<double> volume;
        std::vector<double> amount;
        std::vector<double> amplitude;
        std::vector<double> rise_fall_pct;
        std::vector<double> rise_fall_amt;
        std::vector<double> turnover;
        std::vector<std::string> mkt_code;
        std::vector<double> date_stamp;
        std::vector<double> if_trade;

        foreach (auto s, m_emconcepthistorys) {
            datetimeCodeIdx.push_back((s.date() + QString(" 00:00:00_") + s.mktCode()).toStdString());
            open.push_back(s.open());
            close.push_back(s.close());
            high.push_back(s.high());
            low.push_back(s.low());
            volume.push_back(s.volume());
            amount.push_back(s.amount());
            amplitude.push_back(s.amplitude());
            rise_fall_pct.push_back(s.riseFallPct());
            rise_fall_amt.push_back(s.riseFallAmt());
            turnover.push_back(s.turnover());
            date.push_back(s.date().toStdString());
            mkt_code.push_back(s.mktCode().toStdString());
            date_stamp.push_back(s.dateStamp());
            if_trade.push_back(1);
        }

        int rc = df.load_data(std::move(datetimeCodeIdx), std::make_pair("date", date), std::make_pair("open", open),
                              std::make_pair("close", close), std::make_pair("high", high), std::make_pair("low", low),
                              std::make_pair("volume", volume), std::make_pair("amount", amount),
                              std::make_pair("amplitude", amplitude), std::make_pair("rise_fall_pct", rise_fall_pct),
                              std::make_pair("rise_fall_amt", rise_fall_amt), std::make_pair("turnover", turnover),
                              std::make_pair("mkt_code", mkt_code), std::make_pair("date_stamp", date_stamp),
                              std::make_pair("if_trade", if_trade));

        // df.write<std::ostream, std::string, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
    return df;
};

std::shared_ptr<MyDataFrame> EmConceptHistoryAction::get_dataframe() const { return m_df; }

vector<double> EmConceptHistoryAction::getOpen() const
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

void EmConceptHistoryAction::set_dataframe()
{
    MyDataFrame df = toDataFrame();
    m_df           = std::make_shared<MyDataFrame>(df);
    // m_df->write<std::ostream, std::string, double, int>(std::cout);
}

vector<double> EmConceptHistoryAction::get_pyseries(const char* col) const noexcept
{
    vector<double> series;
    auto cols = get_columns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    std::shared_ptr<MyDataFrame> df;

    try {
        df = get_dataframe();
        // df->write<std::ostream, std::string, double, int>(std::cout);
        series = df->get_column<double>(col);
    } catch (...) {
        std::cout << " error ... "
                  << "\n";
    }
    return series;
}

} // namespace abq
