project(interpreter)

add_library(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/interpreter.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_HOME_DIRECTORY}/lexer/inc)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_HOME_DIRECTORY}/lexer/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_HOME_DIRECTORY}/parser/inc)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_HOME_DIRECTORY}/parser/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_HOME_DIRECTORY}/evaluator/inc)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_HOME_DIRECTORY}/evaluator/src)