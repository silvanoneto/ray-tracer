#include "imagem.hpp"

int mdc(int n1, int n2);

Imagem::Imagem(int pAltura, int pLargura){
	altura = pAltura;
	largura = pLargura;
	dpi = mdc(pAltura, pLargura);
}

vec Imagem::projToPix(vec ponto){
	mat K = { {dpi*1.0, 0.0, largura/2.0},
						{0.0, (-1.0)*dpi, altura/2.0},
						{0.0, 0.0, 1.0} };
	
	return K*ponto;
}

vec Imagem::pixToProj(vec pixel){
	mat K = { {1.0/dpi, 0.0, largura/(-2.0*dpi)},
						{0.0, (-1.0)/dpi, altura/(2.0*dpi)},
						{0.0, 0.0, 1.0} };
	
	return K*pixel;
}

void Imagem::renderizar(vector<Objeto*> *objetos){
	FILE *img = fopen("imagem.ppm", "w");
	fprintf(img, "P3\n%d %d\n%d\n", largura, altura, 255);
	
	for(int h = 0; h < altura; h++){
		for(int w = 0; w < largura; w++){
			vec pix = { w*1.0, h*1.0, 1.0 };
			vec v = pixToProj(pix);
			
			double tAtual = -1.0;
			Objeto *objetoAtual;
			int size = objetos->size();
			
			for(int i = 0; i < size; i++){
				double t = (*objetos)[i]->tVal(v);
				
				if(t >= 0 && ((t < tAtual) || (tAtual == -1.0))){
					tAtual = t;
					objetoAtual = (*objetos)[i];
				}
			}
				
			if(tAtual == -1.0){
				fprintf(img, "%d %d %d ", 255, 255, 255);
			}else{
				vec normal = objetoAtual->getNormal(v, tAtual);
				uvec cores = objetoAtual->getColor();
				fprintf(img, "%d %d %d ", (int)cores[0], (int)cores[1], (int)cores[2]);
			}
		}
		
		fprintf(img, "\n");
		
	}
	
	fclose(img);
	
}

int mdc(int n1, int n2){
	int z = 1;
	
	while ( z !=0 )
	{
		z = n1 % n2;
		n1 = n2;
		n2 = z;
	}
	
	return n1;
}