#include <Arduino.h>

const int ledPin = 2; // Pin, na kterem je pripojena LED

const int dotLength = 150;    // Doba trvani tecky v ms
const int dashLength = 300;   // Doba trvani carky v ms
const int pauseLength = 50;   // Pauza mezi symboly v ms
const int letterPause = 400;  // Pauza mezi pismeny v ms
const int spaceDelay = 800;   // Pauza mezi slovy v ms

// Morseovy kody pro jednotliva pismena (0 = tecka, 1 = carka)
const int morseCodes[26][4] = {
    {0, 1},        // A: .-
    {1, 0, 0, 0},  // B: -...
    {1, 0, 1, 0},  // C: -.-.
    {1, 0, 0},     // D: -..
    {0},           // E: .
    {0, 0, 1, 0},  // F: ..-.
    {1, 1, 0},     // G: --.
    {0, 0, 0, 0},  // H: ....
    {0, 0},        // I: ..
    {0, 1, 1, 1},  // J: .---
    {1, 0, 1},     // K: -.-
    {0, 1, 0, 0},  // L: .-..
    {1, 1},        // M: --
    {1, 0},        // N: -.
    {1, 1, 1},     // O: ---
    {0, 1, 1, 0},  // P: .--.
    {1, 1, 0, 1},  // Q: --.-
    {0, 1, 0},     // R: .-.
    {0, 0, 0},     // S: ...
    {1},           // T: -
    {0, 0, 1},     // U: ..-
    {0, 0, 0, 1},  // V: ...-
    {0, 1, 1},     // W: .--
    {1, 0, 0, 1},  // X: -..-
    {1, 0, 1, 1},  // Y: -.--
    {1, 1, 0, 0}   // Z: --..
};

// Pocet symbolu pro kazde pismeno
const int morseSymbolCount[26] = {2, 4, 4, 3, 1, 4, 3, 4, 2, 4, 3, 4, 2, 2, 3, 4, 4, 3, 3, 1, 3, 4, 3, 4, 4, 4};

void setup() {
    pinMode(ledPin, OUTPUT); // Nastaveni pinu pro LED jako vystup
    Serial.begin(9600);       // Zahajeni serialove komunikace
    Serial.println("Zadejte slovo ktere chcete prevest do moreovi abecedy:");
}

// Funkce pro blikani LED podle Morseova kodu
void blinkMorse(int morseSymbol[], int length) {
    for (int i = 0; i < length; i++) {
        if (morseSymbol[i] == 0) {
            // Tecka
            digitalWrite(ledPin, HIGH);
            delay(dotLength);
        } else if (morseSymbol[i] == 1) {
            // Carka
            digitalWrite(ledPin, HIGH);
            delay(dashLength);
        }
        digitalWrite(ledPin, LOW);  // Vypnuti LED po symbolu
        delay(pauseLength);         // Pauza mezi symboly
        Serial.print(morseSymbol[i] == 0 ? " ." : " -");  // Vypis symbolu
    }
    Serial.print(" /");  // Oddelovac mezi pismeny
}

// Funkce pro prevod retezce na Morseovu abecedu a LED signal
void stringToMorse(String input) {
    input.toUpperCase(); // Prevod vstupu na velka pismena
    for (unsigned int i = 0; i < input.length(); i++) {
        char letter = input[i];
        if (letter == ' ') {
            // Pokud je mezera, vlozi se pauza pro slovo
            delay(spaceDelay);
            Serial.print(" /");
        } else if (letter >= 'A' && letter <= 'Z') {
            // Prevod pismene na Morseuv kod
            int index = letter - 'A';
            blinkMorse(morseCodes[index], morseSymbolCount[index]);
            delay(letterPause); // Pauza mezi pismeny
        } else {
            // Neplatny znak
            Serial.println("Neplatny znak, pouzijte pismeno z abecedy.");
            return;
        }
    }
    Serial.println(" //");  // Ukonceni prevodu
}

void loop() {
    // Pokud je v serialovem bufferu novy vstup
    if (Serial.available() > 0) {
        String input = Serial.readString(); // Cteni vstupu ze serialu
        Serial.println("vstup: " + input); // Vypis vstupu do serialu
        stringToMorse(input);              // Prevod retezce na Morseuv kod
    }
}
