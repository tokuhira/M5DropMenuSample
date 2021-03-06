#ifndef _MenuItem_H_
#define _MenuItem_H_

#include <M5Stack.h>
#include <vector>
#include <functional>

#include "Rect.h"
#include "ButtonDrawer.h"

class MenuItem;
//void (* std::function<void(MenuItem*)>)(MenuItem *); 

// メニュー選択時のコールバック
struct MenuCallBack {
  ButtonDrawer btnDrawer;
  virtual void operator()(MenuItem*) {
    M5.Lcd.fillScreen(0);
    btnDrawer.setTitle("Back","","");
    if (setup()) {
      do {
        btnDrawer.draw();
        M5.update();
      } while (loop() && !M5.BtnA.isPressed());
      close();
      M5.Lcd.fillScreen(0);
    }
  }
  virtual bool setup() { return true; };
  virtual bool loop()  { return false; };
  virtual void close() {};
  virtual ~MenuCallBack() {};
};

class MenuItem {
public:
  static int8_t nestOffset;     // nest x offset
  static int8_t itemHeight;
  static int16_t itemWidth;
  static uint16_t fontColor;
  static uint16_t fillColor;    // item fill 
  static uint16_t frameColor;   // item frame
  static uint16_t cursorColor;  // focused fill
  static uint16_t backgroundColor;
  static MenuItem* selectedItem;
  MenuItem();
  MenuItem(const String& title, const std::vector<MenuItem*> &sub);
  MenuItem(const String& title, std::function<void(MenuItem*)> cb);
  MenuItem(const String& title, std::function<void(MenuItem*)> cb, const std::vector<MenuItem*> &sub);
  MenuItem(const String& title, int tg = 0, std::function<void(MenuItem*)> cb = 0);
  MenuItem(const String& title, int tg    , std::function<void(MenuItem*)> cb, const std::vector<MenuItem*> &sub);
  virtual ~MenuItem() {};
  uint16_t SubCount() const;
  void AddSubItem(MenuItem* item);
  void AddSubItems(const std::vector<MenuItem*> &sub);
  void SetSubItems(const std::vector<MenuItem*> &sub);
  void DisposeSubItems();
  void ParentUpdateDestRect();
  int16_t UpdateDestRect(int16_t x = 0, int16_t y = 0);
  void DestRectYScroll(int16_t add_y);
  bool Move();
  void DrawTitle(const String& str);
  void DrawTitle();
  MenuItem* Draw(bool force, const MenuItem* forceItem = 0, const Rect* forceRect = 0);
  void Hide();

  virtual void OnEnter();
  virtual void OnExit() { }
  virtual void OnAfterDraw();

  String title;
  std::function<void(MenuItem*)> callback = 0;
  Rect destRect;    // destinationPoint
  bool visible;
  int tag = 0;

  MenuItem* parentItem = 0;
  std::vector<MenuItem*> subItems;
  Rect rect;        // displayPoint
protected:
  bool moving;
  Rect prevRect;    // previousDisplayPoint
};

#endif
