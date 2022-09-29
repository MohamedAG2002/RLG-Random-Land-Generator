#pragma once

#include <SFML/Graphics.hpp>

struct Cell
{
	sf::Vector2f position;
	float size;
	sf::Color color;
	bool isActive;
	sf::RectangleShape shape;

	void Init(const sf::Vector2f& pos, const float& s, const sf::Color& c, const bool& active)
	{
		position = pos;
		size = s;
		color = c;
		isActive = active;
	}

	void SetShape()
	{
		shape.setSize(sf::Vector2f(size, size));
		shape.setPosition(sf::Vector2f(position.x, position.y));
		shape.setFillColor(color);
	}

	void Render(sf::RenderWindow& window)
	{
		window.draw(shape);
	}
};