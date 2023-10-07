#include <iostream>
#include <cmath>
#include <fstream>

const double bohrRad = 0.05292;
const double masa = .067;
const double w = 10/27211.6;
const double step = .1;
const int l = 100; 		// dlugosc w nm
const double L = l/bohrRad;
const int dindex = 20; 	// jest związane z epsilonem, od niego zależy liczba, długości tablicy z falą

class Element{
	public:
		int *indices, cellNumber; // odsyla do numeru globalnego
		Element(int*, int, int, int);
		Element(){}
};

class Node{
	private:
		int global;
		double x,y;
	public:
		Node(){}
		Node(double a, double b, int glo){ x=a; y=b; global=glo; };
		std::string info(){ return " numer globalny " + std::to_string(global+1) + ", wspolrzedne (x,y) = (" + std::to_string(x*bohrRad) + ", " + std::to_string(y*bohrRad) + ")"; }
		double getx(){ return x; }
		double gety(){ return y; }
};

class Simulation{
	private:
		int N, lenNodes, lenItems, lenFala;
		double delta, dxDM1, shift, **fala, ***g;
		Element* items;	// [globalny][lokalny]
		Node* nodes;
	public:
		Simulation(int);
		int coord2indx(double coord){ return round((coord+shift)*dxDM1); }
		void save2file();
		void zad1();
};

int main(){
	Simulation dlaN2(10);
	
	//dlaN2.zad1();
	dlaN2.save2file();
	
	return 0;
}

void Simulation::save2file(){
	std::ofstream destination("wave.txt");
	if(!destination.is_open())
		throw "Plik się nie otworzył";

	for(int i=0; i<lenFala; i++){
		for(int j=0; j<lenFala; j++)
			destination << fala[i][j] << " ";
		destination << std::endl;
	}
}

void Simulation::zad1(){
	for(int i=0; i<lenItems; i++){
		for(int j=0; j<N-1; j++){
			printf("Element %d, numer lokalny %d", i+1, j+1);
			std::cout << nodes[items[i].indices[j]].info() << std::endl;
		}
		std::cout << std::endl;
	}
}

Element::Element(int* toAss, int lenIndices, int lenG, int cell){
	cellNumber = cell;
		  
	indices = new int[lenIndices];

	for(int i=0; i<lenIndices; i++)
		indices[i] = toAss[i];
}

Simulation::Simulation(int n): N{2*n+1}, delta{L/(N-1)}, lenNodes{N*N}, lenFala{dindex*2*n+1}, lenItems{4*n*n}, dxDM1{(lenFala-1)/L}, shift{delta*n} {
	printf("Info o stałych: \ndelta = %lf\nlenNodes = %d\nlenItems = %d\nlenFala (globalna) = %d\ndindex = %d\n", delta, lenNodes, lenItems, lenFala, dindex);

	nodes = new Node[lenNodes];
	items = new Element[lenItems];
	fala = new double*[lenFala];
	
	for(int i=0; i<lenFala; i++)
		fala[i] = new double[lenFala];

	g = new double**[4];

	for(int i=0; i<4; i++){
		g[i] = new double*[dindex+1];
		for(int j=0; j<dindex+1; j++)
			g[i][j] = new double[dindex+1];
	}

	double e1 = -1.0, e2 = -1.0;

	for(int i=0.; i<=dindex; i++, e1 +=.1){
		e2 = -1.0;
		for(int j=0; j<=dindex; j++, e2 +=.1){
			g[0][i][j] = .25*(1-e1)*(1-e2);
			g[1][i][j] = .25*(1+e1)*(1-e2);
			g[2][i][j] = .25*(1-e1)*(1+e2);
			g[3][i][j] = .25*(1+e1)*(1+e2);
		}
	}

	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			double x = delta*j - shift;
			double y = delta*i - shift;
			nodes[i*N + j] = Node(x, y, j + N*i); 						// tu są współrzędne
			fala[i*dindex][j*dindex] = exp(-masa*w/2*(x*x+y*y)); 	// jak mam dostac indeksy?
		}
	}

	int* temp = new int[N-1];

	for(int i=0; i<lenItems; i++){ 						// numer globalny
		int row = i/N;
		for(int j=0; j<N-1; j++)							// numer lokalny
			temp[j] = j>1 ? i+j+3+row : i+j+row;
		items[i] = Element(temp, N-1, lenFala, i);
	}

	delete [] temp;
	double* x = new double[4];
	double* y = new double[4];
	
	int iIndx = 0, jIndx = 0;

	for(int itemIndx=0; itemIndx<lenItems; itemIndx++){ 				// tutaj lecimy po elementach
		for(int j=0; j<4; j++){
			x[j] = nodes[items[itemIndx].indices[j]].getx(); 			// tutaj mamy coordsy węzłów 
			y[j] = nodes[items[itemIndx].indices[j]].gety();
		}
		for(int i=0; i<=dindex; i++){
			for(int j=0; j<=dindex; j++){
				double wspX = 0.0; 
				double wspY = 0.0; 
				for(int loc=0; loc<4; loc++){
					wspX += x[loc]*g[loc][i][j];
					wspY += y[loc]*g[loc][i][j];
				}
				int indxX = coord2indx(wspX);				// tu są indeksy uzyskane z coordinatesów 	
				int indxY = coord2indx(wspY);
				fala[indxX][indxY] = 0;
				for(int temp=0; temp<4; temp++)
					fala[indxX][indxY] += fala[coord2indx(x[temp])][coord2indx(y[temp])] * g[temp][i][j];
			}
		}
	}

	printf("1/dx = %lf\nL = %lf\n", dxDM1, L);
}

// TODO S
// TODO srawdzić czy wszystko gra i czy ma sens dla zad 1.
// TODO Simulation::save2file
