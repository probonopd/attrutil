# attrutil

Command line tool to set extended attributes on Linux [tmpfs](https://kernel.org/doc/html/next/filesystems/tmpfs.html)

## Motivation

The tmpfs filesystem does not support the `user.*` namespace, and standard unprivileged applications cannot access the `trusted.*` namespace. To overcome this limitation and allow non-root users to manage extended attributes in the `trusted.*` namespace on tmpfs, this program serves as a setuid helper. It is installed with elevated privileges and can perform the required extended attribute operations.

__Note:__ The need for this tool would go away if the tmpfs filesystem supported the `user.*` namespace.

## Usage

`attrutil` provides command-line options to list, read, write, and delete attributes within the `trusted.*` namespace on files. The program should be carefully reviewed and tested before use in any production environment.

**IMPORTANT:**
Setuid programs can introduce security risks if not managed properly. Do not use setuid programs unless you fully understand the implications and take appropriate security precautions.

Below is a list of available options:

```
Usage: attrutil [options] <file>
Options:
  -l, --list          List extended attributes
  -r, --read <attr>   Read the specified attribute
  -w, --write <attr>  <value>
                      Write the specified attribute with the given value
  -d, --delete <attr> Delete the specified attribute
  -h, --help          Display this help message
```

## Building and Installation
1. Install `libattr1-dev` using: `sudo apt-get install libattr1-dev`
2. Compile the program using a command like: `gcc -o attrutil attrutil.c -lattribute`
3. Set the setuid bit using: `chmod u+s attrutil`
4. Change ownership to root using: `sudo chown root:root attrutil`
5. Secure permissions with: `chmod 4755 attrutil`
