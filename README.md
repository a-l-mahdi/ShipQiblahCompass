# Ship Qiblah Compass: An Arduino Project to Find the Qiblah Direction on a Ship.

By using this hardware, you can receive Gyro and GPS NMEA signals and calculate the qiblah direction simultaneously and show it on any client that is connected to the local network on the ship.

## Table of Contents
•  [Required Hardware](#Hardware)

•  [Installation](#installation)

•  [Usage](#usage)

•  [Screenshots](#screenshots)

•  [Test Videos](#Videos)
<!-- 
•  [License](#license) -->

•  [Contributor](#contributor)

•  [Contact](#contact)


## Hardware
Arduino UNO, Arduino Ethernet Shield, RS422 To TTL Converter


## Installation
To install this app, you need to have Arduino IDE installed on your computer. You also need Mentioned Hardware onboard.

•  Download or Clone this repository using `git clone https://github.com/a-l-mahdi/ShipQiblahCompass.git`

•  Open the .ino file in Arduino IDE, set desire IP for Ethernet Shield and finally uploud .ino File into Arduino Board.

•  Copy index.htm file into memroy SD card and insert to Ethernet Shield.

•  Connect Gyro and GPS NMEA 0183 into Arduino Software Serial usind RS422 to TTL converter.

•  Connect Ethernet Shield to onboard LAN network switch.

•  Open Web Page using IP address that was you set it.

•  Change heading of the ship and see Qiblah direction simultanously on your Phone and Enjoy.



## Usage
When you brows the IP of Arduino on your mobile phone or PC, you will see a Clock with on Hand that shows the Qiblah direction relative to Mecca bearing. You can also see the prayer times for your location.


•  To change the prayer calculation method, tap on the drop down list and select a method from the list.



## Videos

RAW htm version:
[See on YouTube](https://youtu.be/QEIn0vujR9M)


Improved:
[See on YouTube](https://youtu.be/D12Pzf8zaG0)
[See on Aparat](https://aparat.com/v/12Vp5)


## Screenshots
![Screenshot 1](https://github.com/a-l-mahdi/ShipQiblahCompass/blob/main/Screenshot.png)

<!-- 
## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details. -->

## Contributor

Thanks to [artarya](https://github.com/artarya) for develope index.v2.htm files.

## Contact

•  Email: a-l-mahdi@live.com

