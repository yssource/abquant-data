#!/usr/bin/env python
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake, tools, Meson, RunEnvironment
import os
import shutil
import platform
import pathlib


class AbquantConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {
        "shared": False,
        "fPIC": True,
        "qt:qtdeclarative": True,
        "qt:qttools": True,
        "qt:qtwebsockets": True,
    }

    generators = ["cmake", "qmake"]

    @property
    def _prefix(self):
        if tools.os_info.is_linux:
            PREFIX = "/usr/local"
        else:
            PREFIX = "/usr/local"
        return PREFIX

    @property
    def _bindir(self):
        return "{}/bin".format(self._prefix)

    @property
    def _libdir(self):
        return "{}/lib".format(self._prefix)

    @property
    def _includedir(self):
        return "{}/include/abquant".format(self._prefix)

    @property
    def _datadir(self):
        return "{}/share/abquant".format(self._prefix)

    @property
    def _source_subfolder(self):
        return self.source_folder

    @property
    def _build_subfolder(self):
        return os.path.join(self.source_folder, "qbuild")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("qt/5.12.6@{}/{}".format("bincrafters", "stable"))
        self.requires("xtensor/0.21.3@")
        self.requires("dataframe/1.6.0@")
        self.requires("pybind11/2.2.4@{}/{}".format("conan", "stable"))

    def _build_with_qmake(self, test=None):
        tools.mkdir(self._build_subfolder)
        with tools.chdir(self._build_subfolder):
            pass
            # try:
            #     cmd = "rm -fv .qmake.stash"
            #     self.output.info(cmd)
            #     self.run(cmd, run_environment=True)

            #     # [ -f Makefile ] && make -k distclean >/dev/null 2>&1
            #     path = pathlib.Path("Makefile")
            #     if path.exists():
            #         cmd = "make -k distclean >/dev/null 2>&1"
            #         self.output.info(cmd)
            #         self.run(cmd, run_environment=True)
            # except Exception:
            #     pass

        self.output.info("Building with qmake")

        with tools.vcvars(
            self.settings
        ) if self.settings.compiler == "Visual Studio" else tools.no_op():
            args = [self.source_folder, "DESTDIR=lib"]

            def _getenvpath(var):
                val = os.getenv(var)
                if val and tools.os_info.is_windows:
                    val = val.replace("\\", "/")
                    os.environ[var] = val
                return val

            value = _getenvpath("CC")
            if value:
                args += [
                    "QMAKE_CC=" + value,
                    "QMAKE_LINK_C=" + value,
                    "QMAKE_LINK_C_SHLIB=" + value,
                ]

            value = _getenvpath("CXX")
            if value:
                args += [
                    "QMAKE_CXX=" + value,
                    "QMAKE_LINK=" + value,
                    "QMAKE_LINK_SHLIB=" + value,
                ]

            if test:
                args += ["-recursive", "{}/main.pro".format(self.test_m)]

            # args += ["target.path=\"{}\"".format(self._libdir), "header.path=\"{}\"".format(self._includedir)]

            cmd = "qmake %s" % " ".join(args)
            self.output.info(cmd)
            self.run(cmd, run_environment=True)
            if tools.os_info.is_windows:
                if self.settings.compiler == "Visual Studio":
                    self.run("jom", run_environment=True)
                else:
                    self.run("mingw32-make", run_environment=True)
            else:
                cmd = "bear make"
                self.output.info(cmd)
                self.run("bear make", run_environment=True)

    def build(self):
        self._build_with_qmake()
