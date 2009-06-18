libwdbLoad_la_SOURCES += \
				src/LoaderConfiguration.cpp \
				src/LoaderDatabaseConnection.cpp \
				src/wdbSetup.cpp \
				src/transactors/AddPlaceDefinition.h \
				src/transactors/AddSrid.h \
				src/transactors/GetPlaceName.h \
				src/transactors/GetSrid.h \
				src/transactors/BeginWci.h \
				src/transactors/EndWci.h \
				src/transactors/loadTransactorWriteByteA.h \
				src/transactors/loaderTransactorValue.h \
				src/transactors/loaderTransactorUnit.h
	
pkginclude_HEADERS += \
				src/LoaderDatabaseInterface.h \
				src/LoaderConfiguration.h \
				src/LoaderDatabaseConnection.h \
				src/wdbSetup.h \
				src/WdbLevel.h
