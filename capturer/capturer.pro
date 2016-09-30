TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    frame-buffer.cpp \
    frame-data.cpp \
    utils.cpp \
    namespacer.cpp \
    name-components.cpp \
    publisher.cpp

HEADERS += \
    capturer.hpp \
    encoder.hpp \
    tmp.hpp \
    frame-buffer.h \
    frame-data.h \
    common.h \
    utils.h \
    namespacer.h \
    name-components.h \
    publisher.h \
    logger.hpp

LIBS += \
    -lboost_thread \
    -lboost_system \
    -lboost_chrono \
    -lboost_regex \
    -lndn-cpp \
    -lpthread \
    -lprotobuf \
    -lsqlite3 \
    -lcrypto \
    -ldl

CONFIG += C++11

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include


# include ffmpeg

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ffmpeg/release/ -lavutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ffmpeg/debug/ -lavutil
else:unix: LIBS += -L$$PWD/lib/ffmpeg/ -lavutil

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ffmpeg/release/ -lavcodec
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ffmpeg/debug/ -lavcodec
else:unix: LIBS += -L$$PWD/lib/ffmpeg/ -lavcodec

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ffmpeg/release/ -lpostproc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ffmpeg/debug/ -lpostproc
else:unix: LIBS += -L$$PWD/lib/ffmpeg/ -lpostproc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ffmpeg/release/ -lswscale
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ffmpeg/debug/ -lswscale
else:unix: LIBS += -L$$PWD/lib/ffmpeg/ -lswscale

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ffmpeg/release/ -lavdevice
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ffmpeg/debug/ -lavdevice
else:unix: LIBS += -L$$PWD/lib/ffmpeg/ -lavdevice

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ffmpeg/release/ -lavformat
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ffmpeg/debug/ -lavformat
else:unix: LIBS += -L$$PWD/lib/ffmpeg/ -lavformat

INCLUDEPATH += $$PWD/include/ffmpeg
DEPENDPATH += $$PWD/include/ffmpeg


# include glog

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/glog/release/ -lglog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/glog/debug/ -lglog
else:unix: LIBS += -L$$PWD/lib/glog/ -lglog

INCLUDEPATH += $$PWD/include/glog
DEPENDPATH += $$PWD/include/glog


#
