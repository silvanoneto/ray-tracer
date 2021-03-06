#include "objeto.hpp"

//Esfera
Esfera::Esfera(vec pCentro, double pRaio, uvec pCor, uvec pSpecCor, double pShineness){
	centro = pCentro;
	raio = pRaio;
	cor = pCor;
	specCor = pSpecCor;
	shineness = pShineness;
}

uvec Esfera::getDifuseColor(){ return cor; }
uvec Esfera::getSpecColor(){ return specCor; }
double Esfera::getShineness(){ return shineness; }

double Esfera::tVal(vec pontoProj, vec diretor){
	double contRaiz = (dot(diretor, pontoProj-centro))*(dot(diretor,
					pontoProj-centro))-(dot(diretor,diretor))*(dot(pontoProj
					-centro,pontoProj-centro)-(raio*raio));
					
	if(contRaiz<0){ return -1.0; }
	
	double t1 = (dot((-1.0)*diretor, pontoProj-centro) + sqrt(contRaiz))/
				dot(diretor,diretor);
	double t2 = (dot((-1.0)*diretor, pontoProj-centro) - sqrt(contRaiz))/
				dot(diretor,diretor);
	
	if(t2 < t1){ return t1; } else { return t2; }
}

vec Esfera::getNormal(vec pontoProj, vec diretor, double t){
	vec ponto = pontoProj + diretor*t;
	
	return (ponto - centro) / raio;
}

//Triângulo

Triangulo::Triangulo(vec pA, vec pB, vec pC, uvec pCor, uvec pSpecCor, 
	double pShineness, vec pNA, vec pNB, vec pNC){
		
	a = pA;
	b = pB;
	c = pC;
	cor = pCor;
	specCor = pSpecCor;
	shineness = pShineness;
	nA = pNA;
	nB = pNB;
	nC = pNC;
	
}

/*
double Triangulo::tVal(vec diretor){
	vec c_a = c-a;
	vec b_a = b-a;
	vec o_a = diretor-a;
	
	mat A; A.insert_cols(0, c_a); A.insert_cols(1, b_a);
	A.insert_cols(2, (-1.0)*diretor); double detA = det(A);
	
	mat B; B.insert_cols(0, o_a); B.insert_cols(1, b_a);
	B.insert_cols(2, (-1.0)*diretor); double beta = det(B) / detA;
}*/

double Triangulo::tVal(vec pontoProj, vec diretor){
	vec a_b = a-b;
	vec a_c = a-c;
	vec a_e = a-pontoProj;
	
	double gama = this->getGama(pontoProj, diretor);
	
	if(gama < 0.0 || gama > 1.0){ return -1.0; }
	
	double beta = this->getBeta(pontoProj, diretor);
	
	if(beta < 0.0 || beta > 1.0 - gama){ return -1.0; }
	
	mat T; T.insert_cols(0, a_b); T.insert_cols(1, a_c);
	T.insert_cols(2, a_e); double t = det(T) / this->getA(diretor);
	
	if(t < 0.0){ return -1.0; }
	
	return t;

}

double Triangulo::getA(vec diretor){
	vec a_b = a-b;
	vec a_c = a-c;
	
	mat A;
	A.insert_cols(0, a_b); 
	A.insert_cols(1, a_c);
	A.insert_cols(2, diretor);
	
	return det(A);
}

double Triangulo::getGama(vec pontoProj, vec diretor){
	vec a_b = a-b;
	vec a_e = a-pontoProj;
	
	mat G;
	G.insert_cols(0, a_b);
	G.insert_cols(1, a_e);
	G.insert_cols(2, diretor);
	
	return det(G) / this->getA(diretor);
}

double Triangulo::getBeta(vec pontoProj, vec diretor){
	vec a_e = a-pontoProj;
	vec a_c = a-c;
	
	mat B;
	B.insert_cols(0, a_e);
	B.insert_cols(1, a_c);
	B.insert_cols(2, diretor);
	
	return det(B) / this->getA(diretor);
}

vec Triangulo::getNormal(vec pontoProj, vec diretor, double t){
	
	if(nA.is_empty() == 0){
		nA = cross((b-a), (c-a));
		nB = cross((a-b), (c-b));
		nC = cross((a-c), (b-c));
	}
	
	double beta = this->getBeta(pontoProj, diretor);
	double gama = this->getGama(pontoProj, diretor);
	double alfa = 1.0 - (beta+gama);
	
	vec normal = nA*alfa + nB*beta + nC*gama;
	
	return normal;
}

uvec Triangulo::getDifuseColor(){ return cor; }
uvec Triangulo::getSpecColor(){ return specCor; }
double Triangulo::getShineness(){ return shineness; }
