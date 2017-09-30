cmake_minimum_required(VERSION 2.8)
project(qtableview)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
add_executable(qtableview
	main.cc
	table_model.cc
	table_view.cc
)
target_compile_options(qtableview PRIVATE -std=c++14 -g)

# due to Qt5
target_compile_options(qtableview PRIVATE -Wno-inconsistent-missing-override)
qt5_use_modules(qtableview Widgets)
