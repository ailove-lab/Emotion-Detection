#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>

#include<iostream>
#include<stdio.h>
#include<sstream>
#include<fstream>
#include<cmath>

using namespace std;
using namespace dlib;

string shapeFileName = "./data/shape_predictor_68_face_landmarks.dat";
shape_predictor sp;
int faceNumber = 0;

int detectFaceAndCrop(char *imageName);
int storeAttributesToCSV(int noOfFaces,int emotion);
double length(point a,point b);
double slope(point a,point b);
void removePhotos();

int detectFaceAndCrop(char *imageName)
{
	frontal_face_detector detector = get_frontal_face_detector();

	array2d<rgb_pixel> img;

	load_image(img,imageName);

	pyramid_up(img);

	std::vector<dlib::rectangle> faceRectangles = detector(img);

	std::vector<full_object_detection> facialFeatures;

	for (int j = 0; j < faceRectangles.size(); ++j)
	{
		full_object_detection feature = sp(img, faceRectangles[j]);
		facialFeatures.push_back(feature);
	}

	dlib::array< array2d<rgb_pixel> > faces;

	extract_image_chips(img, get_face_chip_details(facialFeatures,500), faces);

	for(int i = 0; i < faces.size();i++,faceNumber++)
	{
		stringstream s;
		s<<"face"<<(faceNumber)<<".jpg";
		save_jpeg(faces[i],s.str(),100);
	}

	return(faceRectangles.size());
}

int storeAttributesToCSV(int noOfFaces,int emotion)
{
	int i,j,k;
	frontal_face_detector detector = get_frontal_face_detector();

	ofstream outfile;
	ifstream infile("points.csv");
	stringstream s;

	outfile.open("points.csv",ios::app);

	if(!infile.good())
	{
		for(i = 0; i < 68;i++)
			for(j = 0; j < i;j++)
				if(i!=j)
					outfile<<"lena"<<i<<"b"<<j<<","<<"a"<<i<<"b"<<j<<",";
		outfile<<"emotion\n";
	}

	for(i = 0; i < faceNumber; i++)
	{
		array2d<rgb_pixel> img;
		s.str("");
		s<<"face"<<(i)<<".jpg";
		load_image(img,s.str());
		cout<<"writing image"<<(i+1)<<" to csv\n";

		std::vector<dlib::rectangle> faceRectangles = detector(img);
		if(faceRectangles.size() <= 0)
		{
			remove(s.str().c_str());
			continue;
		}
		std::vector<full_object_detection> facialFeatures;

		full_object_detection feature = sp(img, faceRectangles[0]);

		for(int j = 0; j < 68; j++)
			for(int k = 0; k < j; k++)
				outfile<<length(feature.part(j),feature.part(k))<<","<<slope(feature.part(j),feature.part(k))<<",";

		if(emotion == 0)
			outfile<<"neutral"<<"\n";
		else if(emotion == 1)
			outfile<<"happy"<<"\n";
		else if(emotion == 2)
			outfile<<"sad"<<"\n";
		else if(emotion == 3)
			outfile<<"surprise"<<"\n";
		remove(s.str().c_str());
	}
	outfile.close();
	infile.close();
	return i;
}


double length(point a,point b)
{
	int x1,y1,x2,y2;
	double dist;
	x1 = a.x();
	y1 = a.y();
	x2 = b.x();
	y2 = b.y();

	dist = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	dist = sqrt(dist);
	return dist;
}

double slope(point a,point b)
{
	int x1,y1,x2,y2;

	x1 = a.x();
	y1 = a.y();
	x2 = b.x();
	y2 = b.y();
	if((x1-x2) == 0)
		if((y1-y2) > 0)
			return (M_PI/2);
		else
			return (-M_PI/2);
	else
		return atan(double(y1-y2))/(x1-x2);
}


int main(int argc,char **argv)
{
	int noOfFaces = 0;
	if(argc < 3)
	{
		printf("\n\n!!Arguments not Given properly!!\n\n");
	}
	deserialize(shapeFileName) >> sp;
	cout<<"\n\nProgram Started\n\n";
	for(int i = 2;i < argc; i++)
	{
		noOfFaces += detectFaceAndCrop(argv[i]);
		cout<<"image "<<(i-1)<<"\t"<<argv[i]<<"\n";
	}
	storeAttributesToCSV(noOfFaces,(int)(argv[1][0]-'0'));
	return 0;
}
