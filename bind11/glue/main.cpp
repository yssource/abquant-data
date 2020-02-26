#include "blog.h" //  include the model class
// #include "foocontroller.h"
// #include "models/blog.h" //  include the model class
#include <pybind11/pybind11.h>

#include <QtCore/QObject>
#include <iostream>
// #include <TfTest/TfTest>

using namespace std;
namespace abq
{
// class TestBlog : public QObject {
//   Q_OBJECT
// private slots:
//   // void create_data();
//   QString create();
// };

// void TestBlog::create_data() {
//   // definition of test data
//   QTest::addColumn<QString>("title");
//   QTest::addColumn<QString>("body");

//   // adding to test data
//   QTest::newRow("No1") << "Hello"
//                        << "Hello world.";
// }

// // void TestBlog::create() {
// QString TestBlog::create() {
//     // acquisition of test data
//     // QFETCH(QString, title);
//     // QFETCH(QString, body);

//     // logic of the test
//     Blog created = Blog::create(title, body);
//     int id = created.id();
//     Blog blog = Blog::get(id); // Getting model ID

//     // verification of result execution
//     // QCOMPARE(blog.title(), title);
//     // QCOMPARE(blog.body(), body);
//     return blog.title();
//   }

// //   TF_TEST_MAIN(TestBlog) // specify the class name you created
// #include "main.moc" // charm. Make the extension .moc

int add(int i, int j) { return i + j; }

string foo2()
{
    // TestBlog tb;
    // tb.create_data();
    // string s = tb.create().toUtf8().constData();
    // return s;

    QString s("600001");

    QString title = "Hello";
    QString body  = "Hello world.";

    TF_TEST_MAIN(TestBlog) // specify the class name you created
    Blog created = Blog::create(title, body);
    int id       = created.id();
    Blog blog    = Blog::get(id); // Getting model ID

    // FooController f;
    // QObject::connect(&f, SIGNAL(valueChanged(int)), &b, SLOT(index(QString)));
    // const QString &ss = f.index(s);
    // QString ss = f.index(s);
    // return ss.toInt();
    return blog.title().toUtf8().constData();
    // return s.toUtf8().constData();
}

namespace py = pybind11;

PYBIND11_MODULE(foo, m)
{
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("foo2", &foo2, R"pbdoc(
        foo fffffff Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def(
        "subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
