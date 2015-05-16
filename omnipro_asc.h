//***************************************** P L A N S    O M N I P R O     
class plans_opa {

	public:

	vector<dosis1d> dosi1d;

	plans_opa (void) {};
	plans_opa (char* nomarxiu, int transf, punt translation);
};


/////////////////////////////////////////////////////////////////////////

plans_opa::plans_opa (char* nomarxiu, int transf, punt translation) {

	string str;
	ifstream omniproin (nomarxiu, ios::in);

	dosis1d buffer;
	punt buffer_punt;
	buffer.plan = nomarxiu;
	float A, B, C; 

//// Llig distribucions
	float num1d; 
	omniproin >> str >> num1d;

	for (int i = 0; i< num1d; i++) {

		getline(omniproin, str, '=');
		buffer.beam = "beam_" + i;
		dosi1d.push_back (buffer);

		do {
						
			omniproin >> A >> B >> C >> buffer_punt.dosi >> str; 
			
			if (transf == 1) {	
				buffer_punt.x = 0.1 * B;
				buffer_punt.y = 0.1 * A;
				buffer_punt.z = -0.1 * C;
			}
			else if (transf == 2) {	
				buffer_punt.x = -0.1 * B;
				buffer_punt.y = -0.1 * A;
				buffer_punt.z = -0.1 * C;
			}
			else {	
				buffer_punt.x = 0.1 * A;
				buffer_punt.y = 0.1 * B;
				buffer_punt.z = -0.1 * C;
			};

			buffer_punt.x += translation.x;
			buffer_punt.y += translation.y;
			buffer_punt.z += translation.z;
			dosi1d.at(i).punts.push_back (buffer_punt);

		}while (str == "=");
	};
};
