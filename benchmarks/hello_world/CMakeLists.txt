#HELLO WORLD

add_executable(hello_world main.c)
target_link_libraries (hello_world m)
if(CUDA_CUPTI)
    target_link_libraries (hello_world cupti)
endif()