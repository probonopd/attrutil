#!/bin/sh

ls -lh $(which attrutil)

# Create a tmpfs
mkdir -p tmp && sudo mount -t tmpfs tmfps tmp
mount | tail -n 1

# Set the setuid bit
sudo chmod u+s $(which attrutil)

# Change ownership to root
sudo chown root:root $(which attrutil)

# Secure permissions
sudo chmod 4755  $(which attrutil)

# Create a test file on the tmpfs
test_file="tmp/test_file"
echo "This is a test file." > "$test_file"

# Run the attrutil program to set, list, and read an attribute
echo "Setting extended attribute..."
attrutil -w myattr "Test attribute value" "$test_file"
write_exit_code=$?

echo "Listing extended attributes..."
attrutil -l "$test_file"
list_exit_code=$?

echo "Reading extended attribute..."
read_value=$(attrutil -r myattr "$test_file")
read_exit_code=$?

# Compare the exit codes
if [ "$write_exit_code" -eq 0 ] && [ "$list_exit_code" -eq 0 ] && [ "$read_exit_code" -eq 0 ]; then
    echo "All attrutil commands succeeded."
else
    echo "One or more attrutil commands failed."
fi

# Clean up: Delete the test file
rm "$test_file"

# Compare the exit codes
if [ "$write_exit_code" -eq 0 ] && [ "$list_exit_code" -eq 0 ] && [ "$read_exit_code" -eq 0 ]; then
    echo "All attrutil commands succeeded."
    exit 0
else
    echo "One or more attrutil commands failed."
    exit 1
fi
