#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <iomanip>
FILE *pf;

using namespace cv;
using namespace std;
#include "robokon2.h"



int main( int argc, char** argv )
{
  //yokozako=2;
  VideoCapture		front_cam(0);
  VideoCapture		under_cam(1);
  int			sock;
  struct sockaddr_in	addr;

  sock		       = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family      = AF_INET;
  addr.sin_port	       = htons(12346);
  addr.sin_addr.s_addr = inet_addr("192.168.160.1");
      
  pf = fopen("kuroshio_log.txt", "w");
  int cam = 1; 
    
  //sendto(sock, "L,3,3", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
  //waitKey(3000);
  
  for(int pool =0;;pool++) {
  
      index_img++;
      Mat	color_img_f;
      Mat	color_img_u;
      front_cam >> color_img_f;
      under_cam >> color_img_u;
      if(cam==1){
	char img_name[1024];
	sprintf(img_name, "fimage%05d.png",pool );
	imwrite (img_name,color_img_f);
	sprintf(img_name, "uimage%05d.png",pool );
	imwrite(img_name,color_img_u);
      }
      imshow("front", color_img_f);
      //imshow("under", color_img_u);
      if(bimwrite){
	//imwrite("fimg.png", color_img_f);
	//imwrite("uimg.png", color_img_u);
      }
      switch(yokozako){
      case 0:
	detect_buoy(color_img_f, sock, addr);
	//imshow("disp", color_img_u);
	detect_line(color_img_u, sock, addr);
	break;
      case 1:
	detect_buoy(color_img_f, sock, addr);
	break;
      case 2:
	detect_position(color_img_u,sock,addr);
	break;
      }
      
      //runding
      int	key = waitKey(30);
      if('q' == key){
	break;
      }
      bimwrite	 = false;
      if('w' == key){
	bimwrite = true;
      }

  }

//sendto(sock,"B,0,0",15,0,(struct sockaddr *)&addr, sizeof(addr));
  sendto(sock,"L,0,0",15,0,(struct sockaddr *)&addr, sizeof(addr));
  close(sock);
  fclose(pf);
  return 0;
}
