const float DMAX = 1.0; //distància màx. en cm del punt de ref.(ideal) al punt de normalització (mesurat)
//***************************************** P U N T S
struct punt {
	float x;
	float y;
	float z;
	float dosi;
};

//***************************************** D O S I S 1 D
class dosis1d {

	public:

	string plan, beam;
	vector<punt> punts;
	float Dmin, Dmax;

	dosis1d (void) {};
	bool normalitzar (punt ref);
	void sortida (char* nomarxiu, string tipus);
};

/////////////////////////////////////////////////////////////////////////

bool dosis1d::normalitzar (punt ref) {

	//// Dosi de referència

	float d, dmin, dosiref;
	int punt_norm = 0;
	dmin = DMAX;
	for (int i = 0; i< punts.size(); i++) {
		d = (punts.at(i).x - ref.x) * (punts.at(i).x - ref.x);
		d += (punts.at(i).y - ref.y) * (punts.at(i).y - ref.y);
		d += (punts.at(i).z - ref.z) * (punts.at(i).z - ref.z);
		d = sqrt(d);
		if (d < dmin) {
			punt_norm = i;
			dmin = d;
		};  
	};
	if (d == DMAX) return false;
	else dosiref = punts.at(punt_norm).dosi;

	//// Normalització

	for (int i = 0; i< punts.size(); i++)     punts.at(i).dosi /= dosiref;
	return true;
};

/////////////////////////////////////////////////////////////////////////

void dosis1d::sortida (char* nomarxiu, string tipus) {

	ofstream surt (nomarxiu, ios::out);
	float d;

	Dmin = 1000;
	Dmax = 0; 
	
	for (int i=0; i< punts.size(); i++) {

		if ((punts.at(i).dosi) > 0.0) {
			if (tipus == "XPRO")    d = punts.at(i).x;
			if (tipus == "YPRO")    d = punts.at(i).y;
			if (tipus == "PDD")    d = -1.0 * punts.at(i).z;
			surt<<d<<"    "<<punts.at(i).x<<"    "<<punts.at(i).y<<"    "<<punts.at(i).z<<"    "<<punts.at(i).dosi<<endl; // x, y, z, dosi

			if (d < Dmin)    Dmin = d;
			if (d > Dmax)    Dmax = d;
		};
	};

	surt.close();
};	

/////////////////////////////////////////////////////////////////////////

void calcgamma (dosis1d dist1, dosis1d dist2, dosis1d& gamma, float toldosi, float toldist) {

	float deltax, deltay, deltaz, deltadosi, gammagran;
	punt buffer_punt;
	
	for (int i=0; i<dist1.punts.size(); i++) {
		buffer_punt.x = dist1.punts.at(i).x;
		buffer_punt.y = dist1.punts.at(i).y;
		buffer_punt.z = dist1.punts.at(i).z;
		buffer_punt.dosi = 100;
		gamma.punts.push_back (buffer_punt);

		for (int j=0; j<dist2.punts.size(); j++) {
			deltax = dist1.punts.at(i).x - dist2.punts.at(j).x;
			deltax = (deltax*deltax) / (toldist*toldist);
			deltay = dist1.punts.at(i).y - dist2.punts.at(j).y;
			deltay = (deltay*deltay) / (toldist*toldist);
			deltaz = dist1.punts.at(i).z - dist2.punts.at(j).z;
			deltaz = (deltaz*deltaz) / (toldist*toldist);
			deltadosi = dist1.punts.at(i).dosi - dist2.punts.at(j).dosi;
			deltadosi = (100*deltadosi*100*deltadosi) / (toldosi*toldosi);
			gammagran = deltax + deltay + deltaz + deltadosi;
			gammagran = sqrt(gammagran);
			if (gammagran < gamma.punts.at(i).dosi)    gamma.punts.at(i).dosi = gammagran; 
		};
	};
};

/////////////////////////////////////////////////////////////////////////

void write_gnuplot (char* nomarxiu, string png, string eclipse, string omnipro, string gamma, float toldosi, float toldist, float Dmin, float Dmax) {

	ofstream surt (nomarxiu, ios::out);
	surt<<"set terminal png size 1500,1000"<<endl;
	surt<<"set output \""<<png<<"\" "<<endl;
	surt<<"set title \""<<png<<"\" "<<endl;
	surt<<"set autoscale"<<endl;
	surt<<"set xrange ["<<Dmin<<" : "<<Dmax<<"]"<<endl;
	surt<<"set y2range [0.0 : 2.0]"<<endl;
	surt<<"set xlabel \"distance (cm)\""<<endl;
	surt<<"set ylabel \"relative dose\""<<endl;
	surt<<"set y2label \"gamma\""<<endl;
	surt<<"set ytics nomirror"<<endl;
	surt<<"set y2tics"<<endl;
	surt<<"set grid"<<endl;
	surt<<"unset log "<<endl;

	
	surt<<"plot \""<<eclipse<<"\" using 1:5 with lines axis x1y1 title \"Eclipse\", \""
			<<omnipro<<"\" using 1:5 with lines axis x1y1 title \"OmniPro\", \""
//			<<gamma + "1"<<"\" using 1:5 with lines axis x1y2 title \"Gamma 1\% 1mm\", \""
//			<<gamma + "2"<<"\" using 1:5 with lines axis x1y2 title \"Gamma 2\% 2mm\", \""
			<<gamma + "2"<<"\" using 1:5 with lines axis x1y2 title \"Gamma 2\% 2mm\""<<endl;

	surt.close();

	ofstream batch (BATCH, ios::app);
	batch<<"load \""<<nomarxiu<<"\""<<endl;
	batch.close();
};

			
