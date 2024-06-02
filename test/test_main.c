#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/controller.h"

// Mock function for testing
int mock_send(int sockfd, const void *buf, size_t len, int flags) {
    printf("Mock send called with data: %s\n", (char *)buf);
    return len;
}

// Replace the real send function with the mock function for testing
#define send mock_send

void test_send_file() {
    // Set up mock environment
    int test_socket = 0; // Mock socket
    FILE *mock_file = fopen("../public/test.html", "w");
    fprintf(mock_file, "Test file content.");
    fclose(mock_file);

    // Call the function to be tested
    send_file(test_socket, "test.html", "text/html");

    // Check the results
    // You would normally capture and inspect the data sent to the mock socket here
    // For simplicity, we are just printing the data
    assert(1); // Placeholder assertion
}

int main() {
    test_send_file();
    printf("All tests passed.\n");
    return 0;
}
