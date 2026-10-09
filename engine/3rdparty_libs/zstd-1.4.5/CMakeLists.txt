# CMakeList.txt : CMake project for zstd, include source and define
# git: https://github.com/facebook/zstd/

cmake_minimum_required(VERSION 3.20)

set(TargetName zstd-1.4.5)

project(${TargetName} C)

set(HEADERS
	common/bitstream.h
	common/compiler.h
	common/cpu.h
	common/debug.h
	common/error_private.h
	common/fse.h
	common/huf.h
	common/mem.h
	common/pool.h
	common/threading.h
	common/xxhash.h
	common/zstd_errors.h
	common/zstd_internal.h

	compress/hist.h
	compress/zstd_compress_internal.h
	compress/zstd_compress_literals.h
	compress/zstd_compress_sequences.h
	compress/zstd_compress_superblock.h
	compress/zstd_cwksp.h
	compress/zstd_double_fast.h
	compress/zstd_fast.h
	compress/zstd_lazy.h
	compress/zstd_ldm.h
	compress/zstd_opt.h
	compress/zstdmt_compress.h

	decompress/zstd_ddict.h
	decompress/zstd_decompress_block.h
	decompress/zstd_decompress_internal.h

	dictBuilder/cover.h
	dictBuilder/divsufsort.h
	dictBuilder/zdict.h

	legacy/zstd_legacy.h
	legacy/zstd_v01.h
	legacy/zstd_v02.h
	legacy/zstd_v03.h
	legacy/zstd_v04.h
	legacy/zstd_v05.h
	legacy/zstd_v06.h
	legacy/zstd_v07.h
)

set(SOURCES
	common/debug.c
	common/entropy_common.c
	common/error_private.c
	common/fse_decompress.c
	common/pool.c
	common/threading.c
	common/xxhash.c
	common/zstd_common.c

	compress/fse_compress.c
	compress/hist.c
	compress/huf_compress.c
	compress/zstd_compress.c
	compress/zstd_compress_literals.c
	compress/zstd_compress_sequences.c
	compress/zstd_compress_superblock.c
	compress/zstd_double_fast.c
	compress/zstd_fast.c
	compress/zstd_lazy.c
	compress/zstd_ldm.c
	compress/zstd_opt.c
	compress/zstdmt_compress.c

	decompress/huf_decompress.c
	decompress/zstd_ddict.c
	decompress/zstd_decompress.c
	decompress/zstd_decompress_block.c

	dictBuilder/cover.c
	dictBuilder/divsufsort.c
	dictBuilder/fastcover.c
	dictBuilder/zdict.c

	legacy/zstd_v01.c
	legacy/zstd_v02.c
	legacy/zstd_v03.c
	legacy/zstd_v04.c
	legacy/zstd_v05.c
	legacy/zstd_v06.c
	legacy/zstd_v07.c
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES} ${HEADERS})
add_library(${TargetName} STATIC)
add_library(3rdPartyLibs_zstd ALIAS ${TargetName})

target_include_directories(${TargetName} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
	$<INSTALL_INTERFACE:include/3rdPartyLibs/zstd-1.4.5>
)

target_sources(${TargetName} PRIVATE ${HEADERS} ${SOURCES})

target_compile_definitions(${TargetName} PRIVATE ZSTD_LEGACY_SUPPORT XXH_CPU_LITTLE_ENDIAN)

#add_nau_compile_options(zstd-1.4.5)
#add_nau_folder_property(zstd-1.4.5 LIB arc)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})
