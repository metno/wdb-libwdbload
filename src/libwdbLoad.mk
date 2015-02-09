libwdbLoad_la_SOURCES += \
				src/LoaderConfiguration.cpp \
				src/LoaderDatabaseConnection.cpp \
				src/wdbSetup.cpp \
				src/errors.cpp \
				src/transactors/AddPlaceRegularGrid.h \
				src/transactors/AddSrid.h \
				src/transactors/GetPlaceName.h \
				src/transactors/GetSrid.h \
				src/transactors/BeginWci.h \
				src/transactors/EndWci.h \
				src/transactors/loadTransactorWriteByteA.h \
				src/transactors/loaderTransactorValue.h \
				src/transactors/InfoParameterUnit.h
	
pkginclude_HEADERS += \
				src/LoaderDatabaseInterface.h \
				src/LoaderConfiguration.h \
				src/LoaderDatabaseConnection.h \
				src/wdbSetup.h \
				src/errors.h \
				src/WdbLevel.h

include src/errorlister/libwdbLoad.mk