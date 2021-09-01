#include "ScoreApp.h"
#include "Screen.h"
#include "Symbols.h"
#include "lvgl/lvgl.h"

using namespace Pinetime::Applications::Screens;


static void btnEventHandler(lv_obj_t* obj, lv_event_t event) {
  ScoreApp* screen = static_cast<ScoreApp*>(obj->user_data);
  screen->OnButtonEvent(obj, event);
}

ScoreApp::widget_t ScoreApp::createButton(lv_align_t alignment, uint8_t x, uint8_t y, uint8_t width, uint8_t height, const char text[]){

  widget_t button;

  button.button = lv_btn_create(lv_scr_act(), nullptr);
  button.button->user_data = this;
  lv_obj_set_event_cb(button.button, btnEventHandler);
  lv_obj_align(button.button, lv_scr_act(), alignment, x, y);
  lv_obj_set_size(button.button, width, height);
  button.label = lv_label_create(button.button, nullptr);
  lv_label_set_text(button.label, text);

  return button;
}

ScoreApp::ScoreApp(DisplayApp* app) : Screen(app) {
    
  btnMyScore = createButton(LV_ALIGN_IN_TOP_LEFT, 0, 0, scoreWidth, scoreHeight, "0");
  lv_obj_set_style_local_text_font(btnMyScore.label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);
  lv_obj_set_style_local_text_color(btnMyScore.label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);

  btnYourScore = createButton(LV_ALIGN_IN_TOP_LEFT, 0, +scoreHeight, scoreWidth, scoreHeight, "0");
  lv_obj_set_style_local_text_font(btnYourScore.label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);
  lv_obj_set_style_local_text_color(btnYourScore.label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);

  lv_label_set_text_fmt(btnMyScore.label, "%d", myScore);
  lv_label_set_text_fmt(btnYourScore.label, "%d", yourScore);
  
  btnMyScoreMinus = createButton(LV_ALIGN_IN_TOP_RIGHT, 0, 0, minusWidth, minusHeight, "-1");
  btnYourScoreMinus = createButton(LV_ALIGN_IN_TOP_RIGHT, 0, displayHeight-minusHeight, minusWidth, minusHeight, "-1");
  
  btnReset = createButton(LV_ALIGN_IN_TOP_RIGHT, 0, displayHeight/2, resetWidth, resetHeight, "Rst");

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

ScoreApp::~ScoreApp() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void ScoreApp::OnButtonEvent(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    if (obj == btnMyScore.button) {
      myScore < 255 ? myScore++ : 255; // manage overflow in oneliner
    }
    else if (obj == btnYourScore.button) {
      yourScore < 255 ? yourScore++ : 255;
    }
    else if (obj == btnMyScoreMinus.button) {
     myScore > 0 ? myScore-- : 0; // manage underflow
    }
    else if (obj == btnYourScoreMinus.button) {
      yourScore > 0 ? yourScore-- : 0;
    }
    else if (obj == btnReset.button) {
      myScore = 0;
      yourScore = 0;
    }
    lv_label_set_text_fmt(btnMyScore.label, "%d", myScore);
    lv_label_set_text_fmt(btnYourScore.label, "%d", yourScore);
  }
}
