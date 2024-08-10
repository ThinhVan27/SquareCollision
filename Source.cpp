#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <cmath>
#include <random>
#include <vector>
using namespace std;

const sf::Color blue(135, 206, 250);

mt19937_64 rd(chrono::steady_clock::now().time_since_epoch().count());

const double FPS = 10000;
const int w = 1500;
const int h = 1000;

static long long Rand(long long l, long long r) {
	return l + rd() % (r - l + 1);
}
sf::RenderWindow window(sf::VideoMode(w, h), "My window");
//videomode(width, height)

class myRect {
private:
	sf::RectangleShape& obj;
	double velocity;
public:
	myRect(sf::RectangleShape& obj,double velocity = 0) : obj(obj) {
		this->velocity = (velocity/FPS);
	}
	sf::Vector2f getPosition() {
		return obj.getPosition();
	}

	void setV(double x) {
		this->velocity = x;
	}

	void draw() const{
		window.draw(obj);
	}
	friend void update(myRect& obj);
	friend void collision(myRect& obj1, myRect& obj2);
};
int cnt = 0;
void update(myRect& object) {
	const sf::Vector2f pos = object.obj.getPosition();
	object.obj.setPosition(sf::Vector2f(pos.x+object.velocity,pos.y));
	if (object.obj.getPosition().x <= 100) {
		cnt++;
		object.setV(object.velocity * (-1));
		object.obj.setPosition(sf::Vector2f(100, pos.y));
	}
}

void collision(myRect& obj1, myRect& obj2) {
	if (obj1.getPosition().x + 100 >= obj2.getPosition().x) {
		cnt++;
		double m1 = 1;
		double m2 = 10000;
		double v1 = obj1.velocity;
		double v2 = obj2.velocity;
		double p = m1 * v1 + m2 * v2;
		double V2 = (2 * m1 / (m1 + m2)) * v1 + ((m2 - m1) / (m1 + m2)) * v2;
		double V1 = (p - m2 * V2) / m1;

		obj1.setV(V1);
		obj2.setV(V2);

	}
}


void subDraw() {

	for (int i = 100; i <= 900; i += 20) {
		sf::RectangleShape line;
		line.setPosition(sf::Vector2f(100,i));
		line.setSize(sf::Vector2f(0, 25));
		line.setFillColor(sf::Color(245, 245, 245));
		line.setOutlineColor(sf::Color(245, 245, 245));
		line.setOutlineThickness(2);
		line.setRotation(135);

		window.draw(line);
	}

	for (int i = 120; i <= 1400; i += 20) {
		sf::RectangleShape line;
		line.setPosition(sf::Vector2f(i, 900));
		line.setSize(sf::Vector2f(0, 25));
		line.setFillColor(sf::Color(245, 245, 245));
		line.setOutlineThickness(2);
		line.setRotation(45);

		window.draw(line);
	}
	

}
void drawFrame() {
	window.setFramerateLimit(FPS);
	sf::RectangleShape vertical;
	vertical.setSize(sf::Vector2f(0, 800));
	vertical.setFillColor(sf::Color(245, 245, 245));
	vertical.setOutlineThickness(2);
	vertical.setPosition(100, 100);
	window.draw(vertical);

	subDraw();

	sf::RectangleShape horizontal;
	horizontal.setSize(sf::Vector2f(1350, 0));
	horizontal.setFillColor(sf::Color(245, 245, 245));
	horizontal.setOutlineThickness(2);
	horizontal.setPosition(100, 900);
	window.draw(horizontal);
}
int main() {
	window.setFramerateLimit(FPS);
	sf::RectangleShape largeSquare;
	largeSquare.setSize(sf::Vector2f(300, 300));
	largeSquare.setFillColor(blue);
	largeSquare.setPosition(500, 600);

	sf::RectangleShape smallSquare;
	smallSquare.setSize(sf::Vector2f(100, 100));
	smallSquare.setFillColor(blue);
	smallSquare.setPosition(300, 800);
	
	myRect rect1(largeSquare,-500.f);
	myRect rect2(smallSquare,0);

	sf::Font font;
	font.loadFromFile("arial.ttf");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		

		// Create a text
		sf::Text text("Collisions: " + to_string(cnt), font);
		text.setPosition(sf::Vector2f(1100, 10));
		text.setCharacterSize(40);
		text.setStyle(sf::Text::Bold);
		text.setFillColor(sf::Color::Green);

		// Draw it
		window.draw(text);
		rect1.draw();
		rect2.draw();
		drawFrame();

		update(rect1);
		update(rect2);
		collision(rect2, rect1);

		window.display();
	}
}