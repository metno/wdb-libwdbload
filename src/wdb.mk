libwdbLoaderBase_la_SOURCES += \
				src/LoaderConfiguration.cpp \
				src/LoaderDatabaseConnection.cpp \
				src/wdbSetup.cpp \
				src/transactors/loaderTransactorPlaceDefinition.h \
				src/transactors/loaderTransactorSrid.h \
				src/transactors/loaderTransactorValue.h \
				src/transactors/loaderTransactorUnit.h \
				src/transactors/loaderTransactorWci.h
	
pkginclude_HEADERS += \
				src/LoaderDatabaseInterface.h \
				src/LoaderConfiguration.h \
				src/LoaderDatabaseConnection.h \
				src/wdbSetup.h \
				src/WdbLevel.h
