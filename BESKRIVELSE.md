# Vandtank Niveausensor Projekt

## Projektbeskrivelse
Dette projekt har til formål at overvåge vandstanden i en 1.000 liters vandtank ved hjælp af en niveausensor, visualisere vandstanden med en RGB LED-strip og sende niveaudata via MQTT.

## Komponenter
1. ESP8266-baseret mikrocontroller (f.eks. D1 Mini Lite)
2. Ultralydssensor (f.eks. HC-SR04) til niveaumåling
3. RGB LED-strip (vandtæt)
4. Strømforsyning
5. Vandtæt kasse til elektronik

## Funktioner
1. **Niveaumåling**: 
   - Ultralydssensoren måler afstanden til vandoverfladen hvert 10. minut.
   - Vandstanden beregnes og konverteres til en procentdel.

2. **Visuel indikation**:
   - RGB LED-strip viser vandstanden:
     * 100% - 50%: Grøn
     * 50% - 15%: Gul
     * Under 15%: Rød
   - LED-lysstyrken justeres proportionalt med vandstanden inden for hvert farveinterval.

3. **MQTT-integration**:
   - Vandstandsdata sendes som procentdel via MQTT hvert 10. minut.
   - MQTT-emnet kunne være: "vandtank/niveau"

4. **Strømstyring**:
   - Implementer deep sleep-tilstand mellem målinger for at spare strøm.

## Implementeringsdetaljer
1. **Setup**:
   - Konfigurer WiFi-forbindelse og MQTT-klient.
   - Initialiser ultralydssensor og RGB LED-strip.

2. **Hovedloop**:
   - Vågn fra deep sleep hvert 10. minut.
   - Mål vandstanden med ultralydssensoren.
   - Beregn vandstandsprocenten.
   - Opdater RGB LED-strip baseret på vandstandsprocenten.
   - Send data via MQTT.
   - Gå i deep sleep-tilstand.

3. **Kalibrering**:
   - Implementer en kalibreringsrutine for at måle tankens fulde højde.
   - Gem kalibreringsdataene i EEPROM.

4. **Fejlhåndtering**:
   - Implementer fejlkontrol for sensormålinger.
   - Håndter netværksfejl og MQTT-forbindelsesproblemer.

## Udvidelsesmuligheder
1. Tilføj en webserver for lokal visning af data og konfiguration.
2. Implementer alarmer for kritiske vandniveauer.
3. Tilføj en temperaturmåling af vandet.
4. Integrer med hjemmeautomatiseringssystemer som Home Assistant.

## Sikkerhedsovervejelser
1. Sørg for, at al elektronik er ordentligt isoleret og vandtæt.
2. Implementer sikker MQTT-kommunikation med kryptering og autentificering.
3. Overvej at tilføje en fysisk reset-knap i tilfælde af softwarefejl.

Dette projekt kombinerer sensormåling, visuel feedback og IoT-funktionalitet for at skabe en effektiv og informativ vandstandsovervågningsløsning.
