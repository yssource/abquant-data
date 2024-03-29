/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/stockxdxr_p.hpp"

namespace abq
{
/*******************
 * StockXdxrAction *
 *******************/

StockXdxrAction::StockXdxrAction(QStringList codes, int category)
    : pImpl{std::make_shared<impl>(*this, codes, category)}
{
}

//! Destructor
StockXdxrAction::~StockXdxrAction() noexcept = default;

//! Move assignment operator
StockXdxrAction& StockXdxrAction::operator=(StockXdxrAction&& other) noexcept
{
    if (&other == this) {
        return *this;
    }
    swap(pImpl, other.pImpl);

    return *this;
};

MyDataFramePtr StockXdxrAction::get_dataframe() const { return pImpl->get_dataframe(*this); }

QList<StockXdxr> StockXdxrAction::get_securities() const { return pImpl->get_securities(*this); };

QVector<const char*> StockXdxrAction::get_columns() const { return pImpl->get_columns(*this); }

/***********************
 * StockXdxrAction impl *
 **********************/

StockXdxrAction::impl::impl(StockXdxrAction& sa, QStringList codes, int category)
{
    m_stockxdxrs = sa.run<StockXdxr>(codes, category);
    if (m_stockxdxrs.isEmpty()) {
        qDebug() << "No stock xdxr data.\n"
                 << codes << "\n"
                 << "category: " << category << "\n";
    }
    set_dataframe();
    // m_df->template write<std::ostream, index_type, double, int>(std::cout);
}

MyDataFramePtr StockXdxrAction::impl::get_dataframe(const StockXdxrAction&) const
{
    // m_df->template write<std::ostream, index_type, double, int>(std::cout);
    return m_df;
}

void StockXdxrAction::impl::set_dataframe()
{
    MyDataFrame df;
    try {
        // since, std::numeric_limits<int>::quiet_NaN() is 0, which is not what
        // we wanted. So, here we change category type to double for DataFrame

        // int category;
        // QString name;
        // double fenhong;
        // double peigujia;
        // double songzhuangu;
        // double peigu;
        // double suogu;
        // double liquidity_before;
        // double liquidity_after;
        // double shares_before;
        // double shares_after;
        // double fenshu;
        // double xingquanjia;
        // QString date;
        // QString category_meaning;
        // QString code;

        std::vector<index_type> datetimeCodeIdx;
        series_no_cvp_type category;
        std::vector<std::string> name;
        series_no_cvp_type fenhong;
        series_no_cvp_type peigujia;
        series_no_cvp_type songzhuangu;
        series_no_cvp_type peigu;
        series_no_cvp_type suogu;
        series_no_cvp_type liquidity_before;
        series_no_cvp_type liquidity_after;
        series_no_cvp_type shares_before;
        series_no_cvp_type shares_after;
        series_no_cvp_type fenshu;
        series_no_cvp_type xingquanjia;
        std::vector<std::string> date;
        std::vector<std::string> category_meaning;
        std::vector<std::string> code;

        foreach (auto s, m_stockxdxrs) {
            datetimeCodeIdx.push_back((s.date() + QString(" 00:00:00_") + s.code()).toStdString());
            category.push_back(s.category());
            name.push_back(s.name().toStdString());
            fenhong.push_back(s.fenhong());
            peigujia.push_back(s.peigujia());
            songzhuangu.push_back(s.songzhuangu());
            peigu.push_back(s.peigu());
            suogu.push_back(s.suogu());
            liquidity_before.push_back(s.liquidityBefore());
            liquidity_after.push_back(s.liquidityAfter());
            shares_before.push_back(s.sharesBefore());
            shares_after.push_back(s.sharesAfter());
            fenshu.push_back(s.fenshu());
            xingquanjia.push_back(s.xingquanjia());
            date.push_back(s.date().toStdString());
            category_meaning.push_back(s.categoryMeaning().toStdString());
            code.push_back(s.code().toStdString());
        }

        df.load_data(std::move(datetimeCodeIdx), std::make_pair("category", category), std::make_pair("name", name),
                     std::make_pair("fenhong", fenhong), std::make_pair("peigujia", peigujia),
                     std::make_pair("songzhuangu", songzhuangu), std::make_pair("peigu", peigu),
                     std::make_pair("suogu", suogu), std::make_pair("liquidity_before", liquidity_before),
                     std::make_pair("liquidity_after", liquidity_after), std::make_pair("shares_before", shares_before),
                     std::make_pair("shares_after", shares_after), std::make_pair("fenshu", fenshu),
                     std::make_pair("xingquanjia", xingquanjia), std::make_pair("date", date),
                     std::make_pair("category_meaning", category_meaning), std::make_pair("code", code));
        // df.write<std::ostream, unsigned long, std::string, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }

    m_df = std::make_shared<MyDataFrame>(df);
};

} // namespace abq
