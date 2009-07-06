INCLUDES = -I$(srcdir)/../libWifiPad/null -I$(srcdir)/../libWifiPad -I$(srcdir)/../libs
bin_PROGRAMS = GamepadVerify
GamepadVerify_CPPFLAGS = -DNO_QT=1
GamepadVerify_LDADD = -luuid
GamepadVerify_SOURCES = $(srcdir)/GamepadVerify.cpp \
	$(srcdir)/../libWifiPad/BufferedInputReader.cpp \
	$(srcdir)/../libWifiPad/DataSource.cpp \
	$(srcdir)/../libWifiPad/LimitReader.cpp \
	$(srcdir)/../libWifiPad/linux/OS.cpp \
	$(srcdir)/../libWifiPad/null/KeySyms.cpp \
	$(srcdir)/../libWifiPad/FileDataSink.cpp \
	$(srcdir)/../libWifiPad/FileDataSource.cpp \
	$(srcdir)/../libWifiPad/GamePad.cpp
