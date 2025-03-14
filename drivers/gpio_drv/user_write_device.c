#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
    const char dev_path[] = "/dev/gpio_dev";
    int user_dev = open(dev_path, O_RDWR);
    if (user_dev == -1){
        printf("ERROR: Failed to open device- %s\n",dev_path);
        return -1;
    }
    printf("INFO: Device %s opened successful\n",dev_path);
    close(user_dev);
    return 0;
}