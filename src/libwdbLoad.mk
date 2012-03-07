libwdbLoad_la_SOURCES += \
				src/LoaderConfiguration.cpp \
				src/LoaderDatabaseConnection.cpp \
				src/wdbSetup.cpp \
				src/transactors/AddPlaceRegularGrid.h \
				src/transactors/AddSrid.h \
				src/transactors/GetPlaceName.h \
				src/transactors/GetSrid.h \
				src/transactors/BeginWci.h \
				src/transactors/EndWci.h \
				src/transactors/loadTransactorWriteByteA.h \
				src/transactors/loaderTransactorValue.h \
				src/transactors/InfoParameterUnit.h \
				src/FloatDataEntry.cpp
	
pkginclude_HEADERS += \
				src/LoaderDatabaseInterface.h \
				src/LoaderConfiguration.h \
				src/LoaderDatabaseConnection.h \
				src/wdbSetup.h \
				src/WdbLevel.h \
				src/FloatDataEntry.h
