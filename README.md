# IoT alapok projekt feladat 2026 ősz levelező
## Vári Gergely (PBFX0R)

## Növényöntöző és környezetfigyelő rendszer

### Főbb funkciók:
- **Talajnedvesség mérés:** Egy kapacitív talajnedvesség szenzor figyeli a növény talajának a nedvességét.
- **Automata öntözés:** A beolvasott értékek alapján és a beállított küszöbérték szerint egy **ESP32-S3** vezérli az öntözést.
- **Környezet monitorozás:** Egy **DHT22** szenzor valós időben méri a levegő hőmérsékletét és páratartalmát.
- **Víztartály figyelés:** A víztartályban lévő víz magasságának monitorozása.
- **Adafruit IO szinkronizáció:** A gyűjtött mérési adatokat a rendszer Wi-Fi hálózaton folyamatosan szinkronizálja az Adafruit IO-ba.

### Felhasznált eszközök:
- ESP32-S3
- Kapacitív talajnedvesség érzékelő modul
- HC-SR04 ultrahangos távolságmérő szenzor
- DHT22 szenzor
- Mini vízszivattyú 5V
- LED
- IRL3705N MOSFET a szivattyú vezérléshez
- 1N4007 védődióda a szivattyúhoz
- 10kΩ lehúzó ellenállás a MOSFET gate lábára
- 10kΩ felhúzó ellenállás DHT22 Data lábához
- 330Ω védőellenállás a MOSFET gate lába a mikrokontroller közé
- 330Ω előtétellenállás a LED-hez.
- 1kΩ és 2kΩ ellenállás feszültségosztónak a HC-SR04 szenzornak a mikrokontrollerre kapcsolásához.

### A projekt leírása:
Egy növény automatikus locsolásának a megvalósítása ESP32-S3 mikrokontrollerrel. A növény földjének a nedvességét 30 percenként monitorozza és adatok alapján dönt, hogy elindítja-e a locsolást(15mp-re). A rendszerhez tartozó víztartálynak a vízszintjét egy HC-SR04 szenzor ellenőrzi, ha alacsony a vízszint egy LED-et felkapcsol. Egy DHT22-es szenzor monitorozza a hőfokot és páratartalmat. A mért adatokat Wi-Fi hálózaton keresztül feltölti az Adafruit IO-ra, ahol követni lehet az adatokat, grafikonokon megjeleníti és logolja a locsolások időpontját. Alacsony vízszint esetén a LED jelzésen felül az Adafruit IO felületéről küld egy email emlékeztetőt, hogy a rendszert fel kell tölteni.
