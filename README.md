# attrutil

Command line tool to set extended attributes

## Motivation

The tmpfs filesystem does not support `user.*` namespaces, and standard unprivileged applications cannot access the `trusted.*` namespace on tmpfs mounts. To overcome this limitation and allow non-root users to manage extended attributes in the `trusted.*` namespace on tmpfs, this program serves as a setuid helper. It is installed with elevated privileges and can perform the required extended attribute operations.

## Usage

The program provides command-line options to list, read, write, and delete attributes within the `trusted.*` namespace on files. The program should be carefully reviewed and tested before use in any production environment.

**IMPORTANT:**
Setuid programs can introduce security risks if not managed properly. Do not use setuid programs unless you fully understand the implications and take appropriate security precautions.

## Building and Installation
1. Install `libattr1-dev` using: `sudo apt-get install libattr1-dev`
2. Compile the program using a command like: `gcc -o attrutil attrutil.c -lattribute`
3. Set the setuid bit using: `chmod u+s attrutil`
4. Change ownership to root using: `sudo chown root:root attrutil`
5. Secure permissions with: `chmod 4755 attrutil`
