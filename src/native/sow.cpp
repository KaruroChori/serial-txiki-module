
#ifndef __linux__
#error This module is only compatible with linux
#endif

#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <cstdint>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function

//I am going to use a less portable, but more flexible option.
//#include <termios.h> // Contains POSIX terminal control definitions

#include <asm/termbits.h> 
#include <sys/ioctl.h> // Used for TCGETS2/TCSETS2, which is required for custom baud rates

#include <sys/file.h>


enum class feature_t : uint8_t {DISABLED, ENABLED};
enum class stop_t : uint8_t  {ONE_BIT, TWO_BITS};
enum class bits_t  : uint8_t {FIVE, SIX, SEVEN, EIGHT};
enum class hardware_flow_t : uint8_t  {DISABLED, ENABLED};

struct cfg_serial {
    feature_t parity: 1 = feature_t::ENABLED;
    stop_t stop:1 = stop_t::ONE_BIT;
    bits_t bits:2 = bits_t::EIGHT;
    feature_t hardware_flow:1 = feature_t::DISABLED;

    uint8_t vtime = 0;
    uint8_t vmin = 0;

    uint32_t ispeed = 9600;
    uint32_t ospeed = 9600;
};


int configure(const char* path, const cfg_serial& cfg){
    int serial_port = open(path, O_RDWR);

    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    termios2 tty;
    if(ioctl(serial_port, TCGETS2, &tty) != 0) {
        printf("Error %i from ioctl: %s\n", errno, strerror(errno));
    }

    //Parity
    {
        tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (default)
        if(cfg.parity==feature_t::ENABLED)tty.c_cflag |= PARENB;  // Set parity bit, enabling parity
    }

    //Stop
    {
        tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (default)
        if(cfg.stop==stop_t::TWO_BITS)tty.c_cflag |= CSTOPB;  // Set stop field, two stop bits
    }

    //Bits
    {
        tty.c_cflag &= ~CSIZE; // Clear all the size bits
        if(cfg.bits==bits_t::FIVE)tty.c_cflag |= CS5;
        else if(cfg.bits==bits_t::SIX)tty.c_cflag |= CS6;
        else if(cfg.bits==bits_t::SEVEN)tty.c_cflag |= CS7;
        else if(cfg.bits==bits_t::EIGHT)tty.c_cflag |= CS8; //Default
    }

    //Hardware flow
    {
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
        if(cfg.hardware_flow==feature_t::ENABLED)tty.c_cflag |= CRTSCTS;  // Enable RTS/CTS hardware flow control

    }

    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    	
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = cfg.vtime;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = cfg.vmin;

    tty.c_cflag &= ~CBAUD;
    tty.c_cflag |= CBAUDEX;
    tty.c_ispeed = cfg.ispeed; 
    tty.c_ospeed = cfg.ospeed;

    if(ioctl(serial_port, TCSETS2, &tty) != 0) {
        printf("Error %i from ioctl: %s\n", errno, strerror(errno));
    }

    if(flock(serial_port, LOCK_EX | LOCK_NB) == -1) {
        throw std::runtime_error("Serial port with file descriptor " + 
            std::to_string(serial_port) + " is already locked by another process.");
    }

    return serial_port;
}
