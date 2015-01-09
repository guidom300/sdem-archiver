CONFIG += c++11 warn_on debug

QMAKE_CXXFLAGS_WARN_ON += -pedantic -Wextra
QMAKE_CXXFLAGS += -pthread
LIBS += -pthread
