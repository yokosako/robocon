bool	bimwrite = false;

int yokozako = 0;

//ofstream log;
double get_ratio_enbl_pix(Mat &img)
{
  int		num_enbl = 0;
  for(int i = 0; i < img.rows; ++i){
    uchar *	pimg	 = img.ptr<uchar>(i);
    for(int j= 0; j< img.cols; ++j){
      if(pimg[j] == 255){
	num_enbl++;
      }
    }
  }
  return num_enbl/(double)img.total();
}

void detect_position(Mat &color_img, int sock, sockaddr_in &addr){
  Mat nit_img;
  cvtColor(color_img, nit_img, CV_BGR2HSV);
  inRange(nit_img, Scalar(0, 0,0), Scalar(180, 255, 100), nit_img);
  //imshow("position",nit_img);
  cout << get_ratio_enbl_pix(nit_img) << endl;
  //for(int tea =0;;){
    
    if(get_ratio_enbl_pix(nit_img)>0.3){
      sendto(sock, "P", 10, 0, (struct sockaddr *)&addr, sizeof(addr));
      //cout << "position" << endl;
      //imshow("dai",color_img);
      //imshow("dainitika",nit_img);
      return;
    }
  //}
    
    imshow("dai",color_img);
    imshow("dainitika",nit_img);

}

void detect_buoy(Mat &color_img, int sock, sockaddr_in &addr){
  static int	t = 0;
  static int buoy_num =0;
  Mat		gray_img;
      Mat	nit_img;
      //cvtColor(color_img, gray_img, CV_BGR2GRAY);
      cvtColor(color_img, gray_img, CV_BGR2HSV);
      inRange(gray_img, Scalar(0, 0, 0), Scalar(78, 255, 255), nit_img);
      if(bimwrite){
	imwrite("bnit_img.png", nit_img);
      }
      //cout << get_ratio_enbl_pix(nit_img) << endl;
      if(get_ratio_enbl_pix(nit_img)>0.27){
	sendto(sock, "B,0,0", 10, 0, (struct sockaddr *)&addr, sizeof(addr));
	cout << "true" << endl;
	yokozako = 2;
	    return;
	}
      inRange(gray_img, Scalar(0, 0, 0), Scalar(78, 255, 255), gray_img);
      GaussianBlur(gray_img,gray_img,Size(9,9),2,2);
      Canny (gray_img , gray_img , 50,200,3);
      vector<Vec3f>	circles;
      HoughCircles( gray_img, circles, CV_HOUGH_GRADIENT,8, gray_img.rows, 400, 200 , 50 , 100);
      if (circles.size()       != 0){
	buoy_num++;
	    Point	center(cvRound(circles[0][0]), cvRound(circles[0][1]));
	  int		radius	= cvRound(circles[0][2]);
	  
	  circle( color_img, center, 3, Scalar(0,255,0), -1, 8, 0 );
	  circle( color_img, center, radius, Scalar(0,255,0), 3, 8, 0 );
	  if(bimwrite){
	    imwrite("bimg.png", color_img);
	  }  
	  
	  t		    = 0;
	    double	sax = center.x-320;
	    double	say = center.y - 240;
	char		buf[100];
	
	if(sax>0){
	  //sprintf(buf, "B,%f,%f",1,1.5);
	//cout << buf << endl;
	sendto(sock, "B,2.5,1", 10, 0, (struct sockaddr *)&addr, sizeof(addr));
        
	}
	if(sax<0){
	  //sprintf(buf, "B,%f,%f",1.5,1.0);
	  //cout << buf << endl;
	sendto(sock, "B,1,2.5", 10, 0, (struct sockaddr *)&addr, sizeof(addr));

	}
	if(sax == 0){

	  //sprintf(buf, "B,%f,%f",1.5,1.5);
	//cout << buf << endl;
	  sendto(sock, "B,1.5,1.5", 10, 0, (struct sockaddr *)&addr, sizeof(addr));
	}
      }
      if(circles.size()	== 0){
	t++;
	buoy_num=0;
      }
      if(t >= 10)
	{
	  yokozako = 0;
	  //char	buf[100];
	  //sprintf(buf, "Y");
	  //cout << buf << endl;
	  //sendto(sock, buf, 10, 0, (struct sockaddr *)&addr, sizeof(addr));
	}
      if(buoy_num>=3){
	yokozako = 1;
      }
      
      //imshow("ブイ検出nitika",nit_img);
      //imshow("g",gray_img);
      imshow( "ブイ検出", color_img );
}




int		index_img = 0;
int		i	  = 0;
double		kakudoY	  = 0;
double	        seppenY	  = 0;
double		seppen=0;
      double		seppen2=0;
      double		seppen3=0;
      double		kakudo=0;
void detect_line(Mat &color_img, int sock, sockaddr_in &addr){
  //cout << "i : " << i << endl;
char		img_name[1024];
  sprintf(img_name, "pool_20160810_%010d.png", index_img);
      //imwrite(img_name , color_img);
      Mat	gray_img;
      cvtColor(color_img, gray_img, CV_BGR2HSV);
      inRange(gray_img, Scalar(0, 0, 0), Scalar(180, 255, 100), gray_img);
      if(bimwrite){
	imwrite("lnit_img.png", gray_img);
      }
      vector<Vec4i>	lines;
      HoughLinesP( gray_img, lines, 1, CV_PI/180, 80, 30, 10 );
      
      if(lines.size() >= 1){
	
	i=0;
	line( color_img, Point(lines[0][0], lines[0][1]),Point(lines[0][2], lines[0][3]), Scalar(255,0,0), 3, 8 );
	circle (color_img,Point(color_img.cols/2,color_img.rows/2),50,Scalar(0,0,0),2,8);
	circle (color_img,Point(color_img.cols/2,color_img.rows/2),100,Scalar(0,0,0),2,8);
	double	katamuki = (((double)-lines[0][3]+lines[0][1])/(lines[0][2]-lines[0][0]));
	line (color_img, Point(color_img.cols/2,0),Point(color_img.cols/2,color_img.rows),Scalar(0,0,0),2,8);
	line (color_img, Point(0,color_img.rows/2),Point(color_img.cols,color_img.rows/2),Scalar(0,0,0),2,8);
	if(bimwrite){
	  imwrite("limg.png", color_img);
	}
	seppen = color_img.rows+(-1)*(katamuki*lines[0][0]+lines[0][1]);
	
	seppen2	  = katamuki*color_img.cols+seppen;
	seppen3 = (-1)*((seppen2+seppen)/2-color_img.rows/2)/katamuki;
	kakudo	  = atan(katamuki)*(180/CV_PI);
	if(katamuki<0){
	  kakudo += 180;
	}
	
	int	val1 = (int)seppen3;
	int	fuck = 200;

	double	P = 0.03;
	double	I = 0.001;
	double	D = 0.0001;
	cout << seppenY << endl;
	cout << kakudoY << endl;
	if(kakudo >= 0 && kakudo < 90 && seppen3 <=fuck && seppen3 > -1*fuck){
	  int		kaku = 90-kakudo;
	  double	PID  = kaku*P+kaku*I+kaku*D;
	  char		buf[100];
	  sprintf(buf, "L,%f,%f",2+PID,2-PID);
	  sendto(sock, buf, 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >= 90 && kakudo <= 180 && seppen3 <=fuck && seppen3 > -1*fuck){
	  int		kaku = kakudo-90;
	  double	PID  = kaku*P+kaku*I+kaku*D;
	  char		buf[100];
	  sprintf(buf, "L,%f,%f",2-PID,2+PID);
	  sendto(sock, buf, 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >= 45 && kakudo < 80 && seppen3 <= -1*fuck){
	  sendto(sock, "L,2.5,2.5", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >= 0 && kakudo < 45 && seppen3 <= -1*fuck){
	  sendto(sock, "L,0,0", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >= 0 && kakudo < 80 && seppen3 > fuck){
	  sendto(sock, "L,4,-4", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >= 100 && kakudo <= 135 && seppen3 > fuck){
	  sendto(sock, "L,2.5,2.5", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >135 && kakudo <= 180 && seppen3 > fuck){
	  sendto(sock, "L,0,0", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >= 100 && kakudo <= 180 &&   seppen3 < -1*fuck){
	  sendto(sock, "L,-4,4", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >=80 && kakudo <90 &&   seppen3 < -1*fuck){
	  sendto(sock, "L,1.75,2.5", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >=80 && kakudo <90 &&   seppen3 >fuck){
	  sendto(sock, "L,2.5,1.75", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >=90 && kakudo <100 &&   seppen3 >fuck){
	  sendto(sock, "L,2.5,1.75", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	if(kakudo >=80 && kakudo <90 &&   seppen3 < -1*fuck){
	  sendto(sock, "L,1.75,2.5", 20, 0, (struct sockaddr *)&addr, sizeof(addr));
	  
	}
	kakudoY =	kakudo;
	seppenY =	seppen3;

      }
      else{
	i++;
	//cout<<"i"<<i<<endl;
    }
      
      if(i>5){
	//cout << "OK" << endl;
	//cout << seppenY << endl;
	//cout << kakudoY << endl;
	  if(kakudoY>=90 && kakudoY <= 180 && seppenY <0){
	    
	    sendto(sock,"L,-1,3.5",30,0,(struct sockaddr *)&addr, sizeof(addr));
	    cout << 1 << "," << 3 << endl;
	    imwrite("img0_3a.png", color_img);
	  }
	  if(kakudoY>=0 && kakudoY<90 && seppenY>0){
	    
	    sendto(sock,"L,3.5,-1",30,0,(struct sockaddr *)&addr, sizeof(addr));
	    cout << 3 << "," << 1 << endl;
	    imwrite("img3_0a.png", color_img);
	  }
	  if(kakudoY>=0 && kakudoY<90 && seppenY <0){
	    sendto(sock,"L,-1,3.5",30,0,(struct sockaddr *)&addr, sizeof(addr));
	    cout << 1 << "," << 3 << endl;
	    imwrite("img0_3b.png", color_img);
	  }
	  if(kakudoY>=90 && kakudoY <= 180 && seppenY >0){
	    sendto(sock,"L,3.5,-1",30,0,(struct sockaddr *)&addr, sizeof(addr));
	    cout << 3 << "," << 1 << endl;
	    imwrite("img3_0b.png", color_img);
	  }
	 
	}
     
      
      //imwrite(img_name , color_img);
      imshow( "ライン検出", color_img );
      //imshow("ライン検出nitika",gray_img);
      fprintf(pf, "%03f, %03f\n", kakudoY, seppenY);
}
