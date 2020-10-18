#ifndef VIEW_H
#define VIEW_H
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "pointWithHistory.hxx"
namespace view {
auto windowEvent(sf::Event const &event, sf::RenderWindow &window) -> void;

auto createFieldView(unsigned int width, size_t elementsPerRow) -> std::vector<sf::RectangleShape>;

auto drawBoard(sf::RenderWindow &window, std::vector<game::PointWithHistory<unsigned short int>> const &points, sf::Clock &deltaClock)
    -> void;

auto updateFieldView(std::vector<sf::RectangleShape> &fieldView, std::vector<int> const &field) -> void;

auto changeWindowEvent(std::vector<sf::Event> const &events, sf::RenderWindow &window) -> void;

auto extractEvent(sf::RenderWindow &window)->std::vector<sf::Event>;
} // namespace view
#endif // VIEW_H
