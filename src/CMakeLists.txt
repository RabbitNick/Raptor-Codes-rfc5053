

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)


find_package(Boost COMPONENTS system filesystem  program_options timer  REQUIRED)


SET (MAIN_SRCS  main.cc)

ADD_EXECUTABLE (main ${MAIN_SRCS})


INCLUDE_DIRECTORIES  ("${PROJECT_SOURCE_DIR}/src/libraptor" ${Boost_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(main raptor ${Boost_LIBRARIES})



ADD_SUBDIRECTORY (libraptor)