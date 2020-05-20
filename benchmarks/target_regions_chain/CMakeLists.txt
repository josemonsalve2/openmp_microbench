#TARGET REGIONS CHAIN

add_executable(target_regions_chain main.c)
target_link_libraries (target_regions_chain m)
if(CUDA_CUPTI)
    target_link_libraries (target_regions_chain cupti)
endif()