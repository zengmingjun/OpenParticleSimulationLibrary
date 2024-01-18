#include <iostream>  
#include <memory> // for std::shared_ptr  
#include<graphics.h>
#include<chrono>
#include"comp.h"

using namespace sp;

#define TwoPi 6.28318530717


shared_ptr<double[]> attr;

unsigned sWit = 680;
unsigned sHit = 680;

void FUp(Point& selfFData, Point* GestFData,double DisPowTwo) {
	double Dis = sqrtf(DisPowTwo);
	if (Dis < 2)
	{
		//碰撞添加
		DisPowTwo = 1;
		Dis = 1;
	}
	float F = (float)Kmf * attr[selfFData.Numbering] * attr[GestFData->Numbering] / DisPowTwo;
	float dertacor[2];
	for (unsigned i = 0; i < 2; i++)
	{
		dertacor[i] = Distance(selfFData.Coor.Arr[i], GestFData->Coor.Arr[i]) / Dis;
	}
	for (unsigned i = 0; i < 2; i++)
	{
		selfFData.F.Arr[i] -= F * dertacor[i];
	}

}

void CoorUp(Point& self) {

	for (unsigned i = 0; i < 2; i++)
	{
		if (self.F.Arr[i]!=0)
		{
			double Tmpa;
			Tmpa = self.F.Arr[i] / attr[self.Numbering];
			self.V.Arr[i] += Tmpa * dt;
			self.Coor.Arr[i] += self.V.Arr[i] * dt;
		}
		else
		{
			self.Coor.Arr[i] += self.V.Arr[i] * dt;
		}
		self.F.Arr[i] = 0;
	}

	if (self.Coor.Arr[0] <=0|| self.Coor.Arr[0]>= sWit)
	{
		self.V.Arr[0] = -self.V.Arr[0];
	}
	if (self.Coor.Arr[1] <= 0 || self.Coor.Arr[1] >= sHit)
	{
		self.V.Arr[1] = -self.V.Arr[1];
	}
}

unsigned N = 100;
double UnitT = TwoPi/N;

void setcoor(Point& self) {
	self.Coor.Arr[1] = 300;
	self.Coor.Arr[0] = 300;
	//self.Coor.Arr[0] -= 10;
}


void main() {
	vector<long long>s = {1000,1000};
	vector<unsigned>pn = {30,30};
	unsigned eff =	100;
	CreateSpace(pn, s, eff);

	unsigned PotNum = 1;
	attr = make_shared<double[]>(PotNum);
	for (unsigned i = 0; i < PotNum; i++)
	{
		attr[i] = 5;
	}

	AddPointbat(PotNum,0);
	setCoor(&setcoor);
	AddInPart();
	setCoorUpFuc(&CoorUp);
	setFUpFuc(&FUp);


	initgraph(sWit, sHit);
	setorigin(0, sHit);
	setaspectratio(1, -1);
	setfillcolor(RGB(255, 0, 0));
	setlinecolor(RGB(0,200,0));
	setlinestyle(PS_DASH);
	while (true)
	{
		BeginBatchDraw();
		cleardevice();


		//auto start = chrono::high_resolution_clock::now();

		SpaceDataUpUesPotGrp();

		//auto end = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		//std::cout << "函数运行时长: " << duration.count() / 1000000 << " ms" << std::endl;
		

		for (unsigned i = 0; i < PotNum; i++)
		{
			solidcircle(PotGrp[i].Coor.Arr[0], PotGrp[i].Coor.Arr[1], 5);
			circle(PotGrp[i].Coor.Arr[0], PotGrp[i].Coor.Arr[1], eff);
		}
		
		EndBatchDraw();
	}

	DestroyedSpace();
}




