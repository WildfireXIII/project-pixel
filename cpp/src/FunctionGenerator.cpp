//*************************************************************
//  File: FunctionGenerator.cpp
//  Date created: 2/19/2017
//  Date edited: 8/10/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: 
//*************************************************************

#ifndef FF_FUNCTION_GENERATOR_CPP
#define FF_FUNCTION_GENERATOR_CPP

#include "FunctionGenerator.h"

namespace dwl
{
	FFFunction* FunctionGenerator::GenerateFunction(float fDecisionThreshold)
	{
		FFFunction* pFunction = new FFFunction();

		float fA = 0.0f;
		float fB = 0.0f;
		float fC = 0.0f;
		float fD = 0.0f;
		float fE = 0.0f;
		float fF = 0.0f;

		
		if (RandomFloat() > .5) { fA = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fB = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fC = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fD = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fE = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fF = RandomFloat() * 4 - 2; }

		bool bValidFunction = false; // need at least ONE variation to have a weight
		
		while (!bValidFunction)
		{
			for (int i = 0; i <= 12; i++)
			{
				if (RandomFloat() > fDecisionThreshold)
				{
					bValidFunction = true;
					pFunction->SetVariationWeight(i, RandomFloat());
				}
			}
		}

		pFunction->SetMatrixCoefficients({fA, fB, fC, fD, fE, fF});
		pFunction->SetWeight(RandomFloat());
		pFunction->SetColor(RandomFloat());

		return pFunction;
	}
	
	FFFunction* FunctionGenerator::GenerateSymmetryFunction(int iDegree)
	{
		FFFunction* pFunction = new FFFunction();

		float fA = (float)cos(iDegree*PI/180);
		float fB = (float)sin(iDegree*PI/180);
		float fC = 0.0f;
		float fD = -(float)sin(iDegree*PI/180);
		float fE = (float)cos(iDegree*PI/180);
		float fF = 0.0f;

		
		pFunction->SetVariationWeight(FFFunction::VAR_LINEAR, 1.0f);
		pFunction->SetMatrixCoefficients({fA, fB, fC, fD, fE, fF});
		
		pFunction->SetWeight(RandomFloat());
		pFunction->SetSymmetry(true);
		pFunction->SetColor(RandomFloat());

		return pFunction;
	}
	
	float FunctionGenerator::RandomFloat()
	{
		random_device rd;
		srand(rd());
		float fRandom = ((float) rand()) / (float) RAND_MAX;

		return fRandom;
	}

	// NOTE: bounds are inclusive (and must be positive)
	int FunctionGenerator::RandomInt(int iLow, int iHigh) { return (int)round(RandomFloat() * (iHigh-iLow) + iLow); }

	void FunctionGenerator::GenerateFractalFunctionSet(FlameFractal* pFractal)
	{
		// How many functions?
		int iFunctionCount = RandomInt(4,15);
		cout << "Choosing " << iFunctionCount << " functions..." << endl;

		float fWeightSum = 0.0f;
		
		for (int i = 0; i < iFunctionCount; i++)
		{
			float fVarDecThresh = (RandomFloat() * 0.8f) + .1; // .1-.9
			cout << "Variation decision thresh: " << fVarDecThresh << endl;
			FFFunction* pFunc = this->GenerateFunction(fVarDecThresh);
			fWeightSum += pFunc->GetWeight();
			pFractal->AddFunction(*pFunc);
		}

		// Symmetry?
		if (RandomFloat() > .3)
		{
			int iSymmetryCount = RandomInt(2,8);
			int iDegree = (int)round(360.0f / (float)iSymmetryCount);

			cout << "Using " << iSymmetryCount << " symmetry points..." << endl;

			for (int i = 1; i <= iSymmetryCount - 1; i++)
			{
				FFFunction* pFunc = this->GenerateSymmetryFunction(i*iDegree);
				pFunc->SetWeight(fWeightSum);
				pFractal->AddFunction(*pFunc);
			}
		}
	}
}

#endif
