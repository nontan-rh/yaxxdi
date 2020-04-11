
add_executable(yaxxdi-gen-spec gen-spec.cpp spec.cpp)
target_link_libraries(yaxxdi-gen-spec cxxopts nlohmann_json::nlohmann_json)

add_executable(yaxxdi-gen-header gen-header.cpp spec.cpp)
target_link_libraries(yaxxdi-gen-header cxxopts nlohmann_json::nlohmann_json)

add_executable(yaxxdi-gen-source gen-source.cpp spec.cpp)
target_link_libraries(yaxxdi-gen-source cxxopts nlohmann_json::nlohmann_json)
