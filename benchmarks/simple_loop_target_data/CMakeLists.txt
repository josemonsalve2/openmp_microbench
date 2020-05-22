#SIMPLE LOOP

add_executable(simple_loop_target_data main.c)
target_link_libraries (simple_loop_target_data m)
if(CUDA_CUPTI)
    target_link_libraries (simple_loop_target_data cupti)
endif()