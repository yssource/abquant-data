# include(./include/abquant/headers.pri)

# isEmpty(header.path) {
# windows {
# header.path = C:/abquant/$${VERSION}/include
# } else:macx {
# header.path = /usr/include/abquant
# } else:unix {
# header.path = /usr/local/include/abquant
# }
# }


# INSTALL_HEADERS=/usr/local/include/abquant
# INSTALL_HEADERS=/tmp

include(./include/abquant/headers.pri)

isEmpty(header.path) {
windows {
header.path = C:/abquant/$${VERSION}/include
} else:macx {
header.path = /usr/include/abquant
} else:unix {
header.path = /usr/local/include/abquant
}
}

windows {
header.files = $$HEADER_FILES $$HEADER_CLASSES
win32-msvc* {
QMAKE_CXXFLAGS += /source-charset:utf-8 /wd 4819 /wd 4661
}

script.files = ../tfenv.bat
script.path = $$target.path
test.files = $$TEST_FILES $$TEST_CLASSES
test.path = $$header.path/TfTest
# INSTALLS += header script test
} else:unix {
macx:QMAKE_SONAME_PREFIX=@rpath

header.files = $$HEADER_FILES $$HEADER_CLASSES

test.files = $$TEST_FILES $$TEST_CLASSES
test.path = $$header.path/ABQtest
# INSTALLS += header test

message(===header.path: $${header.path})
message(===header.files: $${header.files})
# INSTALLS += header
}


for(header, header.files) {
  path = $${header.path}/$${dirname(header)}
  eval(headers_$${path}.files += $$header)
  eval(headers_$${path}.path = $$path)
  eval(INSTALLS *= headers_$${path})
}
