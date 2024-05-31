# ![Bluebird](docs/bluebird_github_banner.png)

Bluebird is a [hanging kinetic sculpture](https://en.wikipedia.org/wiki/Kinetic_art) inspired by organic natural materials and the shapes of hanging planters with strings that encode the environmental properties of Indoor Air Quality (IAQ) data in the form of a [data physicalization](http://dataphys.org/). It is meant to be hung from the ceiling in small to medium rooms and changes based on real-time air quality monitor data. Strings (plant branches) either become longer or smaller simulating the growth of a plant and the history and predictions of CO2 concentrations. Movements of the leaves indicate the freshness of air for a multi-sensory experience. The overall design philosophy of the shapes and forms uses the notion of calm technology to convey data with [minimal interruption cost](https://calmtech.com/).

## Components

### Controller
A controller device running on an [Arduino Uno Rev3](https://store.arduino.cc/products/arduino-uno-rev3) microcontroller with a [16-Channel PWM/Servo Shield](https://www.adafruit.com/product/1411) is used to control six [360° MG90S type Micro Servo Motors](https://www.towerpro.com.tw/product/mg90s-3/). Attached to these motors are pulleys with fishing lines simulating the growth of the hanging planter so that the string can be moved up and down.

### Gateway
A gateway device running on a [NodeMCU](https://www.nodemcu.com/index_en.html) (ESP8266-based) microcontroller through wi-fi [IOTroam](https://www.surf.nl/en/services/iotroam) fetches the [Lab42 building API](https://lab42.uva.nl/) to gather the latest real-time CO2 concentrations through the avaliable in-building sensors and sends them over [UART](https://docs.arduino.cc/learn/communication/uart/) to the controller device moving the servo motors.

### Materials
The strings, leaves, and housings of the electronics and mechanical hardware are created using [additive manufacturing](https://en.wikipedia.org/wiki/3D_printing) (3D Printing) using a Fused deposition Modeling (FDM) technique using [Polylactic acid](https://en.wikipedia.org/wiki/Polylactic_acid) (PLA) plastic filament in various colors. The electronics enclosures and plant models were modeled using [computer-aided design](https://en.wikipedia.org/wiki/Computer-aided_design) (CAD) software.

## Installation

1) Install a code editor such as [VSCode](https://vscode.dev/)
2) Install the [PlatformIO extension](https://platformio.org/platformio-ide) for VSCode.
3) Clone this repository to your local machine.

```zsh
$ git clone git@github.com:viszlab/prototype.git
```
4) Create a `config.h` file within the _include_ folder with the WiFi SSID, WiFI Password and Lab42 API credentials.

```c++
#define WIFI_SSID "NAME"
#define WIFI_PASSWORD "PASSWORD"
#define API_KEY "KEY"
```
5) Build and upload the `controller.cpp` to the Arduino UNO
6) Build and upload the `gateway.cpp` to the NodeMCU

## Wiring Diagram

![Wire diagram](/docs/bluebird_wiring_diagram.png)

## Project structure

```
├── .vscode                 # Platform IO configuration build files
├── docs                    # Documentation files and images
├── include                 # Platform IO include dependencies
│   └── config.h            # WiFi and API credentials (.env)
├── lib                     # Arduino custom libraries
├── src                     # Platform IO include dependencies
│   ├── gateway.cpp         # Firmware for the NodeMCU gateway
│   └── controller.cpp      # FIrmware for the Arduino controller
├── test                    # Testing scripts
└── platformio.ini          # Platform IO environment config file
```

## Bill of materials

### Electronics

| Qty | Item |
|-----|------|
| 1x  | **Arduino Uno (Rev3) Atmega328P microcontroller** |
| 1x  | **16-channel I2C PWM-Servo Controller (PCA9685)** |
| 1x  | **Universal 5W 2A DC Power Supply Adapter** |
| 1x  | **NodeMCU (ESP8266-based) microcontroller** |
| 6x  | **Mini Servo (MG90S) Digital (360 continuous degrees)** |

### Cables

| Qty | Item |
|-----|------|
| 4x  | **Jumper Wire female/male (Dupont Style 22AWG)** |
| 6x  | **1000mm RC Servo Extension Cables (Dupont Style)** |

### Hardware

| Qty | Item |
|-----|------|
| 6x  | **M2.5L25 DIN 965 countersunk phillips head screws** |
| 12x | **M2.5L25 DIN 965 countersunk phillips head screws** |
| 12x | **M2.5 Heat set Inserts (OD 5mm, L5mm)** |

### Materials

| Qty | Item |
|-----|------|
| 1x  | **Spool of Greenish color (at least ~270g) of 1.75mm PLA (leaves)** |
| 1x  | **Spool of Color of Choice (at least ~570g) of 1.75mm PLA (mounting plates)** |
| 1x  | **Wooden MDF (or other wood type) plate (5mm thickness)** |
| 5x  | **String or rope in color of choice in ~80cm in length (max 5mm thickness)** |

### Tools

| Qty | Item |
|-----|------|
| 1x  | **Screwdriver with small to large phillips bit** |
| 1x  | **Soldering Iron preferably with Heat Set Insert tips** |

## Recommended print settings

To increase the strength of the pulleys and mounting plates the following printer slicing settings are recommended:

* Layer Height: **0.24mm**
* Infill Percentage: **50%**
* Infill Pattern: **Gyroid**
* Perimeters: **4**
* Top Layers: **5**
* Bottom Layers: **5**

## References

For the basics of working with servo motors this tutorial from _DroneBot Workshop_ was used:

1. DroneBot Workshop. (n.d.). Servo motors with Arduino. Retrieved May 31, 2024, from https://dronebotworkshop.com/servo-motors-with-arduino/

The models of the leaves are based on this seat of Montsera leaves by user _@TriTri_786325_:

2. Printables. (n.d.). 20 Monstera leaf coaster set [Files]. Retrieved May 31, 2024, from https://www.printables.com/model/831698-20-monstera-leaf-coaster-set/files

The printing parameters used for the leaves are based on these settings by user _@Posix_:

3. Printables. (n.d.). Multiuse flexible fabric [Files]. Retrieved May 31, 2024, from https://www.printables.com/model/88579-multiuse-flexible-fabric/files


## Project

[**Viszlab**](https://wwww.viszlab.github.io) is a tangible physical data visualization for the [Lab42](https://lab42.uva.nl/) building. An interactive experience using environment sensor data to offer Human Building-Interaction. Viszlab is developed as a Thesis and Graduation project for the Master (MSc) Information Studies: [Information Systems](https://www.uva.nl/shared-content/programmas/en/masters/information-studies/information-studies.html) (track) at the [University of Amsterdam](https://www.uva.nl/en) by part-time student [Danny de Vries](https://www.dandevri.es/).

* **Master**: Master Information Studies: Information Systems (track)
* **University**: University of Amsterdam (UvA)
* **Institute**: Informatics Institute
* **Faculty**: Faculty of Science (FNWI)
* **Research Group**: Digital Interactions Lab (DIL)
* **Student**: BSc Danny de Vries (14495643)
* **Supervisor**: Dr. H. (Hamed) Seiied Alavi PhD

[Viszlab](https://www.viszlab.github.io) © 2024 by [Danny de Vries](https://wwww.github.com/dandevri) is licensed under [CC BY-NC-SA 4.0](http://creativecommons.org/licenses/by-nc-sa/4.0/?ref=chooser-v1).