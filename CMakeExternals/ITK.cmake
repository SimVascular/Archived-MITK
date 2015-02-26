#-----------------------------------------------------------------------------
# ITK
#-----------------------------------------------------------------------------

# Sanity checks
if(DEFINED ITK_DIR AND NOT EXISTS ${ITK_DIR})
  message(FATAL_ERROR "ITK_DIR variable is defined but corresponds to non-existing directory")
endif()

set(proj ITK)
set(proj_DEPENDENCIES GDCM)

if(MITK_USE_OpenCV)
  list(APPEND proj_DEPENDENCIES OpenCV)
endif()

if(MITK_USE_HDF5)
  list(APPEND proj_DEPENDENCIES HDF5)
endif()

set(ITK_DEPENDS ${proj})

if(NOT DEFINED ITK_DIR)

  set(additional_cmake_args )
  if(MINGW)
    set(additional_cmake_args
        -DCMAKE_USE_WIN32_THREADS:BOOL=ON
        -DCMAKE_USE_PTHREADS:BOOL=OFF)
  endif()

  list(APPEND additional_cmake_args
       -DUSE_WRAP_ITK:BOOL=OFF
      )

  if(MITK_USE_OpenCV)
    list(APPEND additional_cmake_args
         -DModule_ITKVideoBridgeOpenCV:BOOL=ON
         -DOpenCV_DIR:PATH=${OpenCV_DIR}
        )
  endif()

  # Keep the behaviour of ITK 4.3 which by default turned on ITK Review
  # see MITK bug #17338
  list(APPEND additional_cmake_args
    -DModule_ITKReview:BOOL=ON
  )

  set(vcl_constexpr_patch)
  if(GCC_VERSION VERSION_LESS 4.7 AND GCC_VERSION VERSION_GREATER 4)
    set(vcl_constexpr_patch
      COMMAND ${PATCH_COMMAND} -N -p1 -i ${CMAKE_CURRENT_LIST_DIR}/ITK-4.5.1-gcc-4.6.patch
    )
  endif()

  ExternalProject_Add(${proj}
     LIST_SEPARATOR ${sep}
     URL ${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/InsightToolkit-4.5.1-3e550bf8.tar.gz
     URL_MD5 80e433ffc0e81cdc19a03dd02a3c329b
     # work with external GDCM
     PATCH_COMMAND ${PATCH_COMMAND} -N -p1 -i ${CMAKE_CURRENT_LIST_DIR}/ITK-4.5.1.patch
                   ${vcl_constexpr_patch}
     CMAKE_GENERATOR ${gen}
     CMAKE_ARGS
       ${ep_common_args}
       ${additional_cmake_args}
       -DBUILD_EXAMPLES:BOOL=OFF
       -DITK_USE_SYSTEM_GDCM:BOOL=ON
       -DGDCM_DIR:PATH=${GDCM_DIR}
       -DITK_USE_SYSTEM_HDF5:BOOL=${MITK_USE_HDF5}
       -DHDF5_DIR:PATH=${HDF5_DIR}
     DEPENDS ${proj_DEPENDENCIES}
    )

  set(ITK_DIR ${ep_prefix})
  mitkFunctionInstallExternalCMakeProject(${proj})

else()

  mitkMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")

endif()
