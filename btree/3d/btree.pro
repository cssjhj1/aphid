INCLUDEPATH += ../../shared ../ ./
LIBS += -L../../lib -laphid -lIlmImf -lHalf			
CONFIG += release
HEADERS       = ../Types.h ../Entity.h ../List.h ../BNode.h ../BTree.h ../C3Tree.h glwidget.h window.h ../../shared/Base3DView.h
SOURCES       = ../Types.cpp ../Entity.cpp ../List.cpp ../BNode.cpp ../BTree.cpp ../C3Tree.cpp glwidget.cpp window.cpp main.cpp ../../shared/Base3DView.cpp
QT           += opengl
win32 {
    INCLUDEPATH += D:/usr/boost_1_51_0 /usr/local/include/OpenEXR
    QMAKE_LIBDIR += D:/usr/boost_1_51_0/stage/lib D:/usr/local/openEXR/lib
    CONFIG += console
}
macx {
    INCLUDEPATH += ../../../Library/boost_1_44_0
	LIBS += -lboost_date_time\
            -lboost_thread
}
