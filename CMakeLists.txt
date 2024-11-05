project(ryzenai_crash)
set(CMAKE_CXX_STANDARD 20)

add_executable(ryzenai_crash main.cpp)

file(GLOB dlls "${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime/bin/*")

add_custom_command(TARGET ryzenai_crash POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dlls}" $<TARGET_FILE_DIR:ryzenai_crash> COMMAND_EXPAND_LISTS)
add_custom_command(TARGET ryzenai_crash POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime/vaip_config.json" $<TARGET_FILE_DIR:ryzenai_crash> COMMAND_EXPAND_LISTS)