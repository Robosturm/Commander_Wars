cmake_minimum_required (VERSION 2.6)
project (OXYGINESOUND)

set(OXYGINESOUND_INCLUDE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tremor/Tremor
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libogg/include
	)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

message(status ${OXYGINESOUND_INCLUDE_DIRS})

set(OXYGINESOUND_DEFINITIONS	
	-DOXYGINE_SOUND=1)

set(SRC
	src/sound/ResSound.cpp
	src/sound/ResSound.h
	src/sound/Sound.h
	src/sound/SoundInstance.cpp
	src/sound/SoundInstance.h
	src/sound/SoundPlayer.cpp
	src/sound/SoundPlayer.h
	src/sound/SoundSystem.cpp
	src/sound/SoundSystem.h

	src/sound/null/SoundSystemNull.cpp
	src/sound/null/SoundSystemNull.h
	src/sound/null/SoundNull.cpp
	src/sound/null/SoundNull.h
	src/sound/null/SoundHandleNull.cpp
	src/sound/null/SoundHandleNull.h
	)

if(EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(SRC ${SRC}
		src/sound/emscripten/SoundSystemEmscripten.h
		src/sound/emscripten/SoundSystemEmscripten.cpp
		src/sound/emscripten/SoundHandleEmscripten.h
		src/sound/emscripten/SoundHandleEmscripten.cpp
		src/sound/emscripten/SoundEmscripten.h
		src/sound/emscripten/SoundEmscripten.cpp

	)

else(EMSCRIPTEN)

	add_subdirectory(dependencies/tremor tremor)
	
	set (SRC ${SRC}
		src/sound/OggStream.cpp
		src/sound/OggStream.h
		src/sound/MemoryStream.cpp
		src/sound/MemoryStream.h
		src/sound/WavStream.cpp
		src/sound/WavStream.h

		src/sound/oal/SoundHandleOAL.cpp
		src/sound/oal/SoundHandleOAL.h
		src/sound/oal/SoundOAL.cpp
		src/sound/oal/SoundOAL.h
		src/sound/oal/SoundSystemOAL.cpp
		src/sound/oal/SoundSystemOAL.h
		src/sound/oal/StreamingSoundHandleOAL.cpp
		src/sound/oal/StreamingSoundHandleOAL.h
		src/sound/oal/StaticSoundHandleOAL.cpp
		src/sound/oal/StaticSoundHandleOAL.h
		src/sound/oal/StaticStreamOAL.cpp
		src/sound/oal/StaticStreamOAL.h
		)

endif(EMSCRIPTEN)


add_library(oxygine-sound STATIC ${SRC})

if(EMSCRIPTEN)
	set(OXYGINESOUND_LIBS
		oxygine-sound)	

	set(linkFlags "-Wno-warn-absolute-paths")

	set_target_properties(oxygine-sound PROPERTIES LINK_FLAGS "${linkFlags}")

else(EMSCRIPTEN)
	set(OXYGINESOUND_INCLUDE_DIRS ${OXYGINESOUND_INCLUDE_DIRS}
		${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openal/include)

	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	
		set(OXYGINESOUND_LIBS oxygine-sound -lopenal tremor -logg)

	else()

		set(LIBOGG libogg_static)

		if (MSVC)
			if(NOT (MSVC_VERSION LESS 1900))
				set(LIBOGG ${LIBOGG}_vs2013)
			endif()
		endif()	

		set(OXYGINESOUND_LIBS
			${CMAKE_CURRENT_SOURCE_DIR}/libs/win32/OpenAL32.lib
			${CMAKE_CURRENT_SOURCE_DIR}/libs/win32/${LIBOGG}.lib		
			tremor
			oxygine-sound)

	endif()
endif(EMSCRIPTEN)

include_directories(${OXYGINESOUND_INCLUDE_DIRS})




target_link_libraries(oxygine-sound -lopenal)
target_link_libraries(oxygine-sound -logg)


#target_link_libraries(oxyginemagicparticles)
set(OXYGINESOUND_INCLUDE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}/src)

set(OXYGINESOUND_INCLUDE_DIRS ${OXYGINESOUND_INCLUDE_DIRS} PARENT_SCOPE)
set(OXYGINESOUND_LIBS ${OXYGINESOUND_LIBS} PARENT_SCOPE)
set(OXYGINESOUND_DEFINITIONS ${OXYGINESOUND_DEFINITIONS} PARENT_SCOPE)