set(GIT_COMMIT_COUNT "0")
 
find_package(Git)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-list HEAD --count
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT_COUNT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
#  message( STATUS "GIT hash: ${GIT_COMMIT_COUNT}")
else()
  message(STATUS "GIT not found")
endif()
 
string(TIMESTAMP _time_stamp)
 
configure_file(../../../KAIN2/Editor/Engine_Version.h.in ../../../KAIN2/Editor/Engine_Version.h @ONLY)