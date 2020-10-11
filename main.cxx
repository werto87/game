#ifdef GUI_MODE
#include "src/view.hxx"
#endif
#include "src/game.hxx"
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

int main(int argc, char **argv) {
  auto consoleArguments = game::parseConsoleArguments(argc, argv);
  bool debug = consoleArguments["debug"].as<bool>();
  int foo = consoleArguments["foo"].as<int>();
  spdlog::info("debug: {} foo: {} ", debug, foo);
  auto const winningCombination = std::vector<int>{1, 1, 1, 1};
  auto const losingNumber = 7;
  auto field = std::vector<int>(winningCombination.size());
  auto isGameWon = true;
#ifdef GUI_MODE
  auto const windowWidth = 640u;
  auto const windowHeight = 480u;
  sf::RenderWindow window(sf::VideoMode{windowWidth, windowHeight}, "ImGui + SFML = <3");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);
  auto fieldView = view::createFieldView(windowWidth, winningCombination.size());
  sf::Clock deltaClock;
#endif
  while (!game::checkGameFinished(field, winningCombination, losingNumber, isGameWon)
#ifdef GUI_MODE
         && window.isOpen()
#endif
  ) {
#ifdef GUI_MODE
    auto eventsToProcess = view::extractEvent(window);
#else
    auto eventsToProcess = std::vector<sf::Event>{game::charNumberToSfEvent(game::readKeyFromCin())};
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
