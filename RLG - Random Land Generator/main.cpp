#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <array>
#include <string>

#include "Cell.hpp"

// Consts
static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;
static const int MAX_CELL_X = 180;
static const int MAX_CELL_Y = 120;

// Other variables
std::array<std::array<Cell, MAX_CELL_X>, MAX_CELL_Y> cells;
int board[MAX_CELL_Y][MAX_CELL_X];
int next[MAX_CELL_Y][MAX_CELL_X];

// Global variables
sf::Color cellColor;
float g_cellSize = 5.0f;
int g_density = 55;
int g_iterations = 0;

// Functions
void InitGame(sf::RenderWindow& window);
void UpdateGame(sf::RenderWindow& window);
void RenderGame(sf::RenderWindow& window);

int main()
{
	// Creating the window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "RLG - Random Land Generator");
	window.setFramerateLimit(60);

	InitGame(window);

	// Main game loop
	while (window.isOpen())
	{
		sf::Event mainEvent;

		// Processing events
		while (window.pollEvent(mainEvent))
		{
			if (mainEvent.type == sf::Event::Closed)
				window.close();
		}

		UpdateGame(window);

		RenderGame(window);
	}
}

void InitGame(sf::RenderWindow& window)
{
	// Variables init
	cellColor = sf::Color::Green;
	int randNum = 0;

	// Cells init
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells.at(0).size(); j++)
		{
			// Getting a random number to set the initial colors
			randNum = rand() % 100;

			if (randNum > g_density) board[i][j] = 0;
			else board[i][j] = 1;
		
			// Cells init
			cells.at(i).at(j).Init(sf::Vector2f(j * g_cellSize, i * g_cellSize), g_cellSize, cellColor, true);
			cells.at(i).at(j).SetShape();
		}
	}
}

void UpdateGame(sf::RenderWindow& window)
{
	// Spawning the cells again
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		InitGame(window);

	// Updating the iterations
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) g_iterations += 2;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) g_iterations -= 2;

	// Updating the density
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) g_density++;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) g_density--;

	// Iterating over the algorithim a specified amount of time
	for (int t = 0; t < g_iterations; t++)
	{
		// Going through the cells(Starting from 1 to skip the bounds because I am lazy)
		for (int i = 1; i < cells.size() - 1; i++)
		{
			for (int j = 1; j < cells.at(0).size() - 1; j++)
			{
				int neighborsWaterCount = 0;

				// Going through the cell's neighbors
				for (int y = i - 1; y <= i + 1; y++)
				{
					for (int x = j - 1; x <= j + 1; x++)
					{
						if(y != i || x != j)
							neighborsWaterCount += board[y][x];
					}

					/* Rules: */
					// If there are more than four water neighbors, turn the current tile into water
					if (neighborsWaterCount > 4) next[i][j] = 0;
					// Otherwise, if there are less than 4 water neighbors, turn the current tile into land
					else if (neighborsWaterCount <= 4) next[i][j] = 1;
				}
			}
		}

		// Starting the next generation
		memcpy(board, next, sizeof(next));
	}
}

void RenderGame(sf::RenderWindow& window)
{
	// Drawing stuff 
	window.clear(sf::Color::Blue);

	// Cells draw
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells.at(0).size(); j++)
		{ 
			if (board[i][j] == 1) cells.at(i).at(j).shape.setFillColor(sf::Color::Green);
			else cells.at(i).at(j).shape.setFillColor(sf::Color::Blue);

			// Cells render
			window.draw(cells.at(i).at(j).shape);
		}
	}

	// UI draw
	sf::Font font;
	font.loadFromFile("aerial.ttf");

	sf::Text densityText("Density: " + std::to_string(g_density), font, 20);
	densityText.setPosition(sf::Vector2f(10.0f, window.getSize().y - 30.0f));
	densityText.setFillColor(sf::Color::Red);
	window.draw(densityText);

	sf::Text iterationText(" Iterations: " + std::to_string(g_iterations), font, 20);
	iterationText.setPosition(210.0f, window.getSize().y - 30.0f);
	iterationText.setFillColor(sf::Color::Red);
	window.draw(iterationText);

	window.display();
}
