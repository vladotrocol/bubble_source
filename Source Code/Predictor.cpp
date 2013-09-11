#include "Predictor.h"

Predictor::Predictor()
{
	init(false);
};

Point3f divideBy(Point3f a, float b)
{
	return Point3f(a.x/b, a.y/b, a.z/b);
};

Point3f multiplyBy(Point3f a, float b)
{
	return Point3f(a.x*b, a.y*b, a.z*b);
};

void Predictor::init(bool display)
{
	if(display){
		img = new Mat(800,1200, CV_8UC3);
		namedWindow("Prediction Filter");
		cvMoveWindow( "Prediction Filter", 0, 0 );
		cvResizeWindow(  "Prediction Filter", 1200, 800);
	}

	status = NO_POINTS;
	cST =clock();
	v_c = 0;
	a_c = 0;
};

void Predictor::correct(Point3f newInput)
{

	switch(status)
	{
		case NO_POINTS:
			//printf("NONE\n");
			status = ONE_POINT;
			//sstore point and time at p2
			P2 = newInput;
			cNT = clock();
			t2 = cNT-cST;
            break;

        case ONE_POINT:
			//printf("ONE\n");
			status = TWO_POINTS;
			//Store point and its time
			P1 = newInput;
			cNT = clock();
			t1 = cNT-cST;
			//compute velocity at t1
			CT = ((float)t1-t2)/CLOCKS_PER_SEC;
			v1 = divideBy((P1-P2), CT);
            break;

		case TWO_POINTS:
			//printf("TWO\n");
			status = READY;
			//store point and time
			P0 = newInput;
			cNT = clock();
			t0 = cNT-cST;
			//Compute velocity at t0
			CT = ((float)t0-t1)/CLOCKS_PER_SEC;
			v0 = divideBy((P0-P1), CT);
			//Compute current acceleration
			a0 = divideBy((v0-v1), CT);

			break;

		case READY:
			//printf("READY\n");
			//A.Shift P1 to P2
			P2 = P1;
			t2 = t1;
			//Shift P0 to P1
			P1=P0;
			t1=t0;
			//Store new point
			P0 = newInput;
			cNT = clock();
			t0 = cNT-cST;
			//B.Compute velocities
			//Shift previous velocity
			v1=v0;
			//Store new velocity
			CT = ((float)t0-t1)/CLOCKS_PER_SEC;
			v0=divideBy(P0-P1,CT);
			//Adaptive part of it (v_c==1 reactive, v_c<<1 noise reduction)
			v0 =multiplyBy(v1, v_c)+multiplyBy(v0, (1-v_c));
			//Acceleration. Adaptive part (a_c==1 reactive, blabla)
			a0 =multiplyBy(divideBy(v0-v1,CT), (1-a_c))+multiplyBy(a0, a_c);
            break;

         default:;
	}
};

Point3f Predictor::predict()
{
	pNT = clock();
	switch(status)
	{
        case NO_POINTS:
			return Point3f(0.0f, 0.0f, 0.0f);
            break;
        case ONE_POINT:
			return P2;
            break;
		case TWO_POINTS:
			PT = (float)(pNT-t1)/CLOCKS_PER_SEC;
			return P1+PT*v1;
            break;
		case READY:
			PT = (float)(pNT-t0)/CLOCKS_PER_SEC;
			return P0 + v0*PT + a0*PT*PT*0.5f;
            break;
         default:
			 return Point3f(0.0f, 0.0f, 0.0f);
     }
};

void Predictor::test(){
	float x,y;
	int c, s, s1, s2;
	s = clock();
	s1 = s;
	s2 = s;
	float _t;
	init(true);
	while(1){
		c = clock();
		_t =  (float)(c - s)/CLOCKS_PER_SEC;
		if(((float)(c-s2)/CLOCKS_PER_SEC)>0.1){
			Point3f p = predict();
			circle( *img, Point2f(p.x, p.y),1,Scalar( 0, 0, 255),-1,8 );
			//printf("%f %f %f %d\n", p.x, p.y, p.z, status);
			s2 = c;
		}
		if(((float)(c-s1)/CLOCKS_PER_SEC)>0.612){
			x =_t*50;
			y =  _t*_t+400;
			correct(Point3f(x, y, 0.0f));
			circle(*img,Point2f(x,y),1,Scalar( 255, 0, 0 ),-1,8 );
			s1 = c;
		}
		imshow("Prediction Filter", *img);
		waitKey(1);
	}
};