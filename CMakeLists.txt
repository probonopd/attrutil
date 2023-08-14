cmake_minimum_required(VERSION 3.0)
project(attrutil C)

set(CMAKE_C_STANDARD 11)

add_executable(attrutil attrutil.c)

# Link the libattr library
target_link_libraries(attrutil attr)

# Add a custom command to execute the setup script
add_custom_command(TARGET attrutil POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Setting up setuid for attrutil..."
    COMMAND chmod u+s $<TARGET_FILE:attrutil>
    COMMAND chown root:root $<TARGET_FILE:attrutil>
    COMMAND chmod 4755 $<TARGET_FILE:attrutil>
    COMMAND ${CMAKE_COMMAND} -E echo "Set up complete."
)
