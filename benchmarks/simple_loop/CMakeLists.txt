#SIMPLE LOOP

add_executable(simple_loop main.c)
target_link_libraries (simple_loop m)
if(CUDA_CUPTI)
    target_link_libraries (simple_loop cupti)
endif()