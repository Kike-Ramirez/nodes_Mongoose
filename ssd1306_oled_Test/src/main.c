/*
 * App Title: ssd1306_oled_Test
 * Author: Kike Ramírez
 * Date: 24/4/2028
 *
 */

#include <Arduino.h>

#include "common/cs_dbg.h"
#include "mgos_app.h"
#include "mgos_timers.h"

Adafruit_SSD1306 *d1 = nullptr;

static void timer_cb(void *arg);

enum mgos_app_init_result mgos_app_init(void) {
  // I2C
  d1 = new Adafruit_SSD1306(4 /* RST GPIO */, Adafruit_SSD1306::RES_128_64);

  if (d1 != nullptr) {
    d1->begin(SSD1306_SWITCHCAPVCC, 0x3C, true /* reset */);
    d1->display();
  }

  mgos_set_timer(1000 /* ms */, true /* repeat */, timer_cb, NULL);
    return MGOS_APP_INIT_SUCCESS;
}

static void show_num(Adafruit_SSD1306 *d, const char *s, int i) {
  d->clearDisplay();
  d->setTextSize(2);
  d->setTextColor(WHITE);
  d->setCursor(d->width() / 4, d->height() / 4);
  d->printf("%s%d", s, i);
  d->display();
}

static void timer_cb(void *arg) {
  static int i = 0;
  if (d1 != nullptr) show_num(d1, "i = ", i);
  LOG(LL_INFO, ("i = %d", i));
  i++;
  (void) arg;
}

#if 0
void loop(void) {
  /* For now, do not use delay() inside loop, use timers instead. */
}
#endif