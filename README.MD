# SMB Watchy Watchface

![SMB](watchy-smb.png)

## Features

- Current time in the top-left.
- Basic weather info.
- Flag position indicates battery % remaining.
- A beanstalk grows from the pipe driven by the stepcounter up to 6000 steps at intervals of 1000 steps.

## Update cycles

- Weather info updates every hour when wifi is available.
- Stepcounter resets at midnight.

All the graphics are included and so is the font, but mind its own license.

## Stuff you need to supply:

- Set a location for the weather: `WEATHER_LOCATION`
- Set an openweathermap API key: `WEATHER_API_KEY`

See https://openweathermap.org for more info on the weather API.