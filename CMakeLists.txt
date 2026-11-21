cmake_minimum_required(VERSION 3.12)
project(PagingSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# 1. Chương trình chính tích hợp
add_executable(paging_sim
    src/main.cpp
    src/page_table.cpp
    src/address_translation.cpp
)

# 2. Demo riêng của Hoàng
add_executable(hoang_demo
    examples/hoang_page_table_demo.cpp
    src/page_table.cpp
)

# 3. Chạy dịch từ tệp (batch)
add_executable(run_from_files
    examples/run_from_files.cpp
    src/page_table.cpp
    src/address_translation.cpp
)

# 4. Kiểm tra độc lập
add_executable(check_independent
    examples/check_independent.cpp
)
