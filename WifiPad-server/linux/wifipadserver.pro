# -------------------------------------------------
# Project created by QtCreator 2009-06-14T07:48:13
# -------------------------------------------------
TARGET = WifiPadServer
TEMPLATE = app
SOURCES += main.cpp \
    MainWindow.cpp \
    ../../libWifiPad/SocketDataSource.cpp \
    ../../libWifiPad/Socket.cpp \
	../../libWifiPad/Server.cpp \
    ../../libWifiPad/LimitReader.cpp \
    ../../libWifiPad/GamePad.cpp \
    ../../libWifiPad/FileDataSource.cpp \
    ../../libWifiPad/FileDataSink.cpp \
    ../../libWifiPad/Directory.cpp \
    ../../libWifiPad/Device.cpp \
    ../../libWifiPad/DataSource.cpp \
    ../../libWifiPad/Configuration.cpp \
    ../../libWifiPad/BufferedInputReader.cpp \
    ../../libWifiPad/linux/QtImage.cpp \
    GamePadView.cpp \
    ButtonConfigDialog.cpp \
    ../../libWifiPad/linux/KeySyms.cpp \
    NewConfigurationDialog.cpp \
    ../../libWifiPad/linux/OS.cpp
HEADERS += MainWindow.h \
    ../../libWifiPad/Uuid.h \
    ../../libWifiPad/Thread.h \
    ../../libWifiPad/SocketDataSource.h \
    ../../libWifiPad/Socket.h \
    ../../libWifiPad/Server.h \
    ../../libWifiPad/Protocol.h \
    ../../libWifiPad/OS.h \
    ../../libWifiPad/Mutex.h \
    ../../libWifiPad/LimitReader.h \
    ../../libWifiPad/KeySyms.h \
    ../../libWifiPad/Image.h \
    ../../libWifiPad/GamePad.h \
    ../../libWifiPad/FileDataSource.h \
    ../../libWifiPad/FileDataSink.h \
    ../../libWifiPad/Directory.h \
    ../../libWifiPad/Device.h \
    ../../libWifiPad/DataSource.h \
    ../../libWifiPad/DataSink.h \
    ../../libWifiPad/Configuration.h \
    ../../libWifiPad/BufferedInputReader.h \
    ../../libWifiPad/Ar.h \
    ../../libWifiPad/linux/QtImage.h \
    GamePadView.h \
    ButtonConfigDialog.h \
    NewConfigurationDialog.h
FORMS += MainWindow.ui \
    ButtonConfigDialog.ui \
    NewConfigurationDialog.ui
INCLUDEPATH = .;../../libs;../../libWifiPad;../../libWifiPad/linux
LIBS = -lXtst -luuid
