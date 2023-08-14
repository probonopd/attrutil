#!/bin/sh

# Create a temporary test file
test_file="/tmp/test_file"
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

# Compare the written value with the read value
expected_value="Test attribute value"
if [ "$read_value" = "$expected_value" ]; then
    echo "Attribute value matches the expected value."
else
    echo "Attribute value does not match the expected value."
fi

# Clean up: Delete the test file
rm "$test_file"

echo "Test complete."
