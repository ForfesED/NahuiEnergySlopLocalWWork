# CMakeList.txt : CMake project for brotli, include source and define
# git: https://github.com/google/brotli/
cmake_minimum_required(VERSION 3.20)

set(TargetName brotli)

project(${TargetName} C)

set(HEADERS
	common/constants.h
	common/context.h
	common/dictionary.h
	common/platform.h
	common/transform.h
	common/version.h

	dec/bit_reader.h
	dec/huffman.h
	dec/prefix.h
	dec/state.h
	
	enc/backward_references.h
	enc/backward_references_hq.h
	enc/backward_references_inc.h
	enc/bit_cost.h
	enc/bit_cost_inc.h
	enc/block_encoder_inc.h
	enc/block_splitter.h
	enc/block_splitter_inc.h
	enc/brotli_bit_stream.h
	enc/cluster.h
	enc/cluster_inc.h
	enc/command.h
	enc/compress_fragment.h
	enc/compress_fragment_two_pass.h
	enc/dictionary_hash.h
	enc/encoder_dict.h
	enc/entropy_encode.h
	enc/entropy_encode_static.h
	enc/fast_log.h
	enc/find_match_length.h
	enc/hash.h
	enc/hash_composite_inc.h
	enc/hash_forgetful_chain_inc.h
	enc/hash_longest_match64_inc.h
	enc/hash_longest_match_inc.h
	enc/hash_longest_match_quickly_inc.h
	enc/hash_rolling_inc.h
	enc/hash_to_binary_tree_inc.h
	enc/histogram.h
	enc/histogram_inc.h
	enc/literal_cost.h
	enc/memory.h
	enc/metablock.h
	enc/metablock_inc.h
	enc/params.h
	enc/prefix.h
	enc/quality.h
	enc/ringbuffer.h
	enc/static_dict.h
	enc/static_dict_lut.h
	enc/utf8_util.h
	enc/write_bits.h

	include/brotli/decode.h
	include/brotli/encode.h
	include/brotli/port.h
	include/brotli/types.h
)

set(SOURCES
	common/constants.c
	common/context.c
	common/dictionary.c
	common/platform.c
	common/transform.c

	dec/bit_reader.c
	dec/decode.c
	dec/huffman.c
	dec/state.c

	enc/backward_references.c
	enc/backward_references_hq.c
	enc/bit_cost.c
	enc/block_splitter.c
	enc/brotli_bit_stream.c
	enc/cluster.c
	enc/command.c
	enc/compress_fragment.c
	enc/compress_fragment_two_pass.c
	enc/dictionary_hash.c
	enc/encode.c
	enc/encoder_dict.c
	enc/memory.c
	enc/entropy_encode.c
	enc/fast_log.c
	enc/histogram.c
	enc/literal_cost.c
	enc/metablock.c
	enc/static_dict.c
	enc/utf8_util.c
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES} ${HEADERS})

add_library(${TargetName} STATIC)

target_include_directories(${TargetName} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/3rdPartyLibs/brotli/include>
)

target_sources(brotli PRIVATE ${HEADERS} ${SOURCES})

#add_nau_compile_options(brotli)
#add_nau_folder_property(brotli LIB arc)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})
