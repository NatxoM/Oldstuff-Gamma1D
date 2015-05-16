#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

const char ENTRADA[50] = "verification.in";
string FOLDERIN = "../Verification/";
string FOLDEROUT = "../Verification/gamma/";
char BATCH[50] = "batch.gnu"; 

#include "analisi.h"
#include "eclipse.h"
#include "omnipro_asc.h"

int main (void) {

	ofstream batch (BATCH, ios::out);
	batch.close();

	ifstream entrades (ENTRADA, ios::in);

	string str;
	char cadena[250];
	getline(entrades, str);

	int transf;
	string folder1, file1, tipus, folder2, file2;
	float toldosi, toldist, refX, refY, refZ, transX, transY, transZ;

	do {

	//// Dades d'entrada

		entrades >> folder1 >> file1 >> tipus >> folder2 >> file2 >> transf >> refX >> refY >> refZ >> transX >> transY >> transZ;

	//// Llig plans

		str = FOLDERIN + folder1 + "/" + file1;
		strcpy (cadena, str.c_str());
		plans_ecl plan_ecl (cadena, tipus);
		cout<<cadena<<endl;

		punt translation;
		translation.x = transX;
		translation.y = transY;
		translation.z = transZ;

		str = FOLDERIN + folder2 + "/" + file2;
		strcpy (cadena, str.c_str());
		plans_opa plan_opa (cadena, transf, translation);

	//// Normalització de totes les ditribucions

		bool flag;
		punt ref;
		ref.x = refX;
		ref.y = refY;
		ref.z = refZ;
	
		for (int i=0; i<(plan_ecl.dosi1d.size()); i++) {
			flag = plan_ecl.dosi1d.at(i).normalitzar (ref);
		};
	
		for (int i=0; i<(plan_opa.dosi1d.size()); i++) {
			 plan_opa.dosi1d.at(i).normalitzar (ref);
		};
	
	
	//// Gamma

		int numgammes = plan_ecl.dosi1d.size();
		dosis1d gamma[numgammes*3];

		float Dmin, Dmax;
		string str_png, str_eclipse, str_omnipro, str_gamma;

		for (int i=0; i<(plan_ecl.dosi1d.size()); i++) { 

			calcgamma (plan_opa.dosi1d.at(i), plan_ecl.dosi1d.at(i), gamma[i], 1, 0.1);
			calcgamma (plan_opa.dosi1d.at(i), plan_ecl.dosi1d.at(i), gamma[numgammes + i], 2, 0.2);
			calcgamma (plan_opa.dosi1d.at(i), plan_ecl.dosi1d.at(i), gamma[2 * numgammes + i], 3, 0.3);
	
			ostringstream oss;
			oss << refZ << "cm";

			//// Sortida
			str_eclipse = FOLDEROUT + tipus + "E_" + plan_ecl.dosi1d.at(i).plan + "_" + plan_ecl.dosi1d.at(i).beam + oss.str() + ".pts";
			strcpy (cadena, str_eclipse.c_str());
			plan_ecl.dosi1d.at(i).sortida(cadena,tipus);
	
			str_omnipro = FOLDEROUT + tipus + "O_" + plan_ecl.dosi1d.at(i).plan + "_" + plan_ecl.dosi1d.at(i).beam + oss.str() + ".pts";
			strcpy (cadena, str_omnipro.c_str());
			plan_opa.dosi1d.at(i).sortida(cadena,tipus);
	
			str_gamma = FOLDEROUT + tipus + "G_" + plan_ecl.dosi1d.at(i).plan + "_" + plan_ecl.dosi1d.at(i).beam + oss.str() + ".pts";
			str = str_gamma + "1";
			strcpy (cadena, str.c_str());
			gamma[i].sortida(cadena,tipus);
			str = str_gamma + "2";
			strcpy (cadena, str.c_str());
			gamma[numgammes + i].sortida(cadena,tipus);
			str = str_gamma + "3";
			strcpy (cadena, str.c_str());
			gamma[2 * numgammes + i].sortida(cadena,tipus);

			str_png = FOLDEROUT + tipus + plan_ecl.dosi1d.at(i).plan + "_" + plan_ecl.dosi1d.at(i).beam + oss.str() + ".png";
	
			str = FOLDEROUT + tipus + plan_ecl.dosi1d.at(i).plan + "_" + plan_ecl.dosi1d.at(i).beam + oss.str() + ".gnu";
			strcpy (cadena, str.c_str());

			if (plan_opa.dosi1d.at(i).Dmin > plan_ecl.dosi1d.at(i).Dmin)    Dmin = plan_opa.dosi1d.at(i).Dmin;
			else    Dmin = plan_ecl.dosi1d.at(i).Dmin;
			if (plan_opa.dosi1d.at(i).Dmax < plan_ecl.dosi1d.at(i).Dmax)    Dmax = plan_opa.dosi1d.at(i).Dmax;
			else    Dmax = plan_ecl.dosi1d.at(i).Dmax;

			write_gnuplot (cadena, str_png, str_eclipse, str_omnipro, str_gamma, toldosi, toldist, Dmin, Dmax);
		};

	    plan_ecl.dosi1d.clear();
	    plan_opa.dosi1d.clear();

	}while(!entrades.eof());
};


