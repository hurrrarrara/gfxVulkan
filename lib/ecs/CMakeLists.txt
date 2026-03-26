add_library(ecs INTERFACE)

target_include_directories(ecs INTERFACE include)

target_compile_features(ecs INTERFACE cxx_std_20)
