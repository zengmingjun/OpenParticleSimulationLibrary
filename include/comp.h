#include"def.h"
#include<iostream>
#include<vector>
#include<memory>

using namespace std;

namespace sp {

#ifdef SPACE_3D

	class LONGDOU
	{
	public:
		long double Arr[3] = { NULL };
	};

	class DOU
	{
	public:
		double Arr[3] = { NULL };
	};

	class UNSI
	{
	public:
		unsigned Arr[3] = {NULL};
	};
	class PartIndex
	{
	public:
		unsigned PartCor[3] = {NULL};
		unsigned Index = 0;
	};

#endif

#ifndef SPACE_3D

	class LONGDOU
	{
	public:
		long double Arr[2] = { NULL };
	};

	class DOU
	{
	public:
		double Arr[2] = { NULL };
	};

	class UNSI
	{
	public:
		unsigned Arr[2] = { NULL };
	};

	class PartIndex
	{
	public:
		unsigned PartCor[2] = { NULL };
		unsigned Index = 0;
		
	};
#endif

	class Point
	{
	public:
#ifndef HVAE_NO_NUMBERING
		long long Numbering;
#endif // !HVAE_NO_NUMBERING
		PartIndex partindex;
		unsigned PartGrpIndex;
		LONGDOU Coor;
		DOU F;
		DOU V;
		Point();
		~Point();
	};

	class Part
	{
	public:
		vector<Point*>pPotGrp;
		Part();
		~Part();
	};

	typedef void(*FUPFUC)(Point& selfFData, Point* GestFData,double DisPowTwo);
	typedef void(*COORUPFUC)(Point& self);
	typedef void(*SETCOORFUC)(Point& self);


	class SilZCor
	{
	public:
		unsigned Cor[3];
		SilZCor();
	};


	class SilYCor
	{
	public:
		unsigned Cor[2];
		SilYCor();
	};


	class SilY
	{
	public:
		unsigned NumZ;
	};

	class SilX
	{
	public:
		unsigned NumY;
		SilY* GrpY;
		SilX();
		~SilX();
	};

	class Sil
	{
	public:
		unsigned NumX;
		SilX* GrpX;
		Sil();
		~Sil();
	};

	class RangePass
	{
	public:
		Sil* FixedCor;
		vector<SilYCor>RemCor;
		RangePass();
		~RangePass();
	};

	

	/*extern unsigned Dimension;
	extern vector<unsigned> PartNum;
	extern vector<long long> Vensize;
	extern vector<unsigned>PartIndexUint;
	extern unique_ptr<Part[]> pPart;*/
	extern vector<Point>PotGrp;
	void CreateSpace(vector<unsigned> partCount,vector<long long> VenSize, unsigned pointvision);
	void AddPointbat(long long PointCount, long long BaseNumbering);
	void setCoor(SETCOORFUC fuc);
	void AddInPart();
	void setFUpFuc(FUPFUC fuc);
	void setCoorUpFuc(COORUPFUC fuc);
	
	void SpaceDataUpUesPotGrp();
	void SpaceDataUpUesPartGrp();

	void IndexParse(PartIndex& partcoor, LONGDOU coor);
	double ThrDistancePow(LONGDOU hcoor, LONGDOU gcoor);
	double Distance(double xed, double x);
	void DestroyedSpace();
}
