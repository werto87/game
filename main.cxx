#ifdef GUI_MODE
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#endif
#include <SFML/Window/Event.hpp>
#include <cxxopts.hpp>
#include <initializer_list>
#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>
auto parseConsoleArguments(int argc, char **argv) -> cxxopts::ParseResult {
  cxxopts::Options options("test", "A brief description");
  options.add_options()("b,bar", "Param bar", cxxopts::value<std::string>())(
      "d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))(
      "f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))("h,help", "Print usage");
  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }
  return result;
}

template <typename T> auto print_container(const T &container, const std::string &delimiter) -> std::string {
  auto result = std::string{};
  std::for_each(std::begin(container), std::end(container), [&result, &delimiter](auto const &element) {
    result.append(std::to_string(element));
    result.append(delimiter);
  });
  return result;
}

#ifdef GUI_MODE
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

auto drawBoard(sf::RenderWindow &window, std::vector<sf::RectangleShape> const &fieldView, sf::Clock &deltaClock)
    -> void {
  ImGui::SFML::Update(window, deltaClock.restart());
  window.clear();
  for (size_t i = 0; i < fieldView.size(); ++i) {
    window.draw(fieldView.at(i));
  }
  ImGui::SFML::Render(window);
  window.display();
}

auto updateFieldView(std::vector<sf::RectangleShape> &fieldView, std::vector<int> const &field) -> void {
  for (size_t i = 0; i < fieldView.size(); ++i) {
    switch (field.at(i)) {
    case 0: {
      fieldView.at(i).setFillColor(sf::Color::Red);
      break;
    }
    case 1: {
      fieldView.at(i).setFillColor(sf::Color::Green);
      break;
    }
    default: {
      fieldView.at(i).setFillColor(sf::Color::Blue);
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

auto extractEvent(sf::RenderWindow &window) {
  using namespace sf;
  auto result = std::vector<Event>{};
  auto event = sf::Event{};
  while (window.pollEvent(event)) {
    result.push_back(event);
  }
  return result;
}
#endif

auto checkGameFinished(std::vector<int> const &field, bool &isGameWon) -> bool {
  auto isGameFinished = false;
  if (std::find(std::begin(field), std::end(field), 10) != field.end()) {
    isGameFinished = true;
    isGameWon = false;
    // spdlog::info("player lost");
  }
  if (field == std::vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1}) {
    // spdlog::info("player won");
    isGameFinished = true;
    isGameWon = true;
  } else if (field == std::vector<int>{2, 0, 0, 3, 2, 1, 1, 1, 1}) {
    spdlog::error("some fancy feature which produces a crash");
    abort();
  }
  return isGameFinished;
}

auto changeFieldEvent(std::vector<sf::Event> const &events, std::vector<int> &field) -> void {
  for (auto const &event : events) {
    using namespace sf;
    switch (event.type) {
    case Event::KeyPressed: {
      if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num9) {
        field.at(static_cast<size_t>(event.key.code - Keyboard::Num1)) += 1;
      }
      spdlog::info("1 2 3 4 5 6 7 8 9");
      spdlog::info("{}", print_container(field, " "));
      break;
    }
    default: {
      break;
    }
    }
  }
}

constexpr auto OFFSET_TO_TRANSFORM_CHAR_INTO_SFML_NUMBER = 22;

int main(int argc, char **argv) {
  auto consoleArguments = parseConsoleArguments(argc, argv);
  bool debug = consoleArguments["debug"].as<bool>();
  int foo = consoleArguments["foo"].as<int>();
  spdlog::info("debug: {} foo: {} ", debug, foo);
  auto elementsPerRow = size_t{9};
  auto field = std::vector<int>(elementsPerRow);
  auto isGameWon = true;
#ifdef GUI_MODE
  auto windowWidth = 640u;
  auto windowHeight = 480u;
  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "ImGui + SFML = <3");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);
  auto fieldView = createFieldView(windowWidth, elementsPerRow);
  sf::Clock deltaClock;
#endif
  while (!checkGameFinished(field, isGameWon)) {
#ifdef GUI_MODE
    auto eventsToProcess = extractEvent(window);
#else
    auto keyPressed = char{};
    spdlog::info("please press a key: ");
    std::cin >> keyPressed;
    while (!std::cin) {
      std::cin.clear();
      std::cin >> keyPressed;
    }
    spdlog::info("keyPressed: {}", keyPressed);
    auto event = sf::Event{.type = sf::Event::KeyPressed,
                           .key = sf::Event::KeyEvent{.code = static_cast<sf::Keyboard::Key>(
                                                          keyPressed - OFFSET_TO_TRANSFORM_CHAR_INTO_SFML_NUMBER)}};
    auto eventsToProcess = std::vector<sf::Event>{event};
#endif
    changeFieldEvent(eventsToProcess, field);
#ifdef GUI_MODE
    changeWindowEvent(eventsToProcess, window);
    updateFieldView(fieldView, field);
    drawBoard(window, fieldView, deltaClock);
#endif
  }
#ifdef GUI_MODE
  window.close();
  ImGui::SFML::Shutdown();
#endif
  return isGameWon ? 0 : 1; // note: 0 means success in linux land. its like an error code. result ==0 -> success
                            // result>0 -> error.
}

// extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
//     auto testData=std::vector<int>{};
//     testData.reserve(Size);
//     for(size_t i=0;i<Size;++i){
//         testData.push_back(Data[i]);
//     }
//     auto someBool=true;
//   checkGameFinished(testData, someBool);
//   return 0;  // Non-zero return values are reserved for future use.
// }
