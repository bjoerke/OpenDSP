#-------------------------------------------------
#
# Project created by QtCreator 2016-07-19T13:07:58
#
#-------------------------------------------------

QT       += core  gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QMAKE_CXXFLAGS += -std=c++11

TARGET = OpenDSP
CONFIG += console
TEMPLATE = app


SOURCES += OpenDsp/Gui/qcustomplot/qcustomplot.cpp \
    #examples/0_convolution.cpp \
    #examples/1_dft_fft.cpp \
    #examples/2_generators.cpp \
    #examples/3_wav.cpp \
    examples/4_plot.cpp \


HEADERS  += \
    OpenDsp/OpenDsp.hpp \
    OpenDsp/Signal.hpp \
    OpenDsp/FrameIO/FrameReader.hpp \
    OpenDsp/FrameIO/FrameWriter.hpp \
    OpenDsp/FrameIO/WavFileFormat.hpp \
    OpenDsp/FrameIO/WavFileReader.hpp \
    OpenDsp/FrameIO/WavFileWriter.hpp \
    OpenDsp/Generators/SignalGenerator.hpp \
    OpenDsp/Generators/SineGenerator.hpp \
    OpenDsp/Generators/Windows.hpp \
    OpenDsp/Math/Complex.hpp \
    OpenDsp/Transforms/Dft.hpp \
    OpenDsp/Transforms/Fft.hpp \
    OpenDsp/Transforms/RealDft.hpp \
    OpenDsp/Transforms/RealFft.hpp \
    OpenDsp/Transforms/Transform.hpp \
    OpenDsp/Gui/qcustomplot/qcustomplot.h \
    OpenDsp/Gui/PlotWindow.hpp

FORMS    +=

LIBS += -lsndfile
