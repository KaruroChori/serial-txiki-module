#include <iostream>
#include <unistd.h>
#include <sow.h>
using namespace std;

int main(){
    cfg_serial cfg;
    int fd = configure("/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0",cfg);
    cout<<"Hello world!";
    close(fd);
    return 0;
}