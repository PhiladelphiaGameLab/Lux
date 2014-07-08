/*
* This code was orginally written and developed by the Lux Backend Team
* at Philadelphia Game Lab:
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*
* - Jeff Ramspacher
*/

#include "HMBL.h"
#include <stdio.h>
#include <iostream>
#include <vector>

//-------------------------STATIC FUNCTIONS--------------------------------
int HMBL_HELPER::getBucket(int xloc, int yloc, int mapx, int mapy, int columns, int rows){
	int bucket_dimensionX = mapx / columns;
	int bucket_dimensionY = mapy / rows;
	int xBuckets = xloc / bucket_dimensionX;
	int yBuckets = yloc / bucket_dimensionY;
	int bucketNum;
	if (yBuckets > 0)
		bucketNum = ((xBuckets * rows) - (rows % yBuckets));// - 1 is for indexing the array of buckets
	else
		bucketNum = (xBuckets * rows - 0);
	return bucketNum;
}

std::vector<int> HMBL_HELPER::surroundings(int xloc, int yloc, int rad, int mapx, int mapy, int columns, int rows){
	std::vector<int> surroundings;

	int currentBucket = getBucket(xloc, yloc, mapx, mapy, columns, rows);
	std::cout << " Current Bucket: " << currentBucket << std::endl;

	int x = currentBucket / rows;
	int y = currentBucket % rows;
	int xval;
	int yval;

	//Algorithm for finding the surrounding relative buckets while taking the edges of the map into account
	//It converts the buckets into a basic coordinate system
	for (int i = rad; i >= 0; i--){
		for (int j = rad; j >= 0; j--){

			xval = x - i;
			yval = y - j;

			if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
				surroundings.push_back(backToBuck(xval, yval, rows));
			}
		}
		for (int j = 1; j <= rad; j++){

			xval = x - i;
			yval = y + j;

			if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
				surroundings.push_back(backToBuck(xval, yval, rows));
			}
		}
	}
	for (int i = 1; i <= rad; i++){
		for (int j = rad; j >= 0; j--){

			xval = x + i;
			yval = y - j;

			if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
				surroundings.push_back(backToBuck(xval, yval, rows));
			}
		}
		for (int j = 1; j <= rad; j++){

			xval = x + i;
			yval = y + j;

			if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
				surroundings.push_back(backToBuck(xval, yval, rows));
			}
		}
	}

	int thresh = surroundings.size(); //The following is for testing only
	std::cout << " Size: " << surroundings.size() << std::endl;
	for (int k = 0; k < thresh; k++){
		std::cout << " [" << k << "] = " << surroundings[k] << std::endl;
	}
	std::cout<<"Returned Successfully from the  surroundings"<<std::endl;
	return surroundings;
}

//Meant for use in the Surroudings function to re-convert coordinates to buckets
int HMBL_HELPER::backToBuck(int x, int y, int ys){
	int cB = (x*ys) + y;
	return cB;
}

//int main(int argc, const char* argv[]) //Only used for testing
//{
	/*printf("Begin Testing of HMBL\n\nRound One:\n");
	HMBL* testHMBL = new HMBL(200, 200, 20, 20);
	printf(" Dimensions: %d, %d\n", testHMBL->mapheight, testHMBL->mapwidth);
	printf(" Bucket x = %d, y = %d\n\n", testHMBL->columns, testHMBL->rows);
	printf(" Where are you at 72,36? Bucket #%d\n", testHMBL->findBucket(72, 36));
	printf(" How about 200,200? Bucket #%d\n", testHMBL->findBucket(200, 200));
	printf("\nRound Two:\n");
	testHMBL->update(7200, 213, 72, 36);
	testHMBL->update(7300, 12, 56, 150);
	testHMBL->update(7400, 54, 56, 150);
	testHMBL->update(8800, 72, 56, 150);
	testHMBL->update(7400, 54, 100, 100);
	testHMBL->revealHMBL();
	printf("\nRound Three:\n");
	testHMBL->surroundings(4, 4, 2, 100, 100, 20, 20);

	getchar();*/
//}
