// This program provides command-line options to list, read, write, and delete attributes within the
// "trusted.*" namespace on files. The program should be carefully reviewed and tested before use 
// in any production environment.
//
// IMPORTANT:
// Setuid programs can introduce security risks if not managed properly.
// Do not use setuid programs unless you fully understand the implications and take appropriate
// security precautions.
//
// Installation:
// 0. sudo apt-get libattr1-dev
// 1. Compile the program using a command like: gcc -o attrutil attrutil.c -lattribute
// 2. Set the setuid bit using: chmod u+s attrutil
// 3. Change ownership to root using: sudo chown root:root attrutil
// 4. Secure permissions with: chmod 4755 attrutil

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <attr/xattr.h>
#include <getopt.h>
#include <unistd.h>

#define MAX_ATTR_NAME_LENGTH 100
#define MAX_ATTR_VALUE_LENGTH 1000

void display_usage() {
    printf("Usage: setuid_attr_app [options] <file>\n");
    printf("Options:\n");
    printf("  -l, --list         List extended attributes\n");
    printf("  -r, --read <attr>  Read the specified attribute\n");
    printf("  -w, --write <attr> <value>\n");
    printf("                     Write the specified attribute with the given value\n");
    printf("  -d, --delete <attr> Delete the specified attribute\n");
    printf("  -h, --help         Display this help message\n");
}

void list_attributes(const char *path) {
    char list[1024];
    ssize_t len = listxattr(path, list, sizeof(list));

    if (len == -1) {
        perror("listxattr");
        exit(EXIT_FAILURE);
    }

    for (char *attr = list; attr < list + len; attr += strlen(attr) + 1) {
        if (strncmp(attr, "trusted.", 8) == 0) {
            printf("%s\n", attr + 8);
        }
    }
}

void read_attribute(const char *path, const char *attr_name) {
    char buffer[MAX_ATTR_VALUE_LENGTH];
    char full_attr_name[MAX_ATTR_NAME_LENGTH];
    snprintf(full_attr_name, sizeof(full_attr_name), "trusted.%s", attr_name);

    ssize_t len = getxattr(path, full_attr_name, buffer, sizeof(buffer));

    if (len == -1) {
        perror("getxattr");
        exit(EXIT_FAILURE);
    }

    buffer[len] = '\0';

    printf("%s\n", buffer);
}

void write_attribute(const char *path, const char *attr_name, const char *value) {
    char full_attr_name[MAX_ATTR_NAME_LENGTH];
    snprintf(full_attr_name, sizeof(full_attr_name), "trusted.%s", attr_name);

    if (strlen(value) >= MAX_ATTR_VALUE_LENGTH) {
        fprintf(stderr, "Error: Attribute value too long.\n");
        exit(EXIT_FAILURE);
    }

    if (setxattr(path, full_attr_name, value, strlen(value), 0) == -1) {
        perror("setxattr");
        exit(EXIT_FAILURE);
    }
    // printf("Attribute %s set to %s\n", full_attr_name, value);
}

void delete_attribute(const char *path, const char *attr_name) {
    char full_attr_name[MAX_ATTR_NAME_LENGTH];
    snprintf(full_attr_name, sizeof(full_attr_name), "trusted.%s", attr_name);

    if (removexattr(path, full_attr_name) == -1) {
        perror("removexattr");
        exit(EXIT_FAILURE);
    }
    // printf("Attribute %s deleted\n", full_attr_name);
}

int main(int argc, char *argv[]) {
    struct option options[] = {
        {"list", no_argument, NULL, 'l'},
        {"read", required_argument, NULL, 'r'},
        {"write", required_argument, NULL, 'w'},
        {"delete", required_argument, NULL, 'd'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    int opt;
    int option_index = 0;

    if (argc == 1) {
        fprintf(stderr, "Error: Missing arguments.\n");
        display_usage();
        exit(EXIT_FAILURE);
    }

    // Parse command line options
    while ((opt = getopt_long(argc, argv, "lr:w:d:h", options, &option_index)) != -1) {
        switch (opt) {
            case 'l':
                list_attributes(argv[optind]);
                break;
            case 'r':
                if (argc - optind < 1) {
                    fprintf(stderr, "Error: Missing file path.\n");
                    display_usage();
                    exit(EXIT_FAILURE);
                }
                read_attribute(argv[optind], optarg);
                break;
            case 'w':
                if (argc - optind < 2) {
                    fprintf(stderr, "Error: -w option requires an attribute name and a value.\n");
                    display_usage();
                    exit(EXIT_FAILURE);
                }
                write_attribute(argv[optind + 1], optarg, argv[optind]);
                optind += 2;
                break;
            case 'd':
                if (argc - optind < 1) {
                    fprintf(stderr, "Error: Missing file path.\n");
                    display_usage();
                    exit(EXIT_FAILURE);
                }
                delete_attribute(argv[optind], optarg);
                break;
            case 'h':
                display_usage();
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Error: Unknown option.\n");
                display_usage();
                exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}
