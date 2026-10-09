# CMakeList.txt : CMake project for zlib-ng, include source and define
# git: https://github.com/zlib-ng/zlib-ng

cmake_minimum_required(VERSION 3.20)

set(TargetName zlib-ng)

project(${TargetName} C)

set(HEADERS
	adler32_p.h
	chunkset_tpl.h
	cpu_features.h
	crc32_comb_tbl.h
	crc32_fold.h
	crc32_p.h
	crc32_tbl.h
	deflate.h
	deflate_p.h
	fallback_builtins.h
	functable.h
	gzguts.h
	inffast.h
	inffixed_tbl.h
	inflate.h
	inflate_p.h
	inftrees.h
	insert_string_tpl.h
	match_tpl.h
	trees.h
	trees_emit.h
	trees_tbl.h
	zbuild.h
	zconf-ng.h
	zconf.h
	zendian.h
	zlib-ng.h
	zlib.h
	zlib_name_mangling-ng.h
	zlib_name_mangling.h
	zutil.h
	zutil_p.h

	arch/x86/adler32_ssse3.c
	arch/x86/chunkset_sse2.c
	arch/x86/compare256_sse2.c
	arch/x86/crc32_fold_pclmulqdq.c
	arch/x86/crc32_fold_vpclmulqdq.c
	arch/x86/insert_string_sse42.c
	arch/x86/slide_hash_sse2.c
	arch/x86/x86_features.c
)

set(SOURCES
	adler32.c
	chunkset.c
	compare256.c
	compress.c
	cpu_features.c
	crc32.c
	crc32_comb.c
	crc32_fold.c
	deflate.c
	deflate_fast.c
	deflate_huff.c
	deflate_medium.c
	deflate_quick.c
	deflate_rle.c
	deflate_slow.c
	deflate_stored.c
	functable.c
	gzlib.c
	gzread.c
	gzwrite.c
	infback.c
	inffast.c
	inflate.c
	inftrees.c
	insert_string.c
	insert_string_roll.c
	slide_hash.c
	trees.c
	uncompr.c
	zutil.c

	arch/x86/x86_features.h
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES} ${HEADERS})

add_library(${TargetName} STATIC)

target_sources(${TargetName} PRIVATE ${HEADERS} ${SOURCES})

target_include_directories(${TargetName} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include/3rdPartyLibs/zlib-ng>
)

if (Compiler_ClangCl)
	target_compile_options(zlib-ng PRIVATE /clang:-Wno-error=deprecated-non-prototype)
endif()

#target_link_libraries()

if(WIN32)
	#NOTE: ZLIB_COMPAT must be public
	target_compile_definitions(${TargetName} PUBLIC ZLIB_COMPAT)
	target_compile_definitions(${TargetName} PRIVATE X86_FEATURES X86_SSE2 X86_SSE2_CHUNKSET X86_SSE2_SLIDEHASH X86_SSSE3)
endif()

#todo other platform

#add_nau_compile_options(zlib-ng)
#add_nau_folder_property(zlib-ng LIB arc)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})
