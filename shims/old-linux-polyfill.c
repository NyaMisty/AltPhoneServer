#include <sys/random.h>
#include <unistd.h>
#include <fcntl.h>

ssize_t getrandom(void *buf, size_t buflen, unsigned int flags) {
    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData < 0)
    {
        // something went wrong
        return -1;
    }
    else
    {
        ssize_t result = read(randomData, buf, buflen);
        if (result < 0)
        {
            // something went wrong
            return -1;
        }
        return result;
    }
}