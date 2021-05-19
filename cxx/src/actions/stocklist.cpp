/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/stocklist_p.hpp"

namespace abq
{
/*******************
 * StockListAction *
 *******************/

StockListAction::StockListAction() : pImpl{std::make_shared<impl>(*this)} {}

// StockListAction::StockListAction(const char* end) : pImpl{std::make_shared<impl>(*this, end)} {
//     std::cout << "StockListAction::StockListAction" << "\n";
//     std::cout << end << "\n";
// }

StockListAction::StockListAction(const QStringList codes, const char* end)
    : pImpl{std::make_shared<impl>(*this, codes, end)}
{
}

//! Destructor
StockListAction::~StockListAction() noexcept = default;

//! Move assignment operator
StockListAction& StockListAction::operator=(StockListAction&& other) noexcept
{
    if (&other == this) {
        return *this;
    }
    swap(pImpl, other.pImpl);

    return *this;
};

MyDataFramePtr StockListAction::getDataFrame() const { return pImpl->getDataFrame(*this); }

QStringList StockListAction::getCodes() const { return pImpl->getCodes(*this); }

QList<StockList> StockListAction::getStocks() const { return pImpl->getStocks(*this); };

QVector<const char*> StockListAction::getColumns() const { return pImpl->getColumns(*this); }

/***********************
 * StockListAction impl *
 **********************/

StockListAction::impl::impl(StockListAction& sa)
{
    m_stocklists = sa.run<StockList>("");
    if (m_stocklists.isEmpty()) {
        qDebug() << "No stock list data.\n";
    }
    setDataFrame();
    // m_df->template write<std::ostream, index_type, double, int>(std::cout);
}

// StockListAction::impl::impl(StockListAction& sa, const char* end)
// {
//     m_stocklists = sa.run<StockList>(end);
//     if (m_stocklists.isEmpty()) {
//         qDebug() << "No stock list data.\n"
//                  << "end: " << end << "\n";
//     }
//     setDataFrame();
//     // m_df->template write<std::ostream, index_type, double, int>(std::cout);
// }

StockListAction::impl::impl(StockListAction& sa, const QStringList codes, const char* end)
{
    m_stocklists = sa.run<StockList>(codes, end);
    if (m_stocklists.isEmpty()) {
        qDebug() << "No stock list data.\n"
                 << codes << "\n"
                 << "end: " << end << "\n";
    }
    setDataFrame();
    // m_df->template write<std::ostream, index_type, double, int>(std::cout);
}

MyDataFramePtr StockListAction::impl::getDataFrame(const StockListAction&) const
{
    // m_df->template write<std::ostream, index_type, double, int>(std::cout);
    return m_df;
}

void StockListAction::impl::setDataFrame()
{
    MyDataFrame df;
    try {
        std::vector<index_type> datetimeCodeIdx;
        std::vector<std::string> code;
        series_no_cvp_type volunit;
        series_no_cvp_type decimal_point;
        std::vector<std::string> name;
        series_no_cvp_type pre_close;
        std::vector<std::string> sse;
        std::vector<std::string> sec;

        foreach (auto s, m_stocklists) {
            datetimeCodeIdx.push_back(s.code().toStdString());
            // code.push_back(s.code().toStdString());
            volunit.push_back(s.volunit());
            decimal_point.push_back(s.decimalPoint());
            name.push_back(s.name().toStdString());
            pre_close.push_back(s.preClose());
            sse.push_back(s.sse().toStdString());
            sec.push_back(s.sec().toStdString());
        }

        df.load_data(std::move(datetimeCodeIdx),
                     /* std::make_pair("code", code),  */ std::make_pair("volunit", volunit),
                     std::make_pair("decimal_point", decimal_point), std::make_pair("name", name),
                     std::make_pair("pre_close", pre_close), std::make_pair("sse", sse), std::make_pair("sec", sec));

        // df.template write<std::ostream, index_type, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }

    m_df = std::make_shared<MyDataFrame>(df);
};

} // namespace abq
