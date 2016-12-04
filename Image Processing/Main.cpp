#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

int main(int argc, char* argv[]){

		//the first three lines of each ppm file and to store string values for rgb
		string ppmFormat,numColours,blank="",red, blue, green;

		//vectors for RGB values of each pixel of every image
		vector<int> red_vector,green_vector,blue_vector;

		//ints for rgb value,height & width of images
		int red_Int, green_Int, blue_Int,height,width; //to store converted values (initially in string form)

        //argv 0 is the command as such skip it when reading files in ,have an array of ifstream for multiple files
        ifstream myFiles[argc-1];
        int j=1,i=0;//i for the new array and j to read argv as it will begin at 1
        while(i<argc-1)
        {
            myFiles[i].open(argv[j]);
            if(i==0 && myFiles[i]!=NULL)
            {
                myFiles[0]>> ppmFormat>>height >> width>> numColours;
            }
            //skip the ppformat,numOfcolours etc which is in the first line
            getline( myFiles[i],blank);
            i++;
            j++;
        }

		ofstream outfile("new_image.ppm"); //create new image file of ppm format
		outfile << ppmFormat << "\n" << height << " " << width << "\n" << numColours << endl; //write lines to the image

		int counter = 0;
		while(counter < height * width)
		{ // create while loop to go through each pixel
            for(int i=0; i<argc-1; i++)
            {
                //get the rgb value of each ppm image to get the median
                myFiles[i]>> red >> green >> blue;
                istringstream n1 (red);
                n1 >> red_Int;
                red_vector.push_back(red_Int);

                istringstream n2 (green);
                n2 >> green_Int;
                green_vector.push_back(green_Int);

                istringstream n3 (blue);
                n3 >> blue_Int;
                blue_vector.push_back(blue_Int);
            }
            //sort vectors
			sort(red_vector.begin(), red_vector.end());
			sort(green_vector.begin(), green_vector.end());
			sort(blue_vector.begin(), blue_vector.end());
            //output meadin pixel to the file
			outfile << red_vector[argc/2] << " " << green_vector[argc/2] << " " << blue_vector[argc/2] << endl; //write to new_img file
            //clear vectors
			red_vector.clear();
			green_vector.clear();
			blue_vector.clear();

			counter++;
		}
}

