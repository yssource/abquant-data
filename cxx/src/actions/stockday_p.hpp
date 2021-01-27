/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/stockday.hpp"

namespace abq
{
class StockDayAction::impl
{
public:
    MyDataFramePtr getDataFrame(const StockDayAction& sa) const;
    void setDataFrame(const StockDayAction& sa);
    MyDataFramePtr toFq(const StockDayAction& sa, FQ_TYPE fq);
    series_no_cvr_t getOpen(const StockDayAction& sa) const;
    void getName(const StockDayAction& sa) const;
    QStringList getCodes(const StockDayAction& sa) const;
    inline QList<StockDay> getStocks(const StockDayAction&) const { return m_stockdays; };
    inline QVector<const char*> getColumns(const StockDayAction&) const { return m_columns; };

    impl(StockDayAction& sa, QStringList codes, const char* start, const char* end, FQ_TYPE xdxr) : m_xdxr{xdxr}
    {
        m_stockdays = sa.run<StockDay>(codes, start, end);
        if (m_stockdays.isEmpty()) {
            qDebug() << "No stock day data.\n"
                     << codes << "\n"
                     << "start: " << start << "\n"
                     << "end: " << end << "\n";
        }
        setDataFrame(sa);
        // m_df->template write<std::ostream, index_t, double, int>(std::cout);
    }

    series_no_cvr_t get_pyseries(const StockDayAction& sa, const char* col) const noexcept;

private:
    QList<StockDay> m_stockdays;
    const QVector<const char*> m_columns{"open", "close", "high", "low", "vol", "amount", "date", "code", "date_stamp"};
    QStringList m_codes;
    const char* m_start;
    const char* m_end;
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};
    MyDataFramePtr m_df{nullptr};
    // std::shared_ptr<std::string> m_name{nullptr};
};
} // namespace abq
