#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dfs_poll.h>

#define SERIAL_DEVICE_PATH "/dev/uart1"

int uart_posix_sample(void)
{
    int fd;
    int count = 0; /* Timeout times */
    char uart_write_buf[] = "Hello RT-Thread \n";
    char uart_read_buf[50];
    struct pollfd fds[1];
    
    /* Open the device by non-blocking mode */
    fd = open(SERIAL_DEVICE_PATH, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        rt_kprintf("open device failed\n");
        return -1;
    }

    /* Add the fd to monitor */
    fds[0].fd = fd;
    fds[0].events = POLLIN;

    rt_kprintf("Please turn on the serial device [uart1], the serial device starts sending data...\n");
    write(fds[0].fd, uart_write_buf, rt_strlen(uart_write_buf));
    do
    {
        /* Monitor received data, timeout 1S */
        poll(fds, 1, 1000);
        if (fds[0].revents & POLLIN)
        {
            int len = read(fds[0].fd, uart_read_buf, sizeof(uart_read_buf));
            uart_read_buf[len] = '\0';
            rt_kprintf("The data received is %s\n", uart_read_buf);
            count = 0;
        }
        else
        {
            count++;
        }
    }
    while(count < 5);  /* Timeout 5 times */

    close(fd);

    rt_kprintf("The program exits\n");

    return 0;
}
MSH_CMD_EXPORT(uart_posix_sample, uart_posix_sample);
