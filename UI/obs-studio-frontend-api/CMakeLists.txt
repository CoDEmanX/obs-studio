project(obs-studio-frontend-api)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")

add_definitions(-DLIBOBS_EXPORTS)

set(obs-studio-frontend-api_SOURCES
	obs-studio-frontend-api.cpp)

set(obs-studio-frontend-api_HEADERS
	obs-studio-frontend-internal.hpp
	obs-studio-frontend-api.h)

add_library(obs-studio-frontend-api SHARED
	${obs-studio-frontend-api_SOURCES}
	${obs-studio-frontend-api_HEADERS})
target_link_libraries(obs-studio-frontend-api
	libobs)

install_obs_core(obs-studio-frontend-api)
