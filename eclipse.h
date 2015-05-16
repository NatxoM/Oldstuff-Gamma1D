//***************************************** P L A N S    E C L I P S E 
class plans_ecl {

	public:

	vector<dosis1d> dosi1d;

	plans_ecl (void) {};
	plans_ecl (char* nomarxiu, string tipus);
};


/////////////////////////////////////////////////////////////////////////

plans_ecl::plans_ecl (char* nomarxiu, string tipus) {

//// Llig patient, plan, posició inicial i posició final de les distribucions
	string str, str_patient, str_plan;
	char cadena[50];  

	ifstream eclipsein (nomarxiu, ios::in);

	getline(eclipsein, str_patient);
	getline(eclipsein, str, ':');
	eclipsein >> str_plan;

	
	float Aini, Bini, Cini, Afi, Bfi, Cfi;
	getline(eclipsein, str, '(');
	eclipsein.getline(cadena,10,',');
	Aini = strtof(cadena,0);
	eclipsein.getline(cadena,10,',');
	Bini = strtof(cadena,0);
	eclipsein.getline(cadena,10,')');
	Cini = strtof(cadena,0);
	getline(eclipsein, str, '(');
	eclipsein.getline(cadena,10,',');
	Afi = strtof(cadena,0);
	eclipsein.getline(cadena,10,',');
	Bfi = strtof(cadena,0);
	eclipsein.getline(cadena,10,')');
	Cfi = strtof(cadena,0);

//// Entra plan i beam de totes les ditribucions

	dosis1d buffer;
	buffer.plan = str_plan;
	do {
		eclipsein >> buffer.beam;
		dosi1d.push_back (buffer);
	}while(buffer.beam!="Total");

//// Entra les dosis de cada punt per cada distribució
	punt buffer_punt;
	buffer_punt.x = 0.0;
	buffer_punt.y = 0.0;
	buffer_punt.z = 0.0;
	eclipsein >> str;
	do {
		for (int i = 0; i< dosi1d.size(); i++) {
			eclipsein >> buffer_punt.dosi;
			dosi1d.at(i).punts.push_back (buffer_punt);
		};
		eclipsein >> str;
	}while(!eclipsein.eof());

	dosi1d.pop_back();
//// Entra x,y,z de cada punt per cada distribució (IEC (cm))
	float A, B, C;
	for (int i = 0; i< dosi1d.size(); i++) {
		for (int j = 0; j< ( dosi1d.at(i).punts.size() ); j++) {

			A = Aini + j * ((Afi - Aini) / (dosi1d.at(i).punts.size() - 1));
			B = Bini + j * ((Bfi - Bini) / (dosi1d.at(i).punts.size() - 1));
			C = Cini + j * ((Cfi - Cini) / (dosi1d.at(i).punts.size() - 1));

			dosi1d.at(i).punts.at(j).x = A;
			dosi1d.at(i).punts.at(j).y = B;
			dosi1d.at(i).punts.at(j).z = C;
		};
	};
};
