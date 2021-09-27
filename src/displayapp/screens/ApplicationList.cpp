#include "ApplicationList.h"
#include <lvgl/lvgl.h>
#include <array>
#include "Symbols.h"
#include "Tile.h"
#include "displayapp/Apps.h"
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;

ApplicationList::ApplicationList(Pinetime::Applications::DisplayApp* app,
                                 Pinetime::Controllers::Settings& settingsController,
                                 Pinetime::Controllers::Battery& batteryController,
                                 Controllers::DateTime& dateTimeController)
  : Screen(app),
    settingsController {settingsController},
    batteryController {batteryController},
    dateTimeController {dateTimeController},
    screens {app,
             settingsController.GetAppMenu(),
             {
               [this]() -> std::unique_ptr<Screen> {
                 return CreateScreen1();
               },
               //[this]() -> std::unique_ptr<Screen> {return CreateScreen2();},
               //[this]() -> std::unique_ptr<Screen> { return CreateScreen3(); }
             },
             Screens::ScreenListModes::UpDown} {
}

ApplicationList::~ApplicationList() {
  lv_obj_clean(lv_scr_act());
}

bool ApplicationList::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> ApplicationList::CreateScreen1() {
  std::array<Screens::Tile::Applications, 6> applications {{
    {Symbols::stopWatch, Apps::StopWatch},
    {Symbols::music, Apps::Music},
    {Symbols::hourGlass, Apps::Timer},
    {Symbols::shoe, Apps::Steps},
    {Symbols::heartBeat, Apps::HeartRate},
    {Symbols::drum, Apps::Metronome},
  }};

  return std::make_unique<Screens::Tile>(0, 1, app, settingsController, batteryController, dateTimeController, applications);
}

std::unique_ptr<Screen> ApplicationList::CreateScreen2() {
  std::array<Screens::Tile::Applications, 6> applications {{
    {Symbols::clock, Apps::Alarm},
    {"", Apps::None},
    {"", Apps::None},
    {"", Apps::None},
    {"", Apps::None},
    {"", Apps::None},
  }};

  return std::make_unique<Screens::Tile>(1, 2, app, settingsController, batteryController, dateTimeController, applications);
}

/*std::unique_ptr<Screen> ApplicationList::CreateScreen3() {
  std::array<Screens::Tile::Applications, 6> applications {
          {{"A", Apps::Meter},
           {"B", Apps::Navigation},
           {"C", Apps::Clock},
           {"D", Apps::Music},
           {"E", Apps::SysInfo},
           {"F", Apps::Brightness}
          }
  };

  return std::make_unique<Screens::Tile>(2, 3, app, settingsController, batteryController, dateTimeController, applications);
}*/
