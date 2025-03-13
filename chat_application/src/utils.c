#include <stdio.h>
#include <string.h>      // @include Header for string operations (e.g., strcpy)
#include <ifaddrs.h>     // @include Header for network interface information (getifaddrs)
#include <arpa/inet.h>   // @include Header for IP address conversion (inet_ntop)
#include <netinet/in.h>  // @include Header for struct sockaddr_in and AF_INET
#include "../inc/utils.h" // @include Custom header for utility function declarations

// @function draw_border
// @brief Draws a decorative border in the console
// @details Prints a cyan-colored line of asterisks to visually separate output sections
void draw_border() {
    printf(ANSI_COLOR_CYAN "****************************\n" ANSI_COLOR_RESET);  // @print Prints a cyan border line
}

// @function show_title
// @brief Displays the title of the chat application
// @details Prints a formatted title between two borders for visual emphasis
void show_title() {
    draw_border();  // @call Draws the top border
    printf(ANSI_COLOR_YELLOW "       CHAT APPLICATION     \n" ANSI_COLOR_RESET);  // @print Prints the title in yellow
    draw_border();  // @call Draws the bottom border
}

// @function get_my_ip
// @brief Retrieves the local machine's IP address
// @param ip_buffer Buffer to store the retrieved IP address as a string
// @details Queries network interfaces to find the first IPv4 address and stores it in the buffer
void get_my_ip(char *ip_buffer) {
    struct ifaddrs *ifaddr, *ifa;  // @var Pointers to network interface addresses and iterator
    if (getifaddrs(&ifaddr) == -1) {  // @call Retrieves network interfaces; checks for failure
        strcpy(ip_buffer, "Unable to get IP");  // @call Copies error message to buffer if retrieval fails
        return;  // @return Exits the function on failure
    }
    
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {  // @loop Iterates through network interfaces
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {  // @if Checks for valid IPv4 address
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;  // @cast Casts to IPv4 address structure
            inet_ntop(AF_INET, &addr->sin_addr, ip_buffer, INET_ADDRSTRLEN);  // @call Converts binary IP to string
            break;  // @break Exits loop after finding the first IPv4 address
        }
    }
    freeifaddrs(ifaddr);  // @call Frees the allocated interface addresses
}