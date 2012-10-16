TESTS = database_functionality_test

check_PROGRAMS = database_functionality_test

database_functionality_test_SOURCES = test/DatabaseFunctionalityTest.cpp $(BOOST_UNIT_TEST_FRAMEWORK_CPPFLAGS)
database_functionality_test_LDFLAGS = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(BOOST_SYSTEM_LIB) 
database_functionality_test_LDADD = $(top_builddir)/libwdbLoad.la $(BOOST_PROGRAM_OPTIONS_LIB)