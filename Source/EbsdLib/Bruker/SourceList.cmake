#--////////////////////////////////////////////////////////////////////////////
#--
#--  Copyright (c) 2009, 2010, 2013 Michael A. Jackson. BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--
#--////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
#// This code was partly written under US Air Force Contract FA8650-07-D-5800
#///////////////////////////////////////////////////////////////////////////////

#-- Get the BRUKER Sources
set(BRUKER_SRCS
    ${EbsdLib_SOURCE_DIR}/BRUKER/BrukerReader.cpp
    ${EbsdLib_SOURCE_DIR}/BRUKER/BrukerPhase.cpp
    ${EbsdLib_SOURCE_DIR}/BRUKER/BrukerFields.cpp
 )

set(BRUKER_HDRS
    ${EbsdLib_SOURCE_DIR}/BRUKER/BrukerConstants.h
    ${EbsdLib_SOURCE_DIR}/BRUKER/BrukerHeaderEntry.h
    ${EbsdLib_SOURCE_DIR}/BRUKER/BrukerReader.h
    ${EbsdLib_SOURCE_DIR}/BRUKER/BrukerPhase.h
    ${EbsdLib_SOURCE_DIR}/BRUKER/BrukerFields.h
)

if (EbsdLib_ENABLE_HDF5)
    add_definitions(-DEbsdLib_HAVE_HDF5)
    set(BRUKER_SRCS ${BRUKER_SRCS}
        ${EbsdLib_SOURCE_DIR}/BRUKER/H5BrukerImporter.cpp
        ${EbsdLib_SOURCE_DIR}/BRUKER/H5BrukerReader.cpp
        ${EbsdLib_SOURCE_DIR}/BRUKER/H5BrukerVolumeReader.cpp
    )
    set(BRUKER_HDRS ${BRUKER_HDRS}
        ${EbsdLib_SOURCE_DIR}/BRUKER/H5BrukerImporter.h
        ${EbsdLib_SOURCE_DIR}/BRUKER/H5BrukerReader.h
        ${EbsdLib_SOURCE_DIR}/BRUKER/H5BrukerVolumeReader.h
    )
    set(EbsdLib_HDF5_SUPPORT 1)
endif()
cmp_IDE_SOURCE_PROPERTIES( "EbsdLib/BRUKER" "${BRUKER_HDRS}" "${BRUKER_SRCS}" ${PROJECT_INSTALL_HEADERS})

if ( ${EbsdLib_INSTALL_FILES} EQUAL 1 )
    INSTALL (FILES ${BRUKER_HDRS}
            DESTINATION include/EbsdLib/BRUKER
            COMPONENT Headers   )
endif()
