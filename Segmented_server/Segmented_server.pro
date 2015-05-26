######################################################################
# Automatically generated by qmake (3.0) dim. mai 24 16:04:55 2015
######################################################################

TEMPLATE = app
TARGET = Segmented_server
INCLUDEPATH += . include
QT += network
QT += xml
CONFIG += warn_off

# Input
HEADERS += include/Client.h \
           include/CommandListener.h \
           include/Demo.h \
           include/DemoManager.h \
           include/EncryptManager.h \
           include/Folder.h \
           include/NetworkManager.h \
           include/Out.h \
           include/XmlManager.h
SOURCES += main.cpp \
           src/Client.cpp \
           src/CommandListener.cpp \
           src/Demo.cpp \
           src/DemoManager.cpp \
           src/EncryptManager.cpp \
           src/Folder.cpp \
           src/NetworkManager.cpp \
           src/XmlManager.cpp