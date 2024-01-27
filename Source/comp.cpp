#include"comp.h"
#include< exception >
#include<stdexcept>
//#include"tmpcomp.h"
//#include<graphics.h>
using namespace std;



namespace sp {
	unsigned Dimension;
	unsigned PointVision;
	unsigned PointVisionPowTwo;
	vector<unsigned> PartNum;
	vector<long long> Vensize;
	vector<unsigned>PartIndexUint;
	vector <double>PartUnit;
	unique_ptr<Part[]> pPart;
	vector<Point>PotGrp;
	vector<double>PartUintOneRem;
	FUPFUC fUpFuc;
	COORUPFUC coorUpFuc;



	Point::Point()
	{

	}

	Point::~Point()
	{

	}


	Part::Part()
	{
		pPotGrp.resize(BasePartGrp);
		for (auto& PotGrpu : pPotGrp)
		{
			PotGrpu = nullptr;
		}
	}

	Part::~Part()
	{
		
	}

	SilX::SilX() {

		GrpY = nullptr;

	}

	SilX::~SilX()
	{
		if (GrpY != nullptr)
		{
			delete[] GrpY;
		}

	}

	Sil::Sil() {
		GrpX = nullptr;
	}

	Sil::~Sil() {
		if (GrpX != nullptr)
		{
			delete[] GrpX;
		}
	}

	void CreateSpace(vector<unsigned> partCount, vector<long long> VenSize,unsigned pointvision) {
		PointVision = pointvision;
		PointVisionPowTwo = pointvision * pointvision;
		Dimension = partCount.size();


		for (unsigned PartNumu : partCount)
		{
			try {
				// 抛出一个异常  
				if (PartNumu == 0)
				{
					throw runtime_error("partCount can't '==0'");
				}

			}
			catch (const exception& e) {
				// 处理异常  
				cerr << "erorr：" << e.what() << endl;
				exit(-1);
			}

		}

		for (auto Vensizeu : VenSize)
		{
			try {
				// 抛出一个异常  
				if (Vensizeu < 100)
				{
					throw runtime_error("VenSize can't '<100'");
				}

			}
			catch (const exception& e) {
				// 处理异常  
				cerr << "erorr：" << e.what() << endl;
				exit(-1);
			}
		}

#ifdef SPACE_3D
		try {
			// 抛出一个异常  
			if (partCount.size() != 3 || VenSize.size() != 3)
			{
				throw runtime_error("size erorr!");
			}

		}
		catch (const exception& e) {
			// 处理异常  
			cerr << "erorr：" << e.what() << endl;
			exit(-1);
		}

		for (size_t i = 0; i < 3; i++)
		{
			double Tmps = VenSize[i] / partCount[i];
			PartUnit.push_back(Tmps);
			PartUintOneRem.push_back((Tmps - 1) / Tmps);
		}

		PartIndexUint.push_back(partCount[0]);
		PartIndexUint.push_back(partCount[0] * partCount[1]);

#endif // SPACE_3D

#ifndef SPACE_3D
			try {
				// 抛出一个异常  
				if (partCount.size() != 2|| VenSize.size()!=2)
				{
					throw runtime_error("size erorr!");
				}

			}
			catch (const exception& e) {
				// 处理异常  
				cerr << "erorr：" << e.what() << endl;
				exit(-1);
			}

			for (size_t i = 0; i < 2; i++)
			{
				double Tmps = VenSize[i] / partCount[i];
				PartUnit.push_back(Tmps);
				PartUintOneRem.push_back((Tmps - 1)/ Tmps);
			}
			PartIndexUint.push_back(partCount[0]);
#endif // !SPACE_3D

		PartNum = move(partCount);
		Vensize = move(VenSize);

		unsigned tmp = 1;
		for (auto partCountu : PartNum)
		{
			tmp *= partCountu;
		}

		pPart = make_unique<Part[]>(tmp);
	}



	void IndexParse(PartIndex& partcoor,LONGDOU coor) {
	
		for (unsigned i = 0; i < Dimension; i++)
		{
			partcoor.PartCor[i] = coor.Arr[i] / PartUnit[i];
		}

#ifdef SPACE_3D
		partcoor.Index = partcoor.PartCor[0];
		partcoor.Index += partcoor.PartCor[1] * PartIndexUint[0];
		partcoor.Index += partcoor.PartCor[2] * PartIndexUint[1];
#endif // !SPACE_3D


#ifndef SPACE_3D
		partcoor.Index = partcoor.PartCor[0];
		partcoor.Index += partcoor.PartCor[1] * PartIndexUint[0];
#endif // !SPACE_3D
		return;
	}


	void AddPointbat(long long PointCount,long long BaseNumbering) {
		PotGrp.resize(PointCount);
		
#ifndef HVAE_NO_NUMBERING
		for (long long i = 0; i < PointCount; i++)
		{
			PotGrp[i].Numbering = BaseNumbering + i;
		}
#endif // !HVAE_NO_NUMBERING
	}

	void setFUpFuc(FUPFUC fuc) {

		fUpFuc = fuc;
		 
	}

	void setCoorUpFuc(COORUPFUC fuc) {

		coorUpFuc = fuc;

	}



	Sil* CilLoad(LONGDOU Coor,unsigned* partCor) {
		Sil* sil = new Sil;
#ifdef SPACE_3D
		int Rale[3] = { NULL };
		for (unsigned i = 0; i < Dimension; i++)
		{
			Rale[i] = Coor.Arr[i] - partCor[i] * PartUnit[i];
			Rale[i] = (PartUnit[i] - Rale[i]) > Rale[i] ? Rale[i] : PartUnit[i] - Rale[i];
		}
		double RmX = (int)PointVision - Rale[0];
		sil->NumX = (RmX < 2) ? 1 : RmX / PartUnit[0] + PartUintOneRem[0] + 1;
		sil->GrpX = new SilX[sil->NumX];
		for (unsigned x = 0; x < sil->NumX; x++)
		{
			unsigned Rx = x == 0 ? 0 : (x - 1) * PartUnit[0] + Rale[0];
			Rx = Rx * Rx;
			double TmpRy = sqrtf(PointVisionPowTwo - Rx);
			double Rmy = TmpRy - Rale[1];
			sil->GrpX[x].NumY = (Rmy < 2) ? 1 : Rmy / PartUnit[1] + PartUintOneRem[1] + 1;
			sil->GrpX[x].GrpY = new SilY[sil->GrpX[x].NumY];
			for (unsigned y = 0; y < sil->GrpX[x].NumY; y++)
			{
				unsigned Ry = y == 0 ? 0 : (y - 1) * PartUnit[1] + Rale[1];
				Ry = Ry * Ry;
				double TmpRz = sqrtf(PointVisionPowTwo - Rx - Ry);
				double Rmz = TmpRz - Rale[2];
				sil->GrpX[x].GrpY[y].NumZ = (Rmz < 2) ? 1 : Rmz / PartUnit[2] + PartUintOneRem[2] + 1;
			}
		}
#endif

#ifndef SPACE_3D
		int Rale[2] = { NULL };
		for (unsigned i = 0; i < Dimension; i++)
		{
			Rale[i] = Coor.Arr[i] - partCor[i] * PartUnit[i];
			Rale[i] = (PartUnit[i] - Rale[i]) > Rale[i] ? Rale[i] : PartUnit[i] - Rale[i];
		}

		double RmX = (int)PointVision - Rale[0];
		sil->NumX = (RmX < 2) ? 1 : RmX / PartUnit[0] + PartUintOneRem[0] + 1;
		sil->GrpX = new SilX[sil->NumX];
		for (unsigned x = 0; x < sil->NumX; x++)
		{
			unsigned Rx = x == 0 ? 0 : (x - 1) * PartUnit[0] + Rale[0];
			Rx = Rx * Rx;
			double TmpRy = sqrtf(PointVisionPowTwo - Rx);
			double Rmy = TmpRy - Rale[1];
			sil->GrpX[x].NumY = (Rmy < 2) ? 1 : Rmy / PartUnit[1] + PartUintOneRem[1] + 1;
		}
#endif
		return sil;
	}

	

	void OnePartTraverse(Point& self,Part* part) {
		for (auto pPotGrpu : part->pPotGrp)
		{
			if (pPotGrpu!=nullptr)
			{
				double TmpDisPow = ThrDistancePow(self.Coor, pPotGrpu->Coor);
				if (TmpDisPow!=0)
				{
					if (TmpDisPow <= PointVisionPowTwo)
					{
						fUpFuc(self, pPotGrpu, TmpDisPow);
						return;
					}
				}
				
			}
			
		}
	}


	void PoitRange(Point& self,Sil* tmpsil) {
#ifdef SPACE_3D
		for (unsigned x = 0; x < tmpsil->NumX; x++)
		{
			unsigned XDCNum;
			XDCNum = x == 0 ? 1 : 2;
			for (unsigned y = 0; y < tmpsil->GrpX[x].NumY; y++)
			{
				unsigned YDCNum;
				YDCNum = y == 0 ? 1 : 2;
				for (unsigned z = 0; z < tmpsil->GrpX[x].GrpY[y].NumZ; z++)
				{
					int Sym[] = { 1,-1 };

					unsigned ZDCNum;

					ZDCNum = z == 0 ? 1 : 2;

					for (unsigned xt = 0; xt < XDCNum; xt++)
					{
						int tmpX = Sym[xt] * x + self.partindex.PartCor[0];
						if (tmpX < 0 || tmpX >= PartNum[0])
						{
							continue;
						}
						for (unsigned yt = 0; yt < YDCNum; yt++)
						{
							int tmpy = Sym[yt] * y + self.partindex.PartCor[1];
							if (tmpy < 0 || tmpy >= PartNum[1])
							{
								continue;
							}
							for (unsigned zt = 0; zt < ZDCNum; zt++)
							{

								int tmpz = Sym[zt] * z + self.partindex.PartCor[2];
								if (tmpz < 0 || tmpz >= PartNum[2])
								{
									continue;
								}

								int tmpindex = tmpX + tmpy * PartIndexUint[0] + tmpz * PartIndexUint[1];
								OnePartTraverse(self, &pPart[tmpindex]);
							}
						}
					}
				}
			}
		}
#endif // SPACE_3D

#ifndef SPACE_3D
		for (unsigned x = 0; x < tmpsil->NumX; x++)
		{
			unsigned XDCNum;
			XDCNum = x == 0 ? 1 : 2;
			for (unsigned y = 0; y < tmpsil->GrpX[x].NumY; y++)
			{
				unsigned YDCNum;
				YDCNum = y == 0 ? 1 : 2;
				int Sym[] = { 1,-1 };

				for (unsigned xt = 0; xt < XDCNum; xt++)
				{
					int tmpX = Sym[xt] * x + self.partindex.PartCor[0];
					if (tmpX < 0 || tmpX >= PartNum[0])
					{
						continue;
					}
					for (unsigned yt = 0; yt < YDCNum; yt++)
					{
						int tmpy = Sym[yt] * y + self.partindex.PartCor[1];
						if (tmpy < 0 || tmpy >= PartNum[1])
						{
							continue;
						}
						int tmpindex = tmpX + tmpy * PartIndexUint[0];
						//drawragn(tmpX, tmpy);
						OnePartTraverse(self, &pPart[tmpindex]);
					}
				}
			}
		}

#endif // !SPACE_3D
	}
	
	void UpPotPart(Point& self) {
		unsigned oldindex = self.partindex.Index;
		IndexParse(self.partindex, self.Coor);
	
		if (oldindex != self.partindex.Index)
		{
			pPart[oldindex].pPotGrp[self.PartGrpIndex] = nullptr;
			if (pPart[oldindex].pPotGrp.size()>MAX_GRPCOUNT)
			{
				if (pPart[oldindex].pPotGrp[pPart[oldindex].pPotGrp.size()-1]==nullptr)
				{
					pPart[oldindex].pPotGrp.pop_back();
				}
			}
		
			for (unsigned i = 0; i < pPart[self.partindex.Index].pPotGrp.size(); i++)
			{
				if (pPart[self.partindex.Index].pPotGrp[i] == nullptr)
				{
					pPart[self.partindex.Index].pPotGrp[i] = &self;
					self.PartGrpIndex = i;
					return;
				}
			}

			
			pPart[self.partindex.Index].pPotGrp.push_back(&self);
			self.PartGrpIndex = pPart[self.partindex.Index].pPotGrp.size() - 1;
			return;
		}
		return;
	}

	double ThrDistancePow(LONGDOU hcoor, LONGDOU gcoor) {
		double pows = 0;
		for (unsigned int i = 0; i < Dimension; i++)
		{
			double c = (double)hcoor.Arr[i] - (double)gcoor.Arr[i];
			pows = pows +c*c;
		}
		return pows;
	}

	double Distance(double xed, double x) {
		double distance = xed - x;
		return distance;
	}
	

	void SpaceDataUpUesPotGrp() {
		for (auto& PotGrpu:PotGrp)
		{
			Sil* tmpsil;
			tmpsil = CilLoad(PotGrpu.Coor , PotGrpu.partindex.PartCor);
			PoitRange(PotGrpu,tmpsil);
			delete tmpsil;
		}

		for (auto& PotGrpu : PotGrp)
		{
			coorUpFuc(PotGrpu);
#ifdef ERORR_OVERRANGE
			try {
				for (unsigned i = 0; i < Dimension; i++)
				{
					if (PotGrpu.Coor.Arr[i] < 0 || PotGrpu.Coor.Arr[i]>Vensize[i])
					{
						throw runtime_error("coor over range(Vensize)");
					}
				}
				// 抛出一个异常  
			}
			catch (const exception& e) {
				// 处理异常  
				cerr << "erorr：" << e.what() << endl;
				exit(-1);
			}
#endif // ERORR_OVERRANGE


		}



		for (auto& PotGrpu : PotGrp)
		{
			UpPotPart(PotGrpu);
		}


	}

	void SpaceDataUpUesPartGrp() {



	}


	void DestroyedSpace() {
		PartNum.clear();
		Vensize.clear();
		pPart = nullptr;
		PotGrp.clear();
	}
	

	void AddOneInOnePart(Point& pot) {
		for (unsigned i = 0; i < pPart[pot.partindex.Index].pPotGrp.size(); i++)
		{
			if (pPart[pot.partindex.Index].pPotGrp[i] == nullptr)
			{
				pPart[pot.partindex.Index].pPotGrp[i] = &pot;
				pot.PartGrpIndex = i;
				return;
			}
		}

		pPart[pot.partindex.Index].pPotGrp.push_back(&pot);
		pot.PartGrpIndex = pPart[pot.partindex.Index].pPotGrp.size() - 1;
	}

	void AddInPart() {

		for (auto& PotGrpu : PotGrp)
		{
			IndexParse(PotGrpu.partindex, PotGrpu.Coor);
			
			AddOneInOnePart(PotGrpu);
		}
		return;
	}

	void setCoor(SETCOORFUC fuc) {
		for (auto& PotGrpu : PotGrp)
		{
			fuc(PotGrpu);

			try {
				for (unsigned i = 0; i < Dimension; i++)
				{
					if (PotGrpu.Coor.Arr[i] < 0|| PotGrpu.Coor.Arr[i]>Vensize[i])
					{
						throw runtime_error("coor over range(Vensize)");
					}
				}
				// 抛出一个异常  
			}
			catch (const exception& e) {
				// 处理异常  
				cerr << "erorr：" << e.what() << endl;
				exit(-1);
			}
		}
	}

}
