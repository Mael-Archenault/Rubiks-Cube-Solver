# cmake_minimum_required(VERSION 3.10) # Specify the minimum version of CMake
# cmake_policy(SET CMP0072 NEW) # Add this line
# # Define the project name, version (optional), and language
# project(Cube VERSION 1.0 LANGUAGES CXX)

# # Set the C++ standard (e.g., C++17)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforce the standard
# set(CMAKE_CXX_EXTENSIONS OFF)       # Disable compiler-specific extensions for portability

# # Find all .cpp files in the src directory
# file(GLOB SOURCE_FILES "src/*.cpp")

# # Add the executable target
# # This tells CMake to compile main.cpp and any other .cpp files listed
# # and link them into an executable named "MyRenderer"
# add_executable(Cube ${SOURCE_FILES})

# # Specify include directories
# # This tells the compiler where to find header files (#include <...>)
# # ${PROJECT_SOURCE_DIR} is a CMake variable pointing to the project's root directory
# target_include_directories(Cube PUBLIC
# ${PROJECT_SOURCE_DIR}/include
# )


# # Find GLFW and GLEW packages
# find_package(glfw3 REQUIRED)
# find_package(GLEW REQUIRED)

# # Link necessary OpenGL libraries
# find_package(OpenGL REQUIRED)



# # Link libraries to your executable
# target_link_libraries(Cube
#     PRIVATE
#         glfw
#         GLEW::GLEW
#         ${OpenGL_LIBRARIES}
# )


cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW) # Good to keep this as discussed earlier
project(CubeTest)


add_subdirectory(vendor/glm)

# Find GLFW and GLEW packages
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Link necessary OpenGL libraries
find_package(OpenGL REQUIRED) # This finds the libraries and populates OpenGL_LIBRARIES

# Add your executable
add_executable(CubeTest ../src/main.cpp ../src/CubePiece.cpp ../src/RubiksCube.cpp)

target_include_directories(CubeTest PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Link libraries to your executable
target_link_libraries(CubeTest
    PRIVATE
        -Wl,--no-as-needed # Temporarily disable --as-needed to force linkage
        glm
        glfw               # Your code calls GLFW functions
        ${GLEW_SHARED_LIBRARY_RELEASE} # Your code calls GLEW, and GLEW needs OpenGL functions
        ${OpenGL_LIBRARIES} # OpenGL libraries needed by GLEW and potentially your direct calls
        -lGL               # Explicitly link libGL.so (the dispatch library)
        -Wl,--as-needed    # Re-enable --as-needed after explicit libraries
)