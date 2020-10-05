#ifdef GUI_MODE
#include "src/view.hxx"
#endif
#include "src/game.hxx"
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <imgui.h>

#ifndef GUI_MODE
constexpr auto OFFSET_TO_TRANSFORM_CHAR_INTO_SFML_NUMBER = 22;
#endif
int main(int argc, char **argv) {
  auto consoleArguments = game::parseConsoleArguments(argc, argv);
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
  auto fieldView = view::createFieldView(windowWidth, elementsPerRow);
  sf::Clock deltaClock;
#endif
  while (!game::checkGameFinished(field, isGameWon)) {
#ifdef GUI_MODE
    auto eventsToProcess = view::extractEvent(window);
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
    game::changeFieldEvent(eventsToProcess, field);
#ifdef GUI_MODE
    view::changeWindowEvent(eventsToProcess, window);
    view::updateFieldView(fieldView, field);
    view::drawBoard(window, fieldView, deltaClock);
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
