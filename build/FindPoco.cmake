# - try to find Poco libraries and include files
# POCO_INCLUDE_DIR where to find Standard.hxx, etc.
# POCO_LIBRARIES libraries to link against
# POCO_FOUND If false, do not try to use POCO

find_path ( POCO_INCLUDE_DIR Poco/Poco.h )

find_library ( POCO_LIB_XML NAMES PocoXML )

find_library ( POCO_LIB_XML_DEBUG NAMES PocoXMLd )

if ( POCO_LIB_FOUNDATION_DEBUG )

set ( POCO_LIBRARIES optimized ${POCO_LIB_XML}
                     debug ${POCO_LIB_XML_DEBUG}
)

else ()

set ( POCO_LIBRARIES ${POCO_LIB_XML} )

endif()

# handle the QUIETLY and REQUIRED arguments and set POCO_FOUND to TRUE if 
# all listed variables are TRUE
include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Poco DEFAULT_MSG POCO_LIBRARIES POCO_INCLUDE_DIR )

mark_as_advanced ( POCO_INCLUDE_DIR
                   POCO_LIB_XML POCO_LIB_XML_DEBUG
)
