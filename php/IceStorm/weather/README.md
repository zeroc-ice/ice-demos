# IceStorm Weather

This demo shows how to use IceStorm to create a simple pub-sub application. In this demo, one or more weather sensors
report the local temperature and humidity to one or more weather stations via IceStorm. The sensors are the publishers
while the weather stations are the subscribers.

```mermaid
flowchart LR
    p1[Sensor #374] --report--> icestorm[IceStorm service<br>Hosts topic 'weather']
    p2[Sensor #789] --report--> icestorm
    icestorm --report--> s1[Station #1]
    icestorm --report--> s2[Station #2]
    icestorm --report--> s3[Station #3]
```

Ice for PHP supports only client-side applications, and as a result, we can only implement the sensors in PHP.

You first need to start IceStorm and the weather station(s) from a demo written in a language with full server-side
support, such as C++, Python, Java or C#.

Then, in a separate window:

- Go to the IceStorm/weather directory

```shell
cd php/IceStorm/weather
```

- Compile the WeatherStation.ice file with the Slice compiler for PHP

```shell
slice2php WeatherStation.ice
```

- Run the sensor

```shell
php Sensor.php
```
