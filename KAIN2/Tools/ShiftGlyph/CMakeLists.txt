cmake_minimum_required (VERSION 3.0)

project (ShiftGlyph_${TARGET_ARCH} VERSION 0.0.1 LANGUAGES CSharp)

#Recursively locate the source files for GAMEWAD_Unpack.
file(GLOB_RECURSE TOOL_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cs)

add_executable(
${PROJECT_NAME}
${TOOL_SRCS}
)

source_group(TREE ${KAIN2_ROOT} FILES ${KAIN2_SRCS})

set_target_properties(
${PROJECT_NAME} PROPERTIES OUTPUT_NAME "ShiftGlyph"
${PROJECT_NAME} PROPERTIES FOLDER "Tools/"
)