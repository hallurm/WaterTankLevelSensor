# Vandtank Niveausensor Projekt

Dette projekt implementerer et system til at overvåge vandstanden i en 1.000 liters vandtank ved hjælp af en ESP8266-baseret mikrocontroller, en ultralydssensor og en RGB LED-strip. Systemet sender også data via MQTT til integration med hjemmeautomatiseringssystemer.

## Funktioner

1. **Niveaumåling**: Måler vandstanden hvert 10. minut ved hjælp af en ultralydssensor.
2. **Visuel indikation**: Viser vandstanden på en RGB LED-strip med farvekodning og variabel lysstyrke.
3. **MQTT-integration**: Sender vandstandsdata som procentdel via MQTT.
4. **Kalibrering**: Inkluderer en kalibreringsrutine for at måle tankens effektive højde.
5. **Fejlhåndtering**: Implementerer fejlkontrol for sensormålinger og netværksforbindelse.
6. **Fuld tank detektion**: Systemet betragter tanken som fuld, når sensoren måler 10 cm afstand.

## Hardware

- ESP8266-baseret mikrocontroller (f.eks. D1 Mini Lite)
- Ultralydssensor (HC-SR04)
- Vandtæt RGB LED-strip
- Strømforsyning
- Vandtæt kasse til elektronik

## Software-afhængigheder

- Arduino framework
- ESP8266WiFi bibliotek
- PubSubClient bibliotek (for MQTT)
- NewPing bibliotek (for ultralydssensor)
- Adafruit_NeoPixel bibliotek (for LED-strip)

## Opsætning

1. Tilslut hardwaren som følger:
   - Ultralydssensor: TRIGGER_PIN til D1, ECHO_PIN til D2
   - LED-strip: DATA_PIN til D4
2. Installer de nødvendige biblioteker via PlatformIO eller Arduino IDE.
3. Opdater WiFi-legitimationsoplysninger, MQTT-broker indstillinger, samt MQTT-brugernavn og adgangskode i koden.
4. Upload koden til din ESP8266.

## Kalibrering

Ved første opstart vil systemet automatisk gå i kalibreringstilstand. Sørg for, at tanken er tom under kalibreringen. Systemet vil tage flere målinger for at bestemme tankens effektive højde. Tanken betragtes som fuld, når sensoren måler 10 cm afstand fra vandoverfladen.

## Brug

Efter opsætning og kalibrering vil systemet:
- Måle vandstanden hvert 10. minut
- Opdatere LED-strippen baseret på vandstanden
- Sende data via MQTT

LED-farvekodning:
- Grøn: 100% - 50% fuld
- Gul: 50% - 15% fuld
- Rød: Under 15% fuld
- Blinkende rød (nederste LED): Tank er tom (0%)

## Fejlfinding

- Hvis LED'erne ikke lyser, kontroller strømforsyningen og forbindelserne.
- Hvis MQTT-data ikke modtages, verificer netværksforbindelsen og MQTT-broker indstillingerne.
- For unøjagtige målinger, overvej at genkalibrere systemet.

## Fremtidige udvidelser

- Implementering af en webserver for lokal datavisning og konfiguration
- Tilføjelse af temperaturmåling
- Integration med Home Assistant eller andre hjemmeautomatiseringssystemer

## Sikkerhedsovervejelser

- Sørg for, at al elektronik er ordentligt isoleret og vandtæt.
- Implementer sikker MQTT-kommunikation med kryptering og autentificering i produktionsmiljøer.
- Brug sikre metoder til at gemme MQTT-brugernavne og adgangskoder, såsom en separat konfigurationsfil eller sikker lagerløsning.
- Overvej at tilføje en fysisk reset-knap for nem genkalibrering eller fejlretning.

## Bidrag

Føl dig fri til at forke dette projekt og indsende pull requests med forbedringer eller nye funktioner.

## Licens

Dette projekt er udgivet under MIT-licensen. Se `LICENSE` filen for detaljer.
