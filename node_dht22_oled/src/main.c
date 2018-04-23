/*
 * App Title: ssd1306_oled_Test
 * Author: Kike Ramirez
 * Date: 24/4/2028
 *
 */

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "common/cs_dbg.h"
#include "mgos_app.h"
#include "mgos_timers.h"

#include "mgos.h"
#include "mgos_dht.h"

Adafruit_SSD1306 *d1 = nullptr;

static struct mgos_dht *s_dht = NULL;

static void show_num(Adafruit_SSD1306 *d, const char *s, int i) {
  d->clearDisplay();
  d->setTextSize(1);
  d->setTextColor(WHITE);
  d->setCursor(d->width() / 8, d->height() / 4);
  d->printf("%s%d", s, i);
  d->display();
}

static void dht_timer_cb(void *arg) {
  float t = mgos_dht_get_temp(s_dht);
  float h = mgos_dht_get_humidity(s_dht);

  if (isnan(h) || isnan(t)) {
    LOG(LL_INFO, ("Failed to read data from sensor\n"));
    return;
  }
  LOG(LL_INFO, ("Temperature: %f *C Humidity: %f %%\n", t, h));
  
  show_num(d1, "Temp: ", t);
  
  (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
  // I2C
  d1 = new Adafruit_SSD1306(4 /* RST GPIO */, Adafruit_SSD1306::RES_128_64);

  if (d1 != nullptr) {
    d1->begin(SSD1306_SWITCHCAPVCC, 0x3C, true /* reset */);
    d1->display();
  }
  
  else return MGOS_APP_INIT_ERROR;

  if ((s_dht = mgos_dht_create(15, DHT22)) == NULL) return MGOS_APP_INIT_ERROR;

  mgos_set_timer(2000 /* ms */, true /* repeat */, dht_timer_cb, NULL);
  
  return MGOS_APP_INIT_SUCCESS;

}

#if 0
void loop(void) {
  /* For now, do not use delay() inside loop, use timers instead. */
}
#endif
