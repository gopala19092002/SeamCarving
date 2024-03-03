#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat computenergy(Mat& image) // compute energy of the image
{
    Mat map(image.rows, image.cols, CV_32S);
    Vec3b up, down, left, right;
    float Ri,Bi,Gi,Rj,Bj,Gj;
    for (int i=0;i<image.rows;i++) {
        for (int j=0;j<image.cols;j++) {
            left=image.at<Vec3b>(max(i-1,0),j);
            right=image.at<Vec3b>(min(i+1,image.rows-1),j);
            up=image.at<Vec3b>(i,max(j-1,0));
            down=image.at<Vec3b>(i, min(j+1,image.cols-1));
            Ri =(right[0]-left[0])*(right[0]-left[0]);
            Gi =(right[1]-left[1])*(right[1]-left[1]);
            Bi =(right[2]-left[2])*(right[2]-left[2]);
            Rj =(down[0]-up[0])*(down[0]-up[0]);
            Gj =(down[1]-up[1])*(down[1]-up[1]);
            Bj =(down[2]-up[2])*(down[2]-up[2]);
            map.at<int>(i,j)=sqrt(Ri+Gi+Bi+Rj+Gj+Bj);    
        }
    }
    return map;// return energy
}
int** c2d(int rows,int cols) {
    int** arr=new int*[rows];
    for (int i=0;i<rows;++i) {
        arr[i]=new int[cols];
    }
    return arr;
}
void seamcarving(Mat& map,int seampath[]) // seam carving 
{
    int rows,cols,col;
    int min= 0;
    int ind,prev;
    rows=map.rows;
    cols=map.cols;
    int** dp=c2d(rows,cols);// initalize a dp table
    int** path=c2d(rows,cols);
    for (int j=0;j<cols;j++) 
    {
        dp[0][j]=map.at<int>(0, j);// storing first row energy in the dp table
    }
    for (int i=1;i<rows;i++) 
    {
        for (int j=0;j<cols;j++) 
        {   ind=j;
            prev=dp[i-1][j];// initializing prev value for the each pixel
             if (j<cols-1&& dp[i-1][j+1]<prev) 
             {
                prev=dp[i-1][j+1];
                ind= j+1;
             }
            if (j>0&& dp[i-1][j-1]<prev) 
            {
                prev=dp[i-1][j-1];
                ind= j-1;
            }
            path[i][j]=ind;// stroring column values for the path to backtrack
            dp[i][j]=map.at<int>(i,j)+prev;  // cumultaive enrgy sum   
        }
    }
    for (int j=1;j<cols;j++) // finding the column with the minimum cumulative energy
    {
        if (dp[rows-1][j]<dp[rows-1][min]) 
        {
            min=j;
        }
    }
    col = min;
    for (int i=rows-1;i>=0;i--) // store the path as in the form of column from each 2d vector
    {
        seampath[i]=col;
        col=path[i][col];
    }
}
void removeSeam(Mat& image,Mat& map,int seampath[]) {
    int rows,cols,seamCol;
    rows=image.rows;
    cols=image.cols;
    Mat newimage(rows,cols-1,CV_8UC3);
    for (int i=0;i<rows;i++) 
    {
        seamCol=seampath[i];
        for (int j=0;j<seamCol;j++) 
        {
            newimage.at<Vec3b>(i,j)=image.at<Vec3b>(i,j);
        }
        for (int j=seamCol+1;j<cols;j++)
        {
            newimage.at<Vec3b>(i,j-1)=image.at<Vec3b>(i,j);
        }
    }
   map=computenergy(newimage);
    newimage.copyTo(image);
}


int main(int argc,char* argv[]) 
{ 
    if(argc!=2)
    {
        cout<<"Arguments are not passed correctly";
        return 1;
    }
    int height;
    int width;
    Mat image, gimage;
    int energy;
    image=imread(argv[1]);
     if (image.empty()) {
        cout<<"Failed to load the image"<<endl;
        return 1;
    }
    Mat imagemap(image.rows, image.cols, CV_32S);
    Mat energyMap=computenergy(image);
     //cout<<image.rows<<"  "<<image.cols;
    cout <<"[Height < "<<image.rows<<"] "<<"[Width < "<<image.cols<<"]"<<endl;
    cin>>height>>width;
    int rh=image.rows-height;
    int rw=image.cols-width;
    //cout<<reductionWidth<<"  "<<reductionheight;
    for (int i=0;i<rw;++i) // reduction in width for the reduction on width
    {
        int seampath[energyMap.rows];
        seamcarving(energyMap,seampath);
        removeSeam(image,energyMap,seampath);
    }
    Mat finalImage=image.t();// Transpose of the image for the reduction in height;
    energyMap =computenergy(finalImage);
    for (int i=0;i<rh;++i) 
    {
        int seampath[energyMap.rows];
        seamcarving(energyMap,seampath);
        removeSeam(finalImage,energyMap,seampath);
    }
    image = finalImage.t();
    imwrite("output.jpg", image);
    return 0;
}
