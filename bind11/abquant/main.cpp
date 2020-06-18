#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <QtCore/QObject>
#include <iostream>

#include "abquant/actions/abquant.hpp"

// using namespace std;
namespace abq
{
class PyAbquant
{
public:
    static void start()
    {
        cout << "pyabquant start 2 ... "
             << "\n";
        Abquant::start();
    }
    static void finish()
    {
        cout << "pyabquant finish ... "
             << "\n";
        Abquant::finish();
    }

    PyAbquant()
    {
        std::cout << "pyabquant start ....000000.. "
                  << "\n";
    };
    ~PyAbquant()
    {
        std::cout << "pyabquant dddd..... ....000000.. "
                  << "\n";
    }

private:
    // = default;
    PyAbquant(const PyAbquant&) = delete;
    PyAbquant& operator=(const PyAbquant&) = delete;
};

namespace py = pybind11;

PYBIND11_MODULE(pyabquant, m)
{
    m.doc() = R"pbdoc(
        Pybind11 PyAbquant plugin
        -----------------------

        .. pyabquant:: currentmodule_exmaple

        .. autosummary::
           :pyabquant: PyAbquant

           pyabquant
    )pbdoc";

    py::class_<PyAbquant> sm_class(m, "PyAbquant");
    sm_class
        .def_static("start", &PyAbquant::start, R"pbdoc(
        start

        start function.
    )pbdoc")
        .def_static("finish", &PyAbquant::finish, R"pbdoc(
        finish void

        finish static void function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
