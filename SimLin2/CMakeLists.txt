# CMakeList.txt : CMake project for SimLin2, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (SimLin2 "SimLin2.cpp" 
						"SimLin2.h" 
						"ui/windows/SandboxWindow.h" 
						"ui/windows/SandboxWindow.cpp" 
						"ui/windows/SetupWindow.h" 
						"ui/windows/SetupWindow.cpp" 				 
						"NodeManager.h" 
						"ui/Camera.h" 
						"ui/Camera.cpp" 
						"ui/CameraController.h" 
						"ui/CameraController.cpp" 
						"ui/Viewport.h" 
						"ui/RenderContext.h" 
						"ui/GridRenderer.h" 
						"ui/GridRenderer.cpp" 
						"ui/NOdeRenderer.h" 
						"ui/NodeRenderer.cpp"  
						"managers/MaterialManager.cpp" 
						"ui/windows/MatEditorWindow.h" 
						"ui/windows/MatEditorWindow.cpp" 
						"ui/windows/Window.h" 
						"ui/windows/Window.cpp")

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(SimLin2 PRIVATE SDL3::SDL3)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(SimLin2 PRIVATE imgui::imgui)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SimLin2 PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
