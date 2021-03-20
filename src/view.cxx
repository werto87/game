#include "view.hxx"
#include <imgui-SFML.h>
#include <imgui.h>
namespace view {
auto windowEvent(sf::Event const &event, sf::RenderWindow &window) -> void {
  using namespace sf;
  switch (event.type) {
  case Event::Closed: {
    window.close();
    break;
  }
  default: {
    break;
  }
  }
}

auto createFieldView(unsigned int width, size_t elementsPerRow) -> std::vector<sf::RectangleShape> {
  auto elementWidth = static_cast<float>(width / elementsPerRow);
  auto fieldView = std::vector<sf::RectangleShape>(elementsPerRow);
  for (auto &element : fieldView) {
    element = sf::RectangleShape{{elementWidth, elementWidth}};
  }
  for (size_t i = 0; i < fieldView.size(); ++i) {
    fieldView.at(i).setPosition(static_cast<float>(i) * elementWidth, 0);
  }
  return fieldView;
}

auto drawBoard(sf::RenderWindow &window, std::vector<game::PointWithHistory<unsigned short int>> const &points,
               std::pair<unsigned short int, unsigned short int> winningPosition, sf::Clock &deltaClock) -> void {
  ImGui::SFML::Update(window, deltaClock.restart());
  window.clear();
  for (size_t i = 0; i < points.size(); ++i) {
    auto const &pointPosition = points.at(i).getPositions().back();
    sf::CircleShape circle{};
    circle.setRadius(1);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(1);
    circle.setPosition(static_cast<float>(pointPosition.first), static_cast<float>(pointPosition.second));
    window.draw(circle);
  }
  sf::CircleShape circle{};
  circle.setRadius(1);
  circle.setOutlineColor(sf::Color::Green);
  circle.setOutlineThickness(1);
  circle.setPosition(static_cast<float>(winningPosition.first), static_cast<float>(winningPosition.second));
  window.draw(circle);
  ImGui::SFML::Render(window);
  window.display();
}

auto updateFieldView(std::vector<sf::RectangleShape> &fieldView, std::vector<unsigned short int> const &field) -> void {
  for (size_t i = 0; i < fieldView.size(); ++i) {
    switch (field.at(i)) {
    case 0: {
      fieldView.at(i).setFillColor(sf::Color::White);
      break;
    }
    case 1: {
      fieldView.at(i).setFillColor(sf::Color::Red);
      break;
    }
    case 2: {
      fieldView.at(i).setFillColor(sf::Color::Green);
      break;
    }
    case 3: {
      fieldView.at(i).setFillColor(sf::Color::Blue);
      break;
    }
    case 4: {
      fieldView.at(i).setFillColor(sf::Color::Yellow);
      break;
    }
    case 5: {
      fieldView.at(i).setFillColor(sf::Color::Magenta);
      break;
    }
    case 6: {
      fieldView.at(i).setFillColor(sf::Color::Cyan);
      break;
    }
    default: {
      fieldView.at(i).setFillColor(sf::Color::Black);
      break;
    }
    }
  }
}

auto changeWindowEvent(std::vector<sf::Event> const &events, sf::RenderWindow &window) -> void {
  for (auto const &event : events) {
    ImGui::SFML::ProcessEvent(event);
    windowEvent(event, window);
  }
}

auto extractEvent(sf::RenderWindow &window) -> std::vector<sf::Event> {
  using namespace sf;
  auto result = std::vector<Event>{};
  auto event = sf::Event{};
  while (window.pollEvent(event)) {
    result.push_back(event);
  }
  return result;
}
} // namespace view
