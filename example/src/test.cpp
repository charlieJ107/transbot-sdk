#include <termios.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    int serial_file_descriptor = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY);
    std::cout << serial_file_descriptor << std::endl;
    struct termios serial_port_settings;
    cfsetispeed(&serial_port_settings, B115200);
    cfsetospeed(&serial_port_settings, B115200);

    // Set data bits to 8
    serial_port_settings.c_cflag &= ~CSIZE;
    serial_port_settings.c_cflag |= CS8;
    // Allow receve and use local
    serial_port_settings.c_cflag |= (CLOCAL | CREAD);
    // Set parity (奇偶校验) to none
    serial_port_settings.c_cflag &= ~PARENB;
    // Set stop bits to 1
    serial_port_settings.c_cflag &= ~CSTOPB;

    // Set wait time to 40ms
    serial_port_settings.c_cc[VTIME] = 4;
    // Set minimum receive bytes to 4
    serial_port_settings.c_cc[VMIN] = 4;

    // Using raw mode
    serial_port_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    serial_port_settings.c_oflag &= ~OPOST;

    // Flush the input and output buffer
    tcflush(serial_file_descriptor, TCIFLUSH);

    // Set the new options for the port
    if (tcsetattr(serial_file_descriptor, TCSANOW, &serial_port_settings) != 0)
    {
        std::cout << "Set serial port settings failed." << std::endl;
        close(serial_file_descriptor);
        return -1;
    }
    std::cout << "Set serial port settings success." << std::endl;

    uint8_t buffer[1024];
    int read_bytes = read(serial_file_descriptor, buffer, 1024);
    std::cout << "Read " << read_bytes << " bytes." << std::endl;
    for (int i = 0; i < read_bytes; i++)
    {
        std::cout << "0x" << std::hex << (int)buffer[i] << " ";
    }
    std::cout << std::endl;
    close(serial_file_descriptor);
    return 0;
}