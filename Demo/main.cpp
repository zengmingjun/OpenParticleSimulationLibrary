#include <iostream>  
#include <memory> // for std::shared_ptr  
#include<graphics.h>
#include<chrono>
#include<math.h>
#include"comp.h"

using namespace sp;

#define TwoPi 6.28318530717
#define KE 10


shared_ptr<double[]> attr;
shared_ptr<double[]> attrQ;

unsigned sWit = 680;
unsigned sHit = 680;

void FUp(Point& selfFData, Point* GestFData,double DisPowTwo) {
	double Dis = sqrtf(DisPowTwo);
	if (Dis < 5)
	{
		//碰撞添加
		DisPowTwo = 25;
		Dis = 5;
	}

	float dertacor[2];
	for (unsigned i = 0; i < 2; i++)
	{
		dertacor[i] = Distance(GestFData->Coor.Arr[i], selfFData.Coor.Arr[i]) / Dis;
	}

	float F = 0;
	/*F = (float)Kmf * attr[selfFData.Numbering] * attr[GestFData->Numbering] / DisPowTwo;
	for (unsigned i = 0; i < 2; i++)
	{
		selfFData.F.Arr[i] += F * dertacor[i];
	}*/

	F = (float)KE * attrQ[selfFData.Numbering] * attrQ[GestFData->Numbering] / DisPowTwo;
	for (unsigned i = 0; i < 2; i++)
	{
		selfFData.F.Arr[i] += -F * dertacor[i];
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



shared_ptr<double[]> BaGua(unsigned precision, unsigned R,bool zf) {
	//std::shared_ptr<int[]> array(new int[precision], [](int* ptr) { delete[] ptr; });  // 智能指针接管数组  
	//array = nullptr;
	std::shared_ptr<double[]> bgt(new double[precision], [](double* ptr) { delete[] ptr; });
	
	//bgt.reset();
	double TmpUnit = (double)R / precision;
	unsigned D = 2 * R;

	for (unsigned i = 0; i < precision; i++)
	{
		double tmpY = i*TmpUnit;
		double tmpYpow = tmpY * tmpY;
		if (zf)
		{
			bgt[i] = sqrtf(D * tmpY - tmpYpow) - sqrtf(R * tmpY - tmpYpow);
		}
		else
		{
			bgt[i] = sqrtf(D * tmpY - tmpYpow) + sqrtf(R * tmpY - tmpYpow);
		}
	}
	//bgt.reset();
	//bgt = nullptr;
	return bgt;
}




void setcoor(Point& self) {
	
	self.Coor.Arr[1] = 300 + self.Numbering*10;
	self.Coor.Arr[0] = 300;
	//self.Coor.Arr[0] -= 10;
}


class But
{
public:
	bool Sta;
	unsigned x;
	unsigned y;
	int wit;
	int hit;
	unsigned SieX;
	unsigned SieY;
	COLORREF Filcolor;
	COLORREF silcolor;
	But();
	But(unsigned Xtop, unsigned Ytop, int inwit, int inhit, COLORREF Filcolr, unsigned ScreenX, unsigned ScreenY);
public:
	void ButInit(unsigned Xtop, unsigned Ytop, int inwit, int inhit, COLORREF Filcolr, unsigned ScreenX, unsigned ScreenY);
	void ButDraw();
	bool ButDown(unsigned x, unsigned y);
private:

};

But::But()
{
	silcolor = RGB(255, 255, 255);
	Sta = false;
}

But::But(unsigned Xtop, unsigned Ytop, int inwit, int inhit, COLORREF Filcolr, unsigned ScreenX, unsigned ScreenY) {
	SieX = ScreenX;
	SieY = ScreenY;
	silcolor = RGB(255, 255, 255);
	Sta = false;
	x = Xtop;
	y = Ytop;
	wit = inwit;
	hit = inhit;
	Filcolor = Filcolr;
}

void But::ButInit(unsigned Xtop, unsigned Ytop, int inwit, int inhit, COLORREF Filcolr, unsigned ScreenX, unsigned ScreenY) {
	SieX = ScreenX;
	SieY = ScreenY;
	x = Xtop;
	y = Ytop;
	wit = inwit;
	hit = inhit;
	Filcolor = Filcolr;
}

bool But::ButDown(unsigned x, unsigned y) {
	if (x >= this->x && x <= this->x + wit)
	{
		unsigned they = SieY - y;

		if (they >= this->y - hit && they <= this->y)
		{
			return true;
		}


	}
	return false;
}

void But::ButDraw() {
	COLORREF oldfilco = getfillcolor();
	COLORREF oldsilco = getlinecolor();
	setfillcolor(Filcolor);
	setlinecolor(silcolor);
	fillrectangle(x, y, x + wit, y - hit);
	setfillcolor(oldfilco);
	setlinecolor(oldsilco);
	return;
}



void main() {

	

	vector<long long>s = {1000,1000};
	vector<unsigned>pn = {10,10};
	unsigned eff =	10;
	unsigned Qlive = 5;
	CreateSpace(pn, s, eff);

	unsigned PotR = 6;
	unsigned wcy = -4;
	//setCoor(&setcoor);

	unsigned bgR = 200;
	unsigned layep = 20;

	unsigned jianxi = bgR/ layep;
	shared_ptr<double[]> bgtwoz = BaGua(layep, bgR, true);
	shared_ptr<double[]> bgtwof = BaGua(layep, bgR, false);

	
	unsigned PotNum = 0;
	unsigned z = 0;
	unsigned f = 0;

	for (size_t i = 0; i < layep; i++)
	{
		z  += bgtwoz[i] / jianxi;
	}
	cout << "少质:" << z << endl;

	for (size_t i = 0; i < layep; i++)
	{
		f += bgtwof[i] / jianxi;
	}
	cout << "多质:" << f << endl;


	PotNum = (z + f) * 2;
	cout << "整体:" << PotNum << endl;

	attr = make_shared<double[]>(PotNum);
	attrQ = make_shared<double[]>(PotNum);

	AddPointbat(PotNum, 0);


	unsigned centerx = 340;
	unsigned centery = 200;

	//unsigned tmplaye = 0;
	unsigned tmpnumber = 0;
	//1
	for (unsigned i = 0; i < layep; i++)
	{
		unsigned tmpz = bgtwoz[i] / jianxi;
		int tmpd = jianxi * i - bgR / 2;
		if (tmpd<0)
		{
			tmpd = -tmpd;
		}
		unsigned tmpx = sqrt(pow(bgR / 2, 2) - tmpd * tmpd);
		for (unsigned l = 0; l < tmpz; l++)
		{

			PotGrp[tmpnumber].Coor.Arr[1] = jianxi * i + centery + 2*PotR + wcy;
			PotGrp[tmpnumber].Coor.Arr[0] = jianxi * l + tmpx+ centerx;

			attrQ[tmpnumber] = Qlive;

			tmpnumber++;
			
		}
	}


	for (unsigned i = 0; i < layep; i++)
	{
		unsigned tmpz = bgtwof[i] / jianxi;
		int tmpd = jianxi * i - bgR / 2;
		if (tmpd < 0)
		{
			tmpd = - tmpd;
		}
		unsigned tmpx = sqrt(pow(bgR / 2, 2) - tmpd * tmpd);
		for (unsigned l = 0; l < tmpz; l++)
		{

			PotGrp[tmpnumber].Coor.Arr[1] = -(int)jianxi * i + centery+2 * bgR;
			PotGrp[tmpnumber].Coor.Arr[0] = jianxi * l - tmpx + centerx;

			attrQ[tmpnumber] = Qlive;

			tmpnumber++;
		}
	}


	//2
	for (unsigned i = 0; i < layep; i++)
	{
		unsigned tmpz = bgtwoz[i] / jianxi;
		int tmpd = jianxi * i - bgR / 2;
		if (tmpd < 0)
		{
			tmpd = -tmpd;
		}
		unsigned tmpx = sqrt(pow(bgR / 2, 2) - tmpd * tmpd);
		for (unsigned l = 0; l < tmpz; l++)
		{

			PotGrp[tmpnumber].Coor.Arr[1] = -(int)jianxi * i + centery + 2 * bgR;
			PotGrp[tmpnumber].Coor.Arr[0] = -(int)jianxi * l - tmpx + centerx - 2 * PotR;

			attrQ[tmpnumber] = -(int)Qlive;

			tmpnumber++;
		}
	}


	for (unsigned i = 0; i < layep; i++)
	{
		unsigned tmpz = bgtwof[i] / jianxi;
		int tmpd = jianxi * i - bgR / 2;
		if (tmpd < 0)
		{
			tmpd = -tmpd;
		}
		unsigned tmpx = sqrt(pow(bgR / 2, 2) - tmpd * tmpd);
		for (unsigned l = 0; l < tmpz; l++)
		{
			PotGrp[tmpnumber].Coor.Arr[1] = jianxi * i + centery + 2 * PotR + wcy;
			PotGrp[tmpnumber].Coor.Arr[0] = -(int)jianxi * l + tmpx + centerx - 2 * PotR;

			attrQ[tmpnumber] = -(int)Qlive;

			tmpnumber++;
		}
	}
	//



	for (unsigned i = 0; i < PotNum; i++)
	{
		attr[i] = 5;
	}

	
	bgtwoz.reset();
	bgtwoz.reset();

	system("pause");


	AddInPart();
	setCoorUpFuc(&CoorUp);
	setFUpFuc(&FUp);

	unsigned addWit = 1000;
	unsigned addHit = 400;
	initgraph(sWit+ addWit, sHit+ addHit);
	setorigin(0, sHit);
	setaspectratio(1, -1);
	setfillcolor(RGB(255, 0, 0));
	setlinecolor(RGB(0,200,0));
	setlinestyle(PS_DASH);
	setbkcolor(RGB(127, 127, 127));
	//unsigned cs = 0;

	But Pause(10, sHit -5,30,20,RGB(200,100,50), sWit, sHit);
	Pause.Sta = true;

	while (true)
	{
	
		BeginBatchDraw();
		cleardevice();


		auto start = chrono::high_resolution_clock::now();

		if (!Pause.Sta)
		{
			SpaceDataUpUesPotGrp();
		}

		

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << "函数运行时长: " << duration.count() / 1000000 << " ms" << std::endl;
		

		for (unsigned i = 0; i < PotNum; i++)
		{
			if (attrQ[i]>0)
			{
				setfillcolor(RGB(0, 0, 0));
			}
			else
			{
				setfillcolor(RGB(255, 255, 255));
			}
			solidcircle(PotGrp[i].Coor.Arr[0]+ addWit/2, PotGrp[i].Coor.Arr[1] - addHit/2, 5);
			//circle(PotGrp[i].Coor.Arr[0] + addWit / 2, PotGrp[i].Coor.Arr[1] - addHit / 2, eff);
		}

		if (Pause.Sta)
		{
			Pause.ButDraw();
		}
		
		EndBatchDraw();

		ExMessage msg;

		if (peekmessage(&msg))
		{
			if (msg.lbutton)
			{
				if (Pause.ButDown(msg.x, msg.y))
				{
					Pause.Sta = !Pause.Sta;
				}
			}
		}

	}

	DestroyedSpace();

}




