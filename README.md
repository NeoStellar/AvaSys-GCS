<div align="center"> <img src="/ui/assets/neostellar-logo.png" width="600"> </div>

>  **W.I.P!! This project is open source but it is developed for our own UAV.**
> 
>  **Project name is configured wrongly, will be fixed soon**


# AvaSys Ground Control System (GCS) - Linux

AvaSys GCS is the ground control system software of the Utku aircraft developed by Neo Stellar for the TEKNOFEST SAVAŞAN UAV competition.

Our software allows us to control features such as making quick flight mode changes in our unmanned aerial vehicle, listing and editing the vehicle's parameters, self-testing the aircraft at startup, and listing problems.

# Dependencies
 - <a href="https://github.com/mavlink/mavlink">MavLink</a>
 - <a href="https://www.openssl.org/source/old/1.1.1/index.html">OpenSSL-1.1.1.g</a>
 - <a href="https://www.qt.io/">Qt-5.15.2</a>
 - <a href="https://www.mapbox.com/">MapBox</a>

 # Current Status of the UI

 <div align="center"> <img src="/ui/assets/ui-current.png" width="600"> </div>

 ## To Do List
| No. | Mission | Status|
|---|---|---|
| 1 |Adding UDP And USB socket connection|✅|
| 2 |Sending heartbeat continously and getting heartbeat continously|✅|
| 3 |Listing all parameters and update parameters support|🕒|
| 4 |Getting GPS coordinates of the plane and updating location on the map sync.|✅|
| 5 |Getting other planes datas from TEKNOFEST REST API clone, listing all planes on the maps, adding jammer locations|✅| (Jammer Locations is not supported by Teknofest Replica... yet)
| 6 |Adding support clicking on other planes for custom plane follow mode|⏺|
| 7 |Getting raw camera content from UDP|❌|
| 8 |Displaying plane status on map (Speed graph, barometer garph etc.)|❌|
