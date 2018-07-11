cmake_minimum_required (VERSION 2.6)
project (TREMOR)

set(TREMOR_SRC Tremor)
file(GLOB SRC
		${TREMOR_SRC}/asm_arm.h
		${TREMOR_SRC}/backends.h
		${TREMOR_SRC}/block.c
		${TREMOR_SRC}/block.h
		${TREMOR_SRC}/codebook.c
		${TREMOR_SRC}/codebook.h
		${TREMOR_SRC}/codec_internal.h
		${TREMOR_SRC}/config_types.h
		${TREMOR_SRC}/floor0.c
		${TREMOR_SRC}/floor1.c
		${TREMOR_SRC}/info.c
		${TREMOR_SRC}/ivorbiscodec.h
		${TREMOR_SRC}/ivorbisfile.h
		${TREMOR_SRC}/lsp_lookup.h
		${TREMOR_SRC}/mapping0.c
		${TREMOR_SRC}/mdct.c
		${TREMOR_SRC}/mdct.h
		${TREMOR_SRC}/mdct_lookup.h
		${TREMOR_SRC}/misc.h
		${TREMOR_SRC}/os.h
		${TREMOR_SRC}/registry.c
		${TREMOR_SRC}/registry.h
		${TREMOR_SRC}/res012.c
		${TREMOR_SRC}/sharedbook.c
		${TREMOR_SRC}/synthesis.c
		${TREMOR_SRC}/vorbisfile.c
		${TREMOR_SRC}/window.c
		${TREMOR_SRC}/window.h
		${TREMOR_SRC}/window_lookup.h)

include_directories(../libogg/include/)
add_library(tremor STATIC ${SRC})