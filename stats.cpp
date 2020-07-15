
#include "stats.h"
#include <iostream>

stats::stats(PNG & im){
	int w = im.width();
	int h = im.height();

	sumRed = vector< vector< long >> (w, vector<long>(h)); 
    sumGreen = vector< vector< long >> (w, vector<long>(h)) ;
    sumBlue = vector< vector< long >> (w, vector<long>(h));
	sumsqRed = vector< vector< long >> (w, vector<long>(h));
	sumsqGreen = vector< vector< long >> (w, vector<long>(h));
    sumsqBlue = vector< vector< long >> (w, vector<long>(h));

long selfR;
long selfG;
long selfB;

long selfRsq;
long selfGsq;
long selfBsq;


for(int i = 0 ; i < im.width(); i++){
	for (int j = 0 ; j< im.height(); j++){

		selfR = im.getPixel(i,j)->r;
		selfG = im.getPixel(i,j)->g;
		selfB = im.getPixel(i,j)->b;

		selfRsq = (im.getPixel(i,j)->r * im.getPixel(i,j)->r);
		selfGsq = (im.getPixel(i,j)->g * im.getPixel(i,j)->g);
		selfBsq = (im.getPixel(i,j)->b * im.getPixel(i,j)->b);

		if (j != 0){
			selfR = selfR + sumRed[i][j-1];
			selfG = selfG + sumGreen[i][j-1];
			selfB = selfB + sumBlue[i][j-1];

			selfRsq = selfRsq + sumsqRed[i][j-1];
			selfGsq = selfGsq + sumsqGreen[i][j-1];
			selfBsq = selfBsq + sumsqBlue[i][j-1];
		}
		for (int k = 0; k < i ; k++){
			selfR = selfR+ im.getPixel(k,j)->r;
			selfG = selfG+ im.getPixel(k,j)->g;
			selfB = selfB + im.getPixel(k,j)->b;

			selfRsq = selfRsq + (im.getPixel(k,j)->r * im.getPixel(k,j)->r) ;
			selfGsq = selfGsq + (im.getPixel(k,j)->g * im.getPixel(k,j)->g) ;
			selfBsq = selfBsq + (im.getPixel(k,j)->b * im.getPixel(k,j)->b) ;
			
		}
		sumRed[i][j] = selfR ;
		sumGreen[i][j] = selfG ;
		sumBlue[i][j] = selfB ;

		sumsqRed[i][j] = selfRsq ;
		sumsqGreen[i][j] = selfGsq ;
		sumsqBlue[i][j] = selfBsq ;
		
	}

}
// YOUR CODE HERE!!
}


long stats::getVal(pair<int,int> p,vector<vector<long>> &l){
    if(p.first >= 0 && p.second >= 0){
        return l[p.first][p.second];
    }
    return 0;
}

long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
 	long sum = 0;
	pair<int,int> lowPoint = make_pair(ul.first - 1, lr.second);
    pair<int,int> upPoint = make_pair(lr.first, ul.second - 1);
    pair<int,int> comPoint = make_pair(ul.first - 1, ul.second - 1); 
	// cout <<"lr.first is " << lr.first<<endl;
	// cout << "width is "<< sumRed.size()<<endl;
	// cout << "height is "<< sumRed[1].size()<<endl;
	// cout <<"lr.second is " << lr.second<<endl;
 	 	if (channel ==  'r'){
		sum = sumRed[lr.first][lr.second] -
                getVal(lowPoint,sumRed) 
                - getVal(upPoint,sumRed) 
                + getVal(comPoint,sumRed);

 		}else if (channel == 'g'){
			 sum = sumGreen[lr.first][lr.second] -
                getVal(lowPoint,sumGreen) 
                - getVal(upPoint,sumGreen) 
                + getVal(comPoint,sumGreen);

 	}else if (channel == 'b'){

      sum = sumBlue[lr.first][lr.second] -
                getVal(lowPoint,sumBlue) 
                - getVal(upPoint,sumBlue) 
                + getVal(comPoint,sumBlue);

	 }
// YOUR CODE HERE!!

return sum;

}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
long sum = 0;
	 
	pair<int,int> lowPoint = make_pair(ul.first - 1, lr.second);
    pair<int,int> upPoint = make_pair(lr.first, ul.second - 1);
    pair<int,int> comPoint = make_pair(ul.first - 1, ul.second - 1); 

 	 	if (channel ==  'r'){
		sum = sumsqRed[lr.first][lr.second] -
                getVal(lowPoint,sumsqRed) 
                - getVal(upPoint,sumsqRed) 
                + getVal(comPoint,sumsqRed);

 		}else if (channel == 'g'){
			 sum = sumsqGreen[lr.first][lr.second] -
                getVal(lowPoint,sumsqGreen) 
                - getVal(upPoint,sumsqGreen) 
                + getVal(comPoint,sumsqGreen);

 	}else if (channel == 'b'){

      sum = sumsqBlue[lr.first][lr.second] -
                getVal(lowPoint,sumsqBlue) 
                - getVal(upPoint,sumsqBlue) 
                + getVal(comPoint,sumsqBlue);

	 }
// YOUR CODE HERE!!

return sum;


}


long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
     int ulx = ul.first;
	 int uly = ul.second;
	 int lrx = lr.first;
	 int lry = lr.second;

	 return ((lry-uly+1) * (lrx - ulx+1));
// YOUR CODE HERE!!

}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
long stats::getScore(pair<int,int> ul, pair<int,int> lr){
long area = rectArea(ul,lr);
long red = (getSum('r',ul,lr) * getSum('r',ul,lr)) / area;
long green = (getSum('g',ul,lr) * getSum('g',ul,lr))/area;
long blue = (getSum('b',ul,lr) * getSum('b',ul,lr))/area ;
long score = getSumSq('r',ul,lr) -  red + getSumSq('g',ul,lr) - green + getSumSq('b',ul,lr) - blue;
return score;

// YOUR CODE HERE!!

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
long area = rectArea(ul,lr);
long redAvg = getSum('r' , ul , lr)  / area;
long greenAvg = getSum('g' , ul , lr)  / area;
long blueAvg = getSum('b' , ul , lr)  / area;
return RGBAPixel(redAvg,greenAvg,blueAvg);


// YOUR CODE HERE!!

}
