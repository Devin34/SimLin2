# CMakeList.txt : CMake project for SimLin2, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (SimLin2 "SimLin2.cpp" "SimLin2.h" "ui/SandboxWindow.h" "ui/SandboxWindow.cpp" "ui/SetupWindow.h" "ui/SetupWindow.cpp" "ui/AddNewNodeWindow.h" "ui/AddNewNodeWindow.cpp" "NodeManager.h")

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(SimLin2 PRIVATE SDL3::SDL3)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(SimLin2 PRIVATE imgui::imgui)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SimLin2 PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
