/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the Apache License 2.0 License.           *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "stockmin.hpp"

namespace abq
{
StockMinAction::StockMinAction(QStringList codes, const char* start, const char* end, MIN_FREQ freq)
    : StockAction(codes), m_codes{codes}, m_start{start}, m_end{end}, m_freq{freq}
{
    m_stockmins = run<StockMin>(codes, start, end, freq);
}

MyDataFrame StockMinAction::toFq(FQ_TYPE fq) const
{
    MyDataFrame qs;
    if (fq == FQ_TYPE::NONE) {
        return qs;
    }
    auto x  = Xdxr<StockMinAction>(*this);
    auto df = toDataFrame();
    qs      = x.getXdxr(df, fq);
    return qs;
}

MyDataFrame StockMinAction::toDataFrame() const
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

        std::vector<std::string> datetimeCodeIdx;
        std::vector<double> open;
        std::vector<double> close;
        std::vector<double> high;
        std::vector<double> low;
        std::vector<double> vol;
        std::vector<double> amount;
        std::vector<std::string> datetime;
        std::vector<std::string> date;
        std::vector<std::string> code;
        std::vector<double> date_stamp;
        std::vector<double> time_stamp;
        std::vector<std::string> type;
        std::vector<double> if_trade;

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

        int rc = df.load_data(std::move(datetimeCodeIdx), std::make_pair("open", open), std::make_pair("close", close),
                              std::make_pair("high", high), std::make_pair("low", low), std::make_pair("vol", vol),
                              std::make_pair("amount", amount), std::make_pair("date", date),
                              std::make_pair("datetime", datetime), std::make_pair("code", code),
                              std::make_pair("date_stamp", date_stamp), std::make_pair("time_stamp", time_stamp),
                              std::make_pair("type", type), std::make_pair("if_trade", if_trade));
        // df.write<std::ostream, unsigned long, std::string, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
    return df;
};

} // namespace abq
