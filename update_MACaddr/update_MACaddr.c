#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>

typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef signed int INT32;
typedef unsigned int UINT32;

INT8 ifconfig_ethx_down_API(const UINT8 *interface_name)
{
	INT32 sock_fd;
	struct ifreq ifr;
	INT32 selector;
	
	if (interface_name == NULL)
	{
		fprintf(stdout, "%s:%d: args invalid!", __FUNCTION__, __LINE__);
		return -1;
	}
	
	if (strncmp((char*)interface_name, (char*)"lo", 2) == 0)
	{
		fprintf(stdout, "%s:%d: You can't pull down interface lo!",  __FUNCTION__, __LINE__);
		return -2;
	}
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		fprintf(stdout, "%s:%d: socket failed!",  __FUNCTION__, __LINE__);  
        return -3;
	}
	
	sprintf(ifr.ifr_name, "%s", interface_name);
	if (ioctl(sock_fd, SIOCGIFFLAGS, &ifr) < 0)
	{
		fprintf(stdout, "%s:%d: ioctl failed 1!",  __FUNCTION__, __LINE__);  
        return -4;
	}
	
	selector = IFF_UP;
	ifr.ifr_flags &= ~selector;
	if (ioctl(sock_fd, SIOCSIFFLAGS, &ifr) < 0)
	{
		fprintf(stdout, "%s:%d: ioctl failed 2!", __FUNCTION__, __LINE__);
		return -5;
	}
	
	close(sock_fd);
	
	return 0;
}

INT8 ifconfig_ethx_up_API(const UINT8 *interface_name)
{
	INT32 sock_fd;
	struct ifreq ifr;
	INT32 selector;
	
	if (interface_name == NULL)
	{
		fprintf(stdout, "%s:%d: args invalid!", __FUNCTION__, __LINE__);
		return -1;
	}
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		fprintf(stdout, "%s:%d: socket failed!",  __FUNCTION__, __LINE__);  
        return -2;
	}
	
	sprintf(ifr.ifr_name, "%s", interface_name);
	if (ioctl(sock_fd, SIOCGIFFLAGS, &ifr) < 0)
	{
		fprintf(stdout, "%s:%d: ioctl failed 1!",  __FUNCTION__, __LINE__);  
        return -3;
	}
	
	selector = (IFF_UP | IFF_RUNNING);
	ifr.ifr_flags |= selector;
	if (ioctl(sock_fd, SIOCSIFFLAGS, &ifr) < 0)
	{
		fprintf(stdout, "%s:%d: ioctl failed 2!", __FUNCTION__, __LINE__);
		return -4;
	}
	
	close(sock_fd);
	
	return 0;
}

INT8 SetLocalMACAddr_API(const UINT8 *interface_name, const UINT8 *str_macaddr)
{
	INT32 ret;
	INT32 sock_fd;
	struct ifreq ifr;
	UINT32 mac2bit[6];
	
	if ((interface_name == NULL) || (str_macaddr == NULL))
	{
		fprintf(stdout, "%s:%d: args invalid!",  __FUNCTION__, __LINE__);  
        return -1;
	}
	
	sscanf((char*)str_macaddr, "%02X:%02X:%02X:%02X:%02X:%02X", 
	(UINT8 *)&mac2bit[0], (UINT8 *)&mac2bit[1],
	(UINT8 *)&mac2bit[2], (UINT8 *)&mac2bit[3],
	(UINT8 *)&mac2bit[4], (UINT8 *)&mac2bit[5]);
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		fprintf(stdout, "%s:%d: socket error!",  __FUNCTION__, __LINE__);
		return -2;
	}
	
	ret = ifconfig_ethx_down_API (interface_name);
	if (ret < 0)
	{
		fprintf(stdout, "%s:%d: close eth0 error",  __FUNCTION__, __LINE__);  
        return -3;
	}
	sleep(1);
	
	sprintf(ifr.ifr_ifrn.ifrn_name, "%s", interface_name);    
    ifr.ifr_ifru.ifru_hwaddr.sa_family = 1;    
    ifr.ifr_ifru.ifru_hwaddr.sa_data[0] = mac2bit[0];  
    ifr.ifr_ifru.ifru_hwaddr.sa_data[1] = mac2bit[1];  
    ifr.ifr_ifru.ifru_hwaddr.sa_data[2] = mac2bit[2];  
    ifr.ifr_ifru.ifru_hwaddr.sa_data[3] = mac2bit[3];  
    ifr.ifr_ifru.ifru_hwaddr.sa_data[4] = mac2bit[4];  
    ifr.ifr_ifru.ifru_hwaddr.sa_data[5] = mac2bit[5];
	
	ret = ioctl(sock_fd, SIOCSIFHWADDR, &ifr);
	if (ret != 0)
	{
		perror("set mac address erorr");  
        return -4;
	}
	
	close(sock_fd);
	
	ret = ifconfig_ethx_up_API(interface_name);  
    if(ret < 0)  
    {  
        fprintf(stdout, "%s:%d: open eth0 error!",  __FUNCTION__, __LINE__);  
        return -5;  
    }  
      
    sleep(2);
  
    return 0;
}

INT8 GetLocalMACAddr_API(const UINT8 *interface_name, UINT8 *str_macaddr)    
{    
    INT32       sock_fd;    
    struct ifreq ifr_mac;

    if(interface_name == NULL || str_macaddr == NULL)  
    {  
        fprintf(stdout, "%s:%d: args invalid!",  __FUNCTION__, __LINE__);  
        return -1;  
    }

    sock_fd = socket( AF_INET, SOCK_STREAM, 0 );    
    if( sock_fd == -1)    
    {    
        perror("create socket failed");    
        sprintf((char *)str_macaddr, "00:00:00:00:00:00");  
        return -2;
    }

    memset(&ifr_mac, 0, sizeof(ifr_mac));       
    sprintf(ifr_mac.ifr_name, "%s", interface_name);       
    
    if( (ioctl(sock_fd, SIOCGIFHWADDR, &ifr_mac)) < 0)
    {    
        perror("mac ioctl error");    
        sprintf((char *)str_macaddr, "00:00:00:00:00:00");  
        return -3;    
    }    
        
    close( sock_fd );    
      
    sprintf((char *)str_macaddr,"%02x:%02x:%02x:%02x:%02x:%02x",    
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],    
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],    
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],    
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],    
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],    
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);    
    
    printf("local mac:<%s> \n", str_macaddr);        
      
    return 0;  
}

int main(void)  
{  
    UINT8 str_macaddr[20];  
      
    memset(str_macaddr, 0, sizeof(str_macaddr));  
    GetLocalMACAddr_API("eth1", str_macaddr);  
    fprintf(stdout, "1 mac: %s\n", str_macaddr);  

    SetLocalMACAddr_API("eth1", "08:00:44:33:22:11");  

    memset(str_macaddr, 0, sizeof(str_macaddr));  
    GetLocalMACAddr_API("eth1", str_macaddr);  
    fprintf(stdout, "2 mac: %s\n", str_macaddr);  

    return 0;  
}
