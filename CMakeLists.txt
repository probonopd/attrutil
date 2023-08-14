cmake_minimum_required(VERSION 3.0)
project(attrutil C)

set(CMAKE_C_STANDARD 11)

# Specify the cross-compiler for Musl libc
set(CMAKE_C_COMPILER musl-gcc)

# Define the source files
set(SOURCE_FILES attrutil.c)

# Add the executable and link against Musl libc and libattr
add_executable(attrutil ${SOURCE_FILES})

# Link against Musl libc's static libraries
target_link_libraries(attrutil -static-libgcc -static-libstdc++)

# Link the libattr library statically
find_library(ATTR_LIBRARY attr)
target_link_libraries(attrutil -Wl,-Bstatic ${ATTR_LIBRARY} -Wl,-Bdynamic)

# Add a custom command to execute the setup script
add_custom_command(TARGET attrutil POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Setting up setuid for attrutil..."
    COMMAND chmod u+s $<TARGET_FILE:attrutil>
    COMMAND chown root:root $<TARGET_FILE:attrutil>
    COMMAND chmod 4755 $<TARGET_FILE:attrutil>
    COMMAND ${CMAKE_COMMAND} -E echo "Set up complete."
)
