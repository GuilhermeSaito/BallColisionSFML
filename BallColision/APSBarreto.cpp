#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <math.h>	//NÃO ESQUECER DE COLOCAR O -lm NO FINAL, PARA COMPILAR


/*------------Vetor de Struct, que tem as posições e velocidades de cada bolinha----------------*/
typedef struct 
{
	float posX;
	float posY;

	float velX;
	float velY;
}BallsProprieties;



/*-----------------------Functions------------------------*/
void updatePosBall(sf::RenderWindow* window, std::vector<sf::CircleShape>* balls, BallsProprieties* propBall, int numberBall);
void wallColision(BallsProprieties* propBall, int numberBall, int hightWindow, int lenghtWindow);
void ballColision(std::vector<sf::CircleShape>* balls, BallsProprieties* propBall, int numberBall);
int centroDa(int b);



/*----------------Variável global para os raios da bola---------------------*/
//Da para colocar para a pessoa setar o raio, mais estou com preguiça agr
int radius = 30;
//Considera que cada bola tem... 1kg.
float kineticEnergy, linearMomentum;
float energyUpper = 0, energyDown = 10000;
float momentumUpper = 0, momentumDowm = 10000;


int main()
{
	int i;
	int lenghtWindow, hightWindow, numberBall;

	srand(time(NULL));

	do
	{
		printf("Informe a Altura e Largura da tela: ");
		scanf("%d %d",&hightWindow, &lenghtWindow);
		printf("Inform o Numero de bolinhas: ");
		scanf("%d",&numberBall);
	}while(hightWindow <= 0 || lenghtWindow <= 0 || numberBall <= 0);//Só para n deixar negativo ou = 0 a altura, largura e a quantidade de bolas

	//Aqui que cria a tela, da o nome da "alma do programa de window. vai ((largura, altura), nome do conteiner, tipo do conteiner)"
	sf::RenderWindow window(sf:: VideoMode(lenghtWindow, hightWindow), "Barretao'sAPS" , sf::Style::Default);

	//Cria um vetor do tipo Circle Shape, com o nome da variavel de balls(com o tamanho do vetor).
	std::vector<sf::CircleShape> balls(numberBall);
	BallsProprieties* propBall = (BallsProprieties*)malloc(numberBall * sizeof(BallsProprieties));//Faz o vetor do struct, q tem as velocidades e espaços de cada bolinha

	sf::Texture texture;
	if(!texture.loadFromFile("Image5.jpg"))
	{
		printf("Error Loading image.\n\n");
		return -1;
	}


	//Seta todas as velocidades de cada bolinha, posição inicial e o raio de todas as bolinhas
	for (i=0; i<numberBall; i++)
	{
		balls[i].setRadius(radius);
		propBall[i].posX = (((rand() % 1200) + 1) / 3);
		propBall[i].posY = ((rand() % 1200) + 1) / 3;
		propBall[i].velX = (((rand() % 3) + 1) / 3) + 0.1;
		propBall[i].velY = (((rand() % 3) + 1) / 3) + 0.1;

		balls[i].setTexture(&texture);
	}


	//Evento Principal
	while (window.isOpen())
	{

		sf::Event event;

		//Se for apertado qualquer coisa, ter passado o mouse na tela, qualquer tipo de input
		if(window.pollEvent(event))
			if(event.type == sf::Event::Closed)//Se for apertado o X em cima da tela, fecha o conteiner.
				window.close();

		//Da um clear na tela.
		window.clear();

		//Atualiza as posições de todas as bolinhas, atualiza pela velocidade de cada 1.
		updatePosBall(&window, &balls, propBall, numberBall);


		window.display();

		//Calculo da colisão das bolinhas com os cantos do conteiner
		wallColision(propBall, numberBall, hightWindow, lenghtWindow);

		//Calculo da colisão das bolinhas com outras as bolinhas
		ballColision(&balls, propBall, numberBall);
	}

	printf("\n\n\nMaior Energia: %.3f\nMenor Energia: %.3f\nMaior Momento: %.3f\nMenor Momento: %.3f\n\n\n", energyUpper, energyDown, momentumUpper, momentumDowm);

	return 0;
}






void updatePosBall(sf::RenderWindow* window, std::vector<sf::CircleShape>* balls, BallsProprieties* propBall, int numberBall)
{
	int i;
	//sf::Text text;

	kineticEnergy = 0;
	linearMomentum = 0;

	//text.setFont(font);
	//text.setString("")

	for (i=0; i<numberBall; i++)
	{
		propBall[i].posX += propBall[i].velX;
		propBall[i].posY += propBall[i].velY;

		//Atualiza a posição das bolinhas conforme a velocidade de cada uma.
		(*balls)[i].setPosition(propBall[i].posX, propBall[i].posY);

		//Fórmula da Energia Cinética: (m*V²)/2, aqui é a energia cinética tota.
		kineticEnergy = kineticEnergy + abs(sqrt((sqrt(pow(propBall[i].velX,2) + pow(propBall[i].velY, 2)), 2) / 2));
		//Fórmula do Momento Linear: m*V;
		linearMomentum = linearMomentum + abs((sqrt(pow(propBall[i].velX,2) + pow(propBall[i].velY, 2))));

		//Desenha cada bolinha na tela.
		window->draw((*balls)[i]);
	}
	printf("Energia Cinetica: %.3f\nMomento Linear: %.3f\n\n", kineticEnergy, linearMomentum);

	if(energyUpper < kineticEnergy)
		energyUpper = kineticEnergy;
	if(energyDown > kineticEnergy)
		energyDown = kineticEnergy;
	if(momentumUpper < linearMomentum)
		momentumUpper = linearMomentum;
	if(momentumDowm > linearMomentum)
		momentumDowm = linearMomentum;

}






void wallColision(BallsProprieties* propBall, int numberBall, int hightWindow, int lenghtWindow)
{
	int i;

	for (i=0; i<numberBall; i++)
	{
		if((propBall[i].posX <= 0) || (propBall[i].posX + 2 * radius >= lenghtWindow))
			propBall[i].velX *= -1;

		if((propBall[i].posY <= 0) || (propBall[i].posY + 2 * radius >= hightWindow))
			propBall[i].velY *= -1;
	}
}




void ballColision(std::vector<sf::CircleShape>* balls, BallsProprieties* propBall, int numberBall)
{
	int i, j;

	for (i=0; i<numberBall; i++)
		for (j=i+1; j<numberBall; j++)
		{
			if((*balls)[i].getGlobalBounds().intersects((*balls)[j].getGlobalBounds()))
			{
				//eixo de colisao
			    double colisao_x = centroDa((propBall[i].posX) - centroDa(propBall[j].posX));
			    double colisao_y = centroDa((propBall[i].posY) - centroDa(propBall[j].posY));

			    double colisao = colisao_x*colisao_x + colisao_y*colisao_y;

			    //velocidades projetadas
			    double pvx1 = ((propBall[i].velX * colisao_x) + (propBall[i].velY * colisao_y)) * colisao_x / colisao;
			    double pvy1 = ((propBall[i].velX * colisao_x) + (propBall[i].velY * colisao_y)) * colisao_y / colisao;
			    double pvx2 = ((propBall[j].velX * colisao_x) + (propBall[j].velY * colisao_y)) * colisao_x / colisao;
			    double pvy2 = ((propBall[j].velX * colisao_x) + (propBall[j].velY * colisao_y)) * colisao_y / colisao;

			    //inverte as velocidades que estao no EIXO de colisao
			    propBall[i].velX -= (pvx1 - pvx2);
			    propBall[i].velY -= (pvy1 - pvy2);
			    propBall[j].velX -= (pvx2 - pvx1);
			    propBall[j].velY -= (pvy2 - pvy1);

			    //separa bolas grudadas
			    if(colisao_x != 0 && colisao_y != 0)
			    {
			        //soma ou diminui um pixel na posiçao das bolas
			        propBall[i].posX += colisao_x / abs(colisao_x);
			        propBall[i].posY += colisao_y / abs(colisao_y);

			        propBall[j].posX -= colisao_x / abs(colisao_x);
			        propBall[j].posY -= colisao_y / abs(colisao_y);
				}
			}
		}
}






int centroDa(int b)
{
    return (b + radius);
}