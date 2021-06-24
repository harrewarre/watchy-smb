#include <Watchy.h>

#include "fonts/PressStart2P8pt7b.h"
#include "fonts/PressStart2P4pt7b.h"

#include "img/smb_background.h"
#include "img/smb_flag.h"
#include "img/smb_weather.h"

#include "img/smb_stepcount.h"

#define BLACK GxEPD_BLACK
#define WHITE GxEPD_WHITE

#define WEATHER_API "http://api.openweathermap.org/data/2.5/weather?q="
#define WEATHER_API_KEY ""
#define WEATHER_LOCATION ""

RTC_DATA_ATTR int weatherTemp;
RTC_DATA_ATTR int weatherCode;

RTC_DATA_ATTR uint8_t activeDay;
RTC_DATA_ATTR uint8_t activeHour;

class SmbWatchFace : public Watchy
{
public:
  void drawWatchFace()
  {
    if (isNewDay())
    {
      sensor.resetStepCounter();
    }

    if (isNewHour())
    {
      updateWeatherInfo();
    }

    drawBackground();
    drawFlag();
    drawTime();
    drawWeatherInfo();
    drawSteps();
  }

private:
  void drawBackground()
  {
    display.drawBitmap(0, 0, smb_background, 200, 200, WHITE);
  }

  void drawTime()
  {
    display.setFont(&PressStart2P8pt7b);
    display.setTextColor(BLACK);

    display.setCursor(6, 22);
    if (currentTime.Hour < 10)
    {
      display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if (currentTime.Minute < 10)
    {
      display.print("0");
    }
    display.println(currentTime.Minute);
  }

  void drawFlag()
  {
    int batteryLevelPercent = 0;
    float vBat = getBatteryVoltage();

    float maxVBat = 4.45;
    float minVBat = 3.2;

    if (vBat >= maxVBat)
    {
      batteryLevelPercent = 100;
    }
    else if (vBat <= minVBat)
    {
      batteryLevelPercent = 0;
    }
    else
    {
      batteryLevelPercent = round(((vBat - minVBat) * 100) / (maxVBat - minVBat));
    }

    int flagTopOffset = 26;

    // Already account for size of the flag itself.
    int flagPoleLength = 124;
    int flagX = 171;

    int flagY = round(abs(((flagPoleLength * batteryLevelPercent) / 100) - flagPoleLength) + flagTopOffset);

    display.drawBitmap(flagX, flagY, smb_flag, 16, 16, BLACK);
  }

  void drawWeatherInfo()
  {
    display.setFont(&PressStart2P4pt7b);
    display.setTextColor(BLACK);

    int iconX = 92;
    int iconY = 6;

    if (weatherCode == 999)
    {
      display.drawBitmap(iconX, iconY, smb_weather_rtc, 16, 16, BLACK);
    }
    else if (weatherCode == 800)
    {
      display.drawBitmap(iconX, iconY, smb_weather_sunny, 16, 16, BLACK);
    }
    else if (weatherCode > 800)
    {
      display.drawBitmap(iconX, iconY, smb_weather_cloudy, 16, 16, BLACK);
    }
    else if (weatherCode >= 700)
    {
      display.drawBitmap(iconX, iconY, smb_weather_mist, 16, 16, BLACK);
    }
    else if (weatherCode >= 600)
    {
      display.drawBitmap(iconX, iconY, smb_weather_snow, 16, 32, BLACK);
    }
    else if (weatherCode >= 500)
    {
      display.drawBitmap(iconX, iconY, smb_weather_rain, 16, 32, BLACK);
    }
    else if (weatherCode >= 300)
    {
      display.drawBitmap(iconX, iconY, smb_weather_drizzle, 16, 32, BLACK);
    }
    else if (weatherCode >= 200)
    {
      display.drawBitmap(iconX, iconY, smb_weather_thunder, 16, 32, BLACK);
    }
    else
    {
      display.setCursor(iconX, iconY + 10);
      display.print(String("??" + weatherCode));
    }

    display.setCursor(115, 14);
    display.print(weatherTemp);
    display.print("'C ");
  }

  void drawSteps()
  {
    int stepCount = sensor.getCounter();

    if (stepCount >= 1000)
    {
      display.drawBitmap(27, 136, smb_stepcount_stalk_top, 16, 16, BLACK);
    }

    if (stepCount >= 2000)
    {
      display.drawBitmap(27, 136, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 120, smb_stepcount_stalk_top, 16, 16, BLACK);
    }

    if (stepCount >= 3000)
    {
      display.drawBitmap(27, 136, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 120, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 104, smb_stepcount_stalk_top, 16, 16, BLACK);
    }

    if (stepCount >= 4000)
    {
      display.drawBitmap(27, 136, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 120, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 104, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 88, smb_stepcount_stalk_top, 16, 16, BLACK);
    }

    if (stepCount >= 5000)
    {
      display.drawBitmap(27, 136, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 120, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 104, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 88, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 72, smb_stepcount_stalk_top, 16, 16, BLACK);
    }

    if (stepCount >= 6000)
    {
      display.drawBitmap(27, 136, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 120, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 104, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 88, smb_stepcount_stalk, 16, 16, BLACK);
      display.drawBitmap(27, 72, smb_stepcount_stalk_top, 16, 16, BLACK);
      display.drawBitmap(27, 56, smb_stepcount_step_goal, 16, 16, BLACK);
    }
  }

  void updateWeatherInfo()
  {
    if (connectWiFi())
    { //Use Weather API for live data if WiFi is connected

      HTTPClient http;
      http.setConnectTimeout(3000); //3 second max timeout

      String weatherQueryURL = String(WEATHER_API) + String(WEATHER_LOCATION) + "&units=metric&appid=" + String(WEATHER_API_KEY);

      http.begin(weatherQueryURL.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode == 200)
      {
        String payload = http.getString();
        JSONVar responseObject = JSON.parse(payload);

        weatherTemp = int(responseObject["main"]["temp"]);
        weatherCode = int(responseObject["weather"][0]["id"]);
      }

      http.end();
      WiFi.mode(WIFI_OFF);
      btStop();
    }
    else
    {
      // No connection
      weatherTemp = RTC.temperature() / 4; //celsius
      weatherCode = 999;
    }
  }

  bool isNewDay()
  {
    if (activeDay != currentTime.Day)
    {
      activeDay = currentTime.Day;
      return true;
    }

    return false;
  }

  bool isNewHour()
  {
    if (isNewDay() || activeHour != currentTime.Hour)
    {
      activeHour = currentTime.Hour;
      return true;
    }

    return false;
  }
};

SmbWatchFace w;

void setup()
{
  w.init();
}

void loop() {}
