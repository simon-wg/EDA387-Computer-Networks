/********************************************************* -- SOURCE -{{{1- */
/** Translate host name into IPv4
 *
 * Resolve IPv4 address for a given host name. The host name is specified as
 * the first command line argument to the program.
 *
 * Build program:
 *  $ g++ -Wall -g -o resolve <file>.cpp
 */
/******************************************************************* -}}}1- */

#include <cstddef>
#include <cstdlib>
#include <err.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <assert.h>
#include <limits.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
//--//////////////////////////////////////////////////////////////////////////
//--    local declarations          ///{{{1///////////////////////////////////

void print_usage(const char *aProgramName);

//--    local config                ///{{{1///////////////////////////////////

/* HOST_NAME_MAX may be missing, e.g. if you're running this on an MacOS X
 * machine. In that case, use MAXHOSTNAMELEN from <sys/param.h>. Otherwise
 * generate an compiler error.
 */
#if !defined(HOST_NAME_MAX)
#if defined(__APPLE__)
#include <sys/param.h>
#define HOST_NAME_MAX MAXHOSTNAMELEN
#else // !__APPLE__
#error "HOST_NAME_MAX undefined!"
#endif // ~ __APPLE__
#endif // ~ HOST_NAME_MAX

//--    main()                      ///{{{1///////////////////////////////////
int main(int aArgc, char *aArgv[]) {
    // Check$ if the user supplied a command line argument.
    if (aArgc != 2) {
        print_usage(aArgv[0]);
        return 1;
    }

    // The (only) argument is the remote host that we should resolve.
    const char *remoteHostName = aArgv[1];

    // Get the local host's name (i.e. the machine that the program is
    // currently running on).
    const size_t kHostNameMaxLength = HOST_NAME_MAX + 1;
    char localHostName[kHostNameMaxLength];

    if (-1 == gethostname(localHostName, kHostNameMaxLength))
        err(EXIT_FAILURE, "gethostname");

    // Print the initial message
    printf("Resolving `%s' from `%s':\n", remoteHostName, localHostName);

    // TODO : add your code here
    // Create a hints struct to pass to getaddrinfo()
    struct addrinfo hints;
    struct addrinfo *results;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;
    hints.ai_addr = NULL;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;
    int addr_status = getaddrinfo(remoteHostName, NULL, &hints, &results);
    if (0 != addr_status) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addr_status));
        exit(EXIT_FAILURE);
    };

    sockaddr *sockAddr = results->ai_addr;
    assert(sockAddr->sa_family == hints.ai_family);
    sockaddr_in *inAddr = (sockaddr_in *)sockAddr;

    uint32_t ipNumber = inAddr->sin_addr.s_addr;
    char ipv4addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipNumber, ipv4addr, INET_ADDRSTRLEN);

    printf("%s has address %s\n", remoteHostName, ipv4addr);
    // getaddrinfo() allocates a addrinfo struct which needs to be freed
    freeaddrinfo(results);
    // Ok, we're done. Return success.
    return 0;
}

//--    print_usage()               ///{{{1///////////////////////////////////
void print_usage(const char *aProgramName) {
    fprintf(stderr, "Usage: %s <hostname>\n", aProgramName);
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab:
