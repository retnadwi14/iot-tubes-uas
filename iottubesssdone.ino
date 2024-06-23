const int pHSensorPin = 32; // Pin analog 32 untuk sensor pH
const int relayPin = 25; // Pin GPIO untuk relay
const int buttonPin = 27; // Pin GPIO untuk tombol

const float acidThreshold = 4.5;
const float baseThreshold = 4.6;

bool pumpOn = false; // Status pompa air (mati secara default)
bool buttonStateLast = true; // Status tombol terakhir

void setup() {
  Serial.begin(9600);

  // Konfigurasi pin relay
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Relay dimatikan secara default

  // Konfigurasi pin tombol
  pinMode(buttonPin, INPUT_PULLUP); // Menggunakan internal pull-up resistor
}

void loop() {
  // Baca status tombol
  bool buttonState = digitalRead(buttonPin);

  // Pengecekan kondisi tombol (ditekan dan perubahan status)
  if (!buttonState && buttonStateLast) {
    // Tombol ditekan
    pumpOn = !pumpOn; // Toggle status pompa air
    controlPump(pumpOn); // Mengontrol pompa air sesuai status saat ini
    delay(500); // Debouncing delay untuk menghindari bounce dari tombol
  }

  // Simpan status tombol terakhir
  buttonStateLast = buttonState;

  int analogValue = analogRead(pHSensorPin);
  float voltage = analogValue * (3.3 / 4095.0); // Konversi nilai analog menjadi tegangan
  float pHValue = calculatePH(voltage);

  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" pH Value: ");
  Serial.println(pHValue, 2);

  // Pengecekan kondisi pH
  if (pHValue < acidThreshold) {
    Serial.println("pH is acidic. Turning on relay and pump.");
    digitalWrite(relayPin, HIGH); // Aktifkan relay
    controlPump(false); // Aktifkan pompa air
    Serial.println("asam"); // Output asam
  } else if (pHValue > baseThreshold) {
    Serial.println("pH is basic. Turning on relay and pump.");
    digitalWrite(relayPin, HIGH); // Aktifkan relay
    controlPump(true); // Aktifkan pompa air
    Serial.println("basa"); // Output basa
  } else {
    Serial.println("pH is within desired range. Turning off relay and pump.");
    digitalWrite(relayPin, LOW); // Matikan relay
    controlPump(false); // Matikan pompa air
  }

  delay(2000); // Delay sebelum membaca kembali
}

float calculatePH(float voltage) {
  // Implementasi fungsi untuk menghitung pH berdasarkan nilai tegangan
  // Sesuaikan formula dengan karakteristik sensor pH yang Anda gunakan
  // Contoh formula, sesuaikan dengan karakteristik sensor Anda
  return 7 + ((2.5 - voltage) * 3.5);
}

void controlPump(bool state) {
  // Fungsi untuk mengontrol pompa air
  if (state) {
    // Aktifkan pompa air
    digitalWrite(relayPin, HIGH);
    Serial.println("Pump is turned ON.");
  } else {
    // Matikan pompa air
    digitalWrite(relayPin, LOW);
    Serial.println("Pump is turned OFF.");
  }
}
