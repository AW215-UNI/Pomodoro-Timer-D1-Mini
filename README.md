Die folgende Datei benötigt auch noch weitere Libraries. Diese sind:
<ESP8266WiFi.h> ,
<WiFiUdp.h> ,
<NTPClient.h> ,
<Adafruit_NeoPixel.h> 

Alle diese Libraries haben auch weitere Dokumentationen, wie diese Verwendet werden können und welche weiteren Funktionen diese auch beinhalten.
Beim weiteren Experementieren mit dem Code verweise ich auf diese Referenzen der anderen Libraries.

Erklärung des Codes:
Zuerst werden die wichtigen Libraries inkludiert und die jeweiligen Pins für die Knöpfe, LEDs usw. definiert.
Im Setup werden für die Knöpfe die jeweils richtigen Pull-up-Resistoren definiert. Außerdem verbindet sich hier der ESP mit dem Internet. Der ESP kann erst seine Funktion erfüllen, nachdem er mit dem Internet verbunden ist. Es wird auch die Lichthelligkeit der LEDs angepasst.

Vor dem eigentlichen Hauptloop befinden sich noch die Funktionen für den Regenbogenfarbwechsel der LEDs, die aus den für uns zur Verfügung stehenden Dateien entnommen wurden. Der Loop für den Timer wird aktiviert, wenn beide Knöpfe gedrückt werden. Sobald dies geschieht, startet der Loop mit dem roten Loop-Counter bei eins. Nach etwa 10 Sekunden beginnt der erste 24-Minuten-Timer. Da zwischen dem ersten Loop technisch kein weiterer Loop liegt, wurde die Wartezeit hier verkürzt. Alle zwei Minuten wird eine weitere LED in Weiß eingeschaltet. Die abzuwartende Zeit zwischen den einzelnen LEDs wird durch präzise Zeitwerte aus dem Internet gesteuert. Dadurch wird ein genauer Timer garantiert. Allerdings benötigt dieser Timer eine gute Verbindung zum Internet, da es sonst durch ungenaue Abfragen zu Abweichungen kommen kann.

Nach dem 24-Minuten-Timer wird der Loop-Counter um 1 erhöht, wodurch eine weitere LED angeht, und es wird ab jetzt zwischen jedem Loop 5 Minuten gewartet. Dies wiederholt sich bis zum letzten Loop. Nach dem letzten Loop findet ein klares Ende statt, bei dem alle Lichter in Regenbogenfarben leuchten. Sobald die LEDs wieder ausgehen, werden die Knöpfe zurückgesetzt, und der Timer kann erneut gestartet werden. Während des Timers haben die Knöpfe keine Funktion; sie können den Timer nur starten, solange dieser nicht bereits läuft.
