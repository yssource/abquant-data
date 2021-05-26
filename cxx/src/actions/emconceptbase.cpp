/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/emconceptbase.hpp"

namespace abq
{
EmConceptBaseAction::EmConceptBaseAction(QStringList codes) : EmConceptAction(codes), m_codes{codes}
{
    m_emconceptbases = run<EmConceptBase>(codes);
    if (m_emconceptbases.isEmpty()) {
        qDebug() << "No em_concept day data.\n" << codes << "\n";
    }
}

MyDataFrame EmConceptBaseAction::toDataFrame() const
{
    MyDataFrame df;
    try {
        // f104_bk_rise_cnt" : 21,
        // f105_bk_fall_cnt" : 3,
        // f12_code" : "BK0990",
        // f13_market" : 90,
        // f14_name" : "快递概念",
        // f3_bk_rise_fall_pct" : 1.32,
        // f8_turnover" : 1.29

        std::vector<std::string> f12_code_idx;
        std::vector<int> f104_bk_rise_cnt;
        std::vector<int> f105_bk_fall_cnt;
        std::vector<std::string> f12_code;
        std::vector<int> f13_market;
        std::vector<std::string> f14_name;
        std::vector<double> f3_bk_rise_fall_pct;
        std::vector<double> f8_turnover;

        foreach (auto s, m_emconceptbases) {
            f12_code_idx.push_back(s.f12Code().toStdString());
            f104_bk_rise_cnt.push_back(s.f104BkRiseCnt());
            f105_bk_fall_cnt.push_back(s.f105BkFallCnt());
            f12_code.push_back(s.f12Code().toStdString());
            f13_market.push_back(s.f13Market());
            f14_name.push_back(s.f14Name().toStdString());
            f3_bk_rise_fall_pct.push_back(s.f3BkRiseFallPct());
            f8_turnover.push_back(s.f8Turnover());
        }

        int rc = df.load_data(
            std::move(f12_code_idx), std::make_pair("f104_bk_rise_cnt", f104_bk_rise_cnt),
            std::make_pair("f105_bk_fall_cnt", f105_bk_fall_cnt), std::make_pair("f12_code", f12_code),
            std::make_pair("f13_market", f13_market), std::make_pair("f14_name", f14_name),
            std::make_pair("f3_bk_rise_fall_pct", f3_bk_rise_fall_pct), std::make_pair("f8_turnover", f8_turnover));

        // df.write<std::ostream, std::string, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
    return df;
}

std::shared_ptr<MyDataFrame> EmConceptBaseAction::get_dataframe() const { return m_df; }

vector<std::string> EmConceptBaseAction::get_f14_name() const
{
    vector<std::string> f14_name_v;
    if (m_df != nullptr) {
        try {
            // m_df->write<std::ostream, std::string, double, int>(std::cout)
            f14_name_v = m_df->template get_column<std::string>("f14_name");
        } catch (const std::exception& e) {
            std::cout << e.what();
        }
    }
    return f14_name_v;
}

vector<int> EmConceptBaseAction::get_f104_bk_rise_cnt() const
{
    vector<int> f104_bk_rise_cnt_v;

    if (m_df != nullptr) {
        try {
            // m_df->write<std::ostream, std::string, double, int>(std::cout)
            f104_bk_rise_cnt_v = m_df->template get_column<int>("f104_bk_rise_cnt");
        } catch (const std::exception& e) {
            std::cout << e.what();
        }
    }
    return f104_bk_rise_cnt_v;
}

void EmConceptBaseAction::set_dataframe()
{
    MyDataFrame df = toDataFrame();
    m_df           = std::make_shared<MyDataFrame>(df);
    // m_df->write<std::ostream, std::string, double, int>(std::cout);
}

vector<double> EmConceptBaseAction::get_pyseries(const char* col) const noexcept
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
