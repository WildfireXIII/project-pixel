//*************************************************************
//  File: Generator.cpp
//  Date created: 1/28/2017
//  Date edited: 08/15/2018
//  Author: Nathan Martindale
//  Copyright © 2018 Digital Warrior Labs
//  Description: 
//*************************************************************


#include <iostream>
#include <iomanip>

#include <cmath>
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include <chrono>
#include <thread>

#include "FlameFractal.h"
#include "FunctionGenerator.h"

using namespace std;
using namespace dwl;

void REPL();
int HandleCommand(string sCommand);
string GetRollingCommand();

void StoreCollectionNum();
void LoadCollectionNum();

FlameFractal* pFractal = new FlameFractal(0,0);
int iCollection = 0;
string sErrorMsg = "";

int iInstanceID = -1;

bool bRollingMode = false;
bool bPaused = false;


float fStatGamma = 0.0f;
float fStatBrightness = 0.0f;
int iStatFilter = 0;
float fStatHistWeight = 1.0f;
float fStatDensityWeight = 1.0f;
float fStatSecondBlur = .2f;

int main(int argc, char **argv)
{
	LoadCollectionNum();
	cout << ">> Collection: " << iCollection << endl;

	
	if (argc > 1)
	{
		// get instance ID
		srand(time(NULL));
		iInstanceID = rand();
		cout << "Instance ID: " << iInstanceID << endl;
		
		string cmd = "run " + string(argv[1]);
		
		// get arguments if any
		for (int i = 2; i < argc; i++) { cmd += " " + string(argv[i]); }
		
		HandleCommand(cmd);
		cout << "Programmatic access to cached rendered results with following id:" << endl;
		cout << iInstanceID << endl;
		return 0;
	}

	REPL();
	return 0;
	
	FlameFractal ff = FlameFractal(1000, 1000);
	
	int iCollection = 22;

	/*FunctionGenerator pGen = FunctionGenerator();
	FFFunction* pF0 = pGen.GenerateFunction();
	ff.AddFunction(*pF0);
	FFFunction* pF1 = pGen.GenerateFunction();
	ff.AddFunction(*pF1);
	FFFunction* pF2 = pGen.GenerateFunction();
	ff.AddFunction(*pF2);
	FFFunction* pF3 = pGen.GenerateFunction();
	ff.AddFunction(*pF3);

	FFFunction* pFSym = pGen.GenerateSymmetryFunction(120);
	pFSym->SetWeight(pF0->GetWeight() + pF1->GetWeight() + pF2->GetWeight() + pF3->GetWeight());
	ff.AddFunction(*pFSym);
	FFFunction* pFSym2 = pGen.GenerateSymmetryFunction(240);
	pFSym2->SetWeight(pF0->GetWeight() + pF1->GetWeight() + pF2->GetWeight() + pF3->GetWeight());
	ff.AddFunction(*pFSym2);*/
	
	// colors
	//ff.SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.0f, 0.5f, 1.0f}}); // nice blue!
	//ff.SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.5f}}); // greeen?
	//ff.SetColorRamp({0.0f, 0.5f, 1.0f}, {{1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.7f, 0.4f, 1.0f}}); // TTU!
	//ff.SetColorRamp({0.0f, 0.15f, 0.3f, 0.55f, 0.7f, 1.0f}, {{1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f, 1.0f}, {0.8f, 0.2f, 1.0f}});
	//ff.SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.7f, 0.4f, 1.0f}}); // purple
	//ff.SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}}); // deep purple
	ff.SetColorRamp({0.0f, 0.5f, 1.0f}, {{0.0f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}}); // purple'n blue
	

	ff.SetZoom(.4, .4);
	ff.PreparePlot();
	ff.InitializeSolution(); 

	ff.LoadFunctionCode("collection/" + to_string(iCollection));
	//ff.LoadImageTrace("collection/" + to_string(iCollection));
	//ff.SaveFunctionCode("collection/" + to_string(iCollection));
	
	ff.Solve(10000000);
	//ff.SaveImageTrace("collection/" + to_string(iCollection));
	
	// render
	ff.Render(2.8, 1.2, 0, 0, 0, 0);
	ff.SaveImageData("imgdata.json");
	system("python3 ./saveaspng.py");
	string sCopyCommand = "copy \"./render.png\" \"./collection/" + to_string(iCollection) + "_render.png\""; // TODO: make cross platform!!
	system(sCopyCommand.c_str());
	

	/*int iPart = 1;
	
	// part 1
	if (iPart == 1)
	{
		ff.PreparePlot();
		ff.InitializeSolution();
		ff.Solve(10000000);
		ff.Render(2.2, 1.0, 0);
		ff.SaveImageTrace("Test");
		ff.SaveImageData("imgdata.json");
		ff.SaveFunctionCode("TestFunctions");
		system("python3 ./saveaspng.py");
	}
	
	// part 2
	if (iPart == 2)
	{
		ff.PreparePlot();
		ff.LoadImageTrace("Test");
		ff.Solve(100000000);
		ff.Render(2.2, 1.0, 0);
		ff.SaveImageTrace("Test");
		ff.SaveImageData("imgdata.json");
		system("python3 ./saveaspng.py");
	}*/
	
	//ff.SetBaseImage(100.0f, 0.0f, 0.0f, 255.0f);
	//ff.LoadFunctionCode("Test");
	//ff.SaveImageTrace("Test");
	//ff.LoadImageTrace("Test");

	//SaveImage("imgdata.json", &ff);
	
	
	/*random_device rd;
	srand(rd());
	float fRandom = ((float) rand()) / (float) RAND_MAX;

	cout << "Random: " << fRandom << endl;*/


	//FFFunction f = FFFunction();
	//f.SetVariationWeight(FFFunction::VAR_LINEAR, 1.0f);
	//cout << "Running function..." << endl;
	////vector<float> result = f.Run(1.0, 1.0);
	//f.Run(1.0, 1.0);
	//float x = f.GetResultX();
	//float y = f.GetResultY();
	//cout << "x: " << x << " y: " << y << endl;

	//for (int i = 0; i < result.size(); i++)
	//{
		//cout << result[i] << endl;
	//}

	
	return 0;
}

void StoreCollectionNum()
{
	cout << ">> Storing collection num..." << endl;
	ofstream fFile;
	fFile.open("./collection/collection", ios::out);
	fFile << iCollection;
	fFile.close();
}

void LoadCollectionNum()
{
	cout << ">> Loading collection num..." << endl;
	ifstream fFile;
	fFile.open("./collection/collection");
	fFile >> iCollection;
	fFile.close();
}

// NOTE: pause works by sending "PAUSE" to handle command the NEXT time (hence the && !bPaused)
string GetRollingCommand()
{
	//if (!bPaused) { cout << ">> Checking for next rolling command" << endl; }

	ifstream fFile;
	fFile.open("./scripts/_rolling");
	string sNextRollingCommand = "";
	getline(fFile, sNextRollingCommand);
	vector<string> vCommands = vector<string>();
	string sLine = "";
	while (getline(fFile, sLine)) { vCommands.push_back(sLine); }
	fFile.close();
	
	string sOutput = "";
	for (int i = 0; i < vCommands.size(); i++) { sOutput += vCommands[i] + "\n"; }
	if (vCommands.size() == 0 && !bPaused) { bPaused = true; }
	else if (vCommands.size() == 0 && bPaused && sNextRollingCommand == "") { return "PAUSE"; }
	else if (vCommands.size() == 0 && bPaused && sNextRollingCommand != "") { bPaused = true; }
	else { bPaused = false; }
	ofstream pFile;
	pFile.open("./scripts/_rolling", ios::out);
	pFile << sOutput;
	pFile.close();
	
	//cout << sNextRollingCommand << endl;
	return sNextRollingCommand;
}

void REPL()
{
	int iResult = 0;
	while (iResult != 2)
	{
		if (!bRollingMode) { cout << "Pixel> "; }
		string sCommand = "";
		//cin >> sCommand;
		if (bRollingMode) { sCommand = GetRollingCommand(); }
		else { getline(cin, sCommand); }
		iResult = HandleCommand(sCommand);
		if (iResult == 1) 
		{ 
			cout << "> ERROR: " << sErrorMsg << endl; 
			if (bRollingMode) 
			{ 
				bRollingMode = false;
				cout << ">> Exiting rolling command mode" << endl;
			}
		}
		else if (iResult == 3) { this_thread::sleep_for(chrono::milliseconds(10000)); }
	}
}

// 0 = normal, 1 = error, 2 = exit, 3 = pause
int HandleCommand(string sCommand)
{
	if (sCommand != "PAUSE") { cout << ">>> " << sCommand << endl; }

	// split on space (black magic split)
	stringstream ss(sCommand);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> vParts(begin, end);

	if (vParts.size() == 0) { return 0; cin.ignore(); cin.clear(); }
	if (vParts[0][0] == '#') { return 0; }
	
	if (vParts[0] == "exit")
	{
		if (vParts.size() == 2 && vParts[1] == "rolling")
		{
			cout << ">> Exiting rolling script mode" << endl;
			bRollingMode = false;
			return 0;
		}
		cout << ">> Have a nice day!" << endl;
		return 2;
	}
	else if (vParts[0] == "PAUSE") { bPaused = true; return 3; }
	else if (vParts[0] == "help")
	{
		cout << "exit {rolling}\necho [MESSAGE]\ncollection [INDEX|increment]\nrun [SCRIPT|rolling] {ARGS}\ncreate [WIDTH] [HEIGHT]\nzoom [X] [Y]\ncolor [blue|green|ttu|purple|purpleblue|orange|yellow|red|portal]\ninit\nsolve [COUNT|avg] {AVGDENSITY}\nrender [GAMMA] [BRIGHTNESS] {FILTERNUM} {HISTBLURWEIGHT=1} {DENSITYBLURWEIGHT=1} {SECONDPASSBLUR=.2}\ngenerate\nsave [image|functions|trace|collection] {FILE}\nload [functions|trace] [FILE]\nproduce [experiment|client|official|personal] [COLLECTION|current] [WIDTH] [HEIGHT] [QUALITY] [COLOR] [ZOOM] [GAMMA] [BRIGHTNESS] {FILTERNUM} {HISTBLURWEIGHT} {DENSITYBLURWEIGHT} {SECONDPASSBLUR}" << endl;
		return 0;
	}
	else if (vParts[0] == "run")
	{
		if (vParts.size() < 2)
		{
			sErrorMsg = "Bad arguments!nFORMAT: run [SCRIPT] {ARGS}";
			return 1;
		}

		// check for starting rolling mode
		if (vParts[1] == "rolling")
		{
			cout << ">> Rolling script mode started! Add commands to scripts/_rolling (and add 'exit rolling' inside the file to return to normal REPL mode)" << endl;
			bRollingMode = true;
			return 0;
		}

		// add the scripts subdir automatically (should never have scripts outside of that)
		vParts[1] = "scripts/" + vParts[1];

		ifstream pFile;
		pFile.open(vParts[1].c_str());

		cout << ">> Running '" << vParts[1] << "'..." << endl;
		string sLine;
		while (getline(pFile, sLine))
		{
			//string sLine;
			//getline(pFile, sLine);

			// replace any $# with the respective argument (if it exists)
			for (int i = 2; i < vParts.size(); i++)
			{
				int iArgNum = i - 2;
				string sSearch = "$" + to_string(iArgNum);
				cout << "Searching for " << sSearch << "..." << endl;
				
				size_t cSearchResults = sLine.find(sSearch);
				while (cSearchResults != string::npos)
				{
					sLine.replace(cSearchResults, sSearch.length(), vParts[i]);
					cSearchResults = sLine.find(sSearch);
				}
			}

			int iResult = HandleCommand(sLine);
			if (iResult == 1) { pFile.close(); return 1; }
		}
		cout << ">> '" << vParts[1] << "' finished executing" << endl;
		
		pFile.close();
		return 0;
	}
	else if (vParts[0] == "create")
	{
		if (vParts.size() != 3) 
		{ 
			sErrorMsg = "Bad arguments!\nFORMAT: create [WIDTH] [HEIGHT]";
			return 1;
		}
		//float fWidth = (float)vParts[1];
		//float fHeight = (float)vParts[2];
		int iWidth = stoi(vParts[1]);
		int iHeight = stoi(vParts[2]);
		cout << ">> Parsed " << "[Width: " << iWidth << "] [Height: " << iHeight << "]" << endl;
		
		delete pFractal;
		pFractal = new FlameFractal(iWidth, iHeight);
		pFractal->PreparePlot();
		
		return 0;
	}
	else if (vParts[0] == "init") { pFractal->InitializeSolution(); return 0; }
	else if (vParts[0] == "zoom")
	{
		if (vParts.size() != 3) 
		{ 
			sErrorMsg = "Bad arguments!\nFORMAT: zoom [XSCALAR] [YSCALAR]";
			return 1;
		}
		//float fWidth = (float)vParts[1];
		//float fHeight = (float)vParts[2];
		float fX = stof(vParts[1]);
		float fY = stof(vParts[2]);
		cout << ">> Parsed " << "[X: " << fX << "] [Y: " << fY << "]" << endl;
		
		pFractal->SetZoom(fX, fY);
		
		return 0;
	}

	else if (vParts[0] == "collection")
	{
		if (vParts.size() != 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: collection [INDEX|increment]";
			return 1;
		}

		if (vParts[1] == "increment") { iCollection++; }
		else
		{
			int iIndex = stoi(vParts[1]);
			cout << ">> Parsed " << "[Index: " << iIndex << "]" << endl;
			iCollection = iIndex;
		}
		return 0;
	}

	else if (vParts[0] == "color")
	{
		if (vParts.size() != 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: color [COLORRAMPNAME]";
			return 1;
		}

		cout << ">> Parsed " << "[Color: " << vParts[1] << "]" << endl;

		bool bFound = false;
		if (vParts[1] == "blue")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.0f, 0.5f, 1.0f}}); // nice blue!
			pFractal->SetColorName("blue");
			bFound = true;
		}
		else if (vParts[1] == "green")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.5f}}); // greeen
			pFractal->SetColorName("green");
			bFound = true;
		}
		else if (vParts[1] == "ttu")
		{
			pFractal->SetColorRamp({0.0f, 0.5f, 1.0f}, {{1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.7f, 0.4f, 1.0f}}); // TTU!
			pFractal->SetColorName("ttu");
			bFound = true;
		}
		else if (vParts[1] == "purple")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.7f, 0.4f, 1.0f}}); // purple
			pFractal->SetColorName("purple");
			bFound = true;
		}
		else if (vParts[1] == "purpleblue")
		{
			pFractal->SetColorRamp({0.0f, 0.5f, 1.0f}, {{0.0f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}}); // purple'n blue
			pFractal->SetColorName("purpleblue");
			bFound = true;
		}
		else if (vParts[1] == "orange")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {1.0f, 0.5f, 0.0f}});
			pFractal->SetColorName("orange");
			bFound = true;
		}
		else if (vParts[1] == "yellow")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}});
			pFractal->SetColorName("yellow");
			bFound = true;
		}
		else if (vParts[1] == "red")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
			pFractal->SetColorName("red");
			bFound = true;
		}
		else if (vParts[1] == "portal")
		{
			pFractal->SetColorRamp({0.0f, 0.5f, 1.0f}, {{1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.5f, 1.0f}});
			pFractal->SetColorName("portal");
			bFound = true;
		}
		else if (vParts[1] == "teal")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.25f, 0.75f, 0.7f}});
			pFractal->SetColorName("teal");
			bFound = true;
		}
		else if (vParts[1] == "neongreen")
		{
			//pFractal->SetColorRamp({0.0f, 0.6f, 1.0f}, {{0.01f, 0.01f, 0.01f}, {1.0f, 1.0f, 1.0f}, {0.69f, 1.0f, .07f}});
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.69f, 1.0f, .07f}});
			pFractal->SetColorName("neongreen");
			bFound = true;
		}

		if (!bFound)
		{
			sErrorMsg = "Could not find colorscheme '" + vParts[1] + "'";
			return 1;
		}
		//cout << "Set color ramp!" << endl;
		return 0;
	}

	else if (vParts[0] == "generate")
	{
		cout << ">> Generating random functions..." << endl;
		FunctionGenerator pGen = FunctionGenerator();
		pGen.GenerateFractalFunctionSet(pFractal);
		/*FFFunction* pF0 = pGen.GenerateFunction();
		pFractal->AddFunction(*pF0);
		FFFunction* pF1 = pGen.GenerateFunction();
		pFractal->AddFunction(*pF1);
		FFFunction* pF2 = pGen.GenerateFunction();
		pFractal->AddFunction(*pF2);
		FFFunction* pF3 = pGen.GenerateFunction();
		pFractal->AddFunction(*pF3);

		FFFunction* pFSym = pGen.GenerateSymmetryFunction(120);
		pFSym->SetWeight(pF0->GetWeight() + pF1->GetWeight() + pF2->GetWeight() + pF3->GetWeight());
		pFractal->AddFunction(*pFSym);
		FFFunction* pFSym2 = pGen.GenerateSymmetryFunction(240);
		pFSym2->SetWeight(pF0->GetWeight() + pF1->GetWeight() + pF2->GetWeight() + pF3->GetWeight());
		pFractal->AddFunction(*pFSym2);*/

		cout << ">> Generated!" << endl;
		return 0;
	}

	else if (vParts[0] == "load")
	{
		if (vParts.size() < 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: load [functions|trace] {FILE}";
			return 1;
		}
		if (vParts[1] == "functions")
		{
			string sFileName = "";
			
			if (vParts.size() == 2) { sFileName = "collection/" + to_string(iCollection); }
			else { sFileName = vParts[2]; }

			pFractal->LoadFunctionCode(sFileName);
			return 0;
		}
		if (vParts[1] == "trace")
		{
			string sFileName = "";
			
			if (vParts.size() == 2) { sFileName = "collection/" + to_string(iCollection); }
			else { sFileName = vParts[2]; }
			
			pFractal->LoadImageTrace(sFileName);
			return 0;
		}
		if (vParts[1] == "collection") { LoadCollectionNum(); return 0; }
	}

	else if (vParts[0] == "solve")
	{
		if (vParts.size() < 2 || vParts.size() > 3)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: solve [COUNT|avg] {AVERAGEDENSITY}";
			return 1;
		}
		//int iSamples = 0;
		long iSamples = 0;
		if (vParts[1] == "avg")
		{
			int iAvg = stoi(vParts[2]);
			iSamples = (long int)(pFractal->GetWidth() * pFractal->GetHeight()) * (long int)(iAvg);
		}
		else { iSamples = stoi(vParts[1]); }
		
		cout << ">> Parsed [Samples: " << iSamples << "]" << endl;
		pFractal->Solve(iSamples);
		return 0;
	}

	else if (vParts[0] == "render")
	{
		if (vParts.size() < 3 || vParts.size() > 7)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: render [GAMMA] [BRIGHTNESS] {FILTERNUM} {HISTBLURWEIGHT=1} {DENSITYBLURWEIGHT=1} {SECONDPASBLUR=.2}";
			return 1;
		}

		float fGamma = stof(vParts[1]);
		float fBrightness = stof(vParts[2]);
		int iFilter = 0;
		float fHistBlurWeight = 1;
		float fDensityBlurWeight = 1;
		float fSecondPassBlur = 0.2f;
		if (vParts.size() > 3) { iFilter = stoi(vParts[3]); }
		if (vParts.size() > 4) { fHistBlurWeight = stof(vParts[4]); }
		if (vParts.size() > 5) { fDensityBlurWeight = stof(vParts[5]); }
		if (vParts.size() > 6) { fSecondPassBlur = stof(vParts[6]); }
		cout << ">> Parsed [Gamma: " << fGamma << "] [Brightness: " << fBrightness << "]" << endl;

		fStatGamma = fGamma;
		fStatBrightness = fBrightness;
		iStatFilter = iFilter;
		fStatHistWeight = fHistBlurWeight;
		fStatDensityWeight = fDensityBlurWeight;
		fStatSecondBlur = fSecondPassBlur;
		
		pFractal->Render(fGamma, fBrightness, iFilter, fHistBlurWeight, fDensityBlurWeight, fSecondPassBlur);
		return 0;
	}

	else if (vParts[0] == "save")
	{
		if (vParts.size() < 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: save [image|functions|trace|collection] {FILE}";
			return 1;
		}

		if (vParts[1] == "image")
		{
			system("mkdir -p render/cache");

			string loc = "imgdata.json";
			if (iInstanceID != -1) { loc = to_string(iInstanceID) + "_imgdata.json"; }
			
			pFractal->SaveImageData("render/cache/" + loc);
			
			string saveCMD = "";
			if (iInstanceID != -1) { saveCMD = "python3 ./saveaspng.py " + to_string(iInstanceID); }
			else { saveCMD = "python3 ./saveaspng.py"; }
			system(saveCMD.c_str());

			string sFileName = "";
			
			if (vParts.size() == 2) { sFileName = "./collection/" + to_string(iCollection) + "_render.png"; }
			else if (vParts[2] == "experiment" || vParts[2] == "official" || vParts[2] == "personal" || vParts[2] == "client")
			{
				string sMkdirCommand = "";
				if (vParts[2] == "experiment") { sMkdirCommand = "mkdir -p ./render/experiments/" + to_string(iCollection); }
				else if (vParts[2] == "personal") { sMkdirCommand = "mkdir -p ./render/personal/" + to_string(iCollection); }
				else if (vParts[2] ==  "client") { sMkdirCommand = "mkdir -p ./render/client/" + to_string(iCollection); }
				else { sMkdirCommand = "mkdir -p ./render/official/" + to_string(iCollection); }
				system(sMkdirCommand.c_str());

				string sResolution = to_string(pFractal->GetWidth()) + "x" + to_string(pFractal->GetHeight());
				string sQuality = to_string((int)round(pFractal->GetIterations() / (pFractal->GetWidth() * pFractal->GetHeight())));
				string sColor = pFractal->GetColorName();
				
				stringstream stream;
				
				stream << fixed << setprecision(2) << pFractal->GetZoom();
				string sZoom = stream.str();
				stream.str("");

				stream << fixed << setprecision(1) << fStatGamma;
				string sGamma = stream.str();
				stream.str("");

				stream << fixed << setprecision(1) << fStatBrightness;
				string sBrightness = stream.str();
				stream.str("");
				
				string sFilter = to_string(iStatFilter);

				stream << fixed << setprecision(1) << fStatHistWeight;
				string sHistWeight = stream.str();
				stream.str("");
				
				stream << fixed << setprecision(1) << fStatDensityWeight;
				string sDensityWeight = stream.str();
				stream.str("");
				
				stream << fixed << setprecision(1) << fStatSecondBlur;
				string sSecondBlur = stream.str();
				stream.str("");
				
				if (vParts[2] == "experiment") { sFileName = "./render/experiments/" + to_string(iCollection) + "/" + to_string(iCollection) + "_" + sResolution + "_q" + sQuality + "_" + sColor + "_z" + sZoom + "_g" + sGamma + "_b" + sBrightness + "_f" + sFilter; }
				else if (vParts[2] == "personal") { sFileName = "./render/personal/" + to_string(iCollection) + "/" + to_string(iCollection) + "_" + sResolution + "_q" + sQuality + "_" + sColor + "_z" + sZoom + "_g" + sGamma + "_b" + sBrightness + "_f" + sFilter; }
				else if (vParts[2] == "client") { sFileName = "./render/client/" + to_string(iCollection) + "/" + to_string(iCollection) + "_" + sResolution + "_q" + sQuality + "_" + sColor + "_z" + sZoom + "_g" + sGamma + "_b" + sBrightness + "_f" + sFilter; }
				else { sFileName = "./render/official/" + to_string(iCollection) + "/" + to_string(iCollection) + "_" + sResolution + "_q" + sQuality + "_" + sColor + "_z" + sZoom + "_g" + sGamma + "_b" + sBrightness + "_f" + sFilter; }
				
				if (sFilter == "1") { sFileName = sFileName + "_" + sHistWeight + "_" + sDensityWeight + "_" + sSecondBlur; }
				sFileName = sFileName + ".png";
			}
			else { sFileName = vParts[2]; }
			
			//string sCopyCommand = "copy \"./render.png\" \"" + sFileName + "\""; // NOTE: only for windows!!
			string sCopyCommand = "cp \"./render/cache/render.png\" \"" + sFileName + "\"";
			if (iInstanceID != -1)
			{
				sCopyCommand = "cp \"./render/cache/" + to_string(iInstanceID) + "_render.png\" \"" + sFileName + "\"";
			}
			
			cout << ">> Copying to " << sFileName << "..." << endl;

			system(sCopyCommand.c_str());
			return 0;
		}
		else if (vParts[1] == "functions")
		{
			string sFileName = "";
			
			if (vParts.size() == 2) { sFileName = "./collection/" + to_string(iCollection); }
			else { sFileName = vParts[2]; }
			
			pFractal->SaveFunctionCode(sFileName);
			return 0;
		}
		else if (vParts[1] == "trace")
		{
			string sFileName = "";

			if (vParts.size() == 2) { sFileName = "./collection/" + to_string(iCollection); }
			else if (vParts[2] == "experiment")
			{
				string sMkdirCommand = "mkdir -p ./render/experiments/" + to_string(iCollection);
				system(sMkdirCommand.c_str());

				string sResolution = to_string(pFractal->GetWidth()) + "x" + to_string(pFractal->GetHeight());
				string sQuality = to_string((int)round(pFractal->GetIterations() / (pFractal->GetWidth() * pFractal->GetHeight())));
				string sColor = pFractal->GetColorName();
				
				stringstream stream;
				
				stream << fixed << setprecision(2) << pFractal->GetZoom();
				string sZoom = stream.str();
				stream.str("");

				sFileName = "./render/experiments/" + to_string(iCollection) + "/" + to_string(iCollection) + "_" + sResolution + "_q" + sQuality + "_" + sColor + "_z" + sZoom;
			}
			else { sFileName = vParts[2]; }

			pFractal->SaveImageTrace(sFileName);
			return 0;
		}
		else if (vParts[1] == "collection") { StoreCollectionNum(); return 0; }
	}

	// DA MACRO!
	else if (vParts[0] == "produce")
	{
		if (vParts.size() < 10)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: produce [experiment|client|official|personal] [COLLECTION|current] [WIDTH] [HEIGHT] [QUALITY] [COLOR] [ZOOM] [GAMMA] [BRIGHTNESS] {FILTERNUM} {HISTBLURWEIGHT} {DENSITYBLURWEIGHT} {SECONDPASSBLUR}";
			return 1;
		}
		HandleCommand("create 0 0");
		
		if (vParts[2] == "current") { vParts[2] = to_string(iCollection); }

		string sEcho = "echo Running " + vParts[1] + " render of fractal " + vParts[2] + "... \\n " + vParts[3] + "x" + vParts[4] + " \\n Zoom: " + vParts[7] + " \\n Color: " + vParts[6] + " \\n Quality: " + vParts[5] + " \\n Gamma: " + vParts[8] + " \\n Brightness: " + vParts[9];
		if (vParts.size() == 10) { sEcho += " \\n (no blur)"; }
		else 
		{ 
			sEcho += " \\n (blur)"; 
			sEcho += " \\n HistBlur: " + vParts[11] + " \\n DensityBlur: " + vParts[12] + " \\n SecondPassBlur: " + vParts[13];
		}
		HandleCommand(sEcho);

		HandleCommand("collection " + vParts[2]);
		HandleCommand("create " + vParts[3] + " " + vParts[4]);
		HandleCommand("zoom " + vParts[7] + " " + vParts[7]);
		HandleCommand("color " + vParts[6]);
		HandleCommand("init");
		HandleCommand("load functions");
		HandleCommand("solve avg " + vParts[5]);
		
		//if (vParts.size() == 10) { HandleCommand("render " + vParts[8] + " " + vParts[9] + " " + vParts[10]); }
		if (vParts.size() == 10) { HandleCommand("render " + vParts[8] + " " + vParts[9]); }
		//else { HandleCommand("render " + vParts[8] + " " + vParts[9] + " " + vParts[10] + " " + vParts[11] + " " + vParts[12] + " " + vParts[13] + " " + vParts[14]); }
		else { HandleCommand("render " + vParts[8] + " " + vParts[9] + " " + vParts[10] + " " + vParts[11] + " " + vParts[12] + " " + vParts[13]); }

		HandleCommand("save image " + vParts[1]);
		return 0;
	}

	else if (vParts[0] == "echo") // for debugging use!
	{
		cout << endl << "========================================" << endl << endl;
		for (int i = 1; i < vParts.size(); i++)
		{
			if (vParts[i] == "\\n") { cout << endl; }
			else { cout << vParts[i] << " "; }
		}
		cout << endl << endl << "========================================" << endl << endl;
		return 0;
	}
	
	sErrorMsg = "Unrecognized command!";
	return 1;
}
