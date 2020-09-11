#include "BluetoothSerial.h" // подключаем библиотеку для работы с bluetooth 4.2
BluetoothSerial SerialBT; // создаём екземпляр SerialBT

int magnet = 0;   // создаём нулевой канал для управления магнитом по ШИМ
int lighting = 1; // создаём первый канал для управления подсветкой по ШИМ

const int resolution = 8; // задаём разрешение ШИМ сигнала от 1 до 16:  7 бит - 127; 8 бит - 255;

char numbers[] = {'-', '-', '-'}; // массив для хранения значения слайдера
char symbol = '-';                // переменная для хранения символа перед значением слайдера

String number_str;       // переменная для хранения значения слайдера
int frequency;           // переменная для хранения частоты
int magnet_brightness;   // переменная для хранения мощности магнита
int lighting_brightness; // переменная для хранения яркости светодиода

void setup() {
  delay(1000);          // ждём 1 секунду
  Serial.begin(115200); // задаём работу СОМ порта
  
  pinMode(22, OUTPUT); // настраиваем 22 пин как выход
  pinMode(23, OUTPUT); // настраиваем 23 пин как выход
  
  ledcSetup (magnet, 0, resolution); // настраиваем канал для управления зелёным светодиодом по ШИМ
  ledcSetup (lighting, 0, resolution);  // настраиваем канал для управления голубым светодиодом по ШИМ
    
  ledcAttachPin (22, magnet); // подключаем магнит к каналу "magnet"
  ledcAttachPin (23, lighting);  // подключаем подсветку к каналу "lighting"
  
  SerialBT.begin("Ghost_frame"); // запускаем bluetooth на ESP32
  Serial.println("bluetooth запущен");
}


void loop() {
  if (SerialBT.available()) { // если нам что-то пришло, то
    symbol = SerialBT.read(); // записываем символ перед значением слайдера в переменную symbol
    Serial.print(symbol);
    
    numbers[0] = SerialBT.read(); // записываем нулевую цифру в нулевой елемент массива
    Serial.print(numbers[0]);
    
    numbers[1] = SerialBT.read(); // записываем первую цифру в первый елемент массива
    Serial.print(numbers[1]);
    
    numbers[2] = SerialBT.read(); // записываем вторую цифру в второй елемент массива
    Serial.print(numbers[2]);
        
    if (symbol == 'Q') { // если символ перед значением слайдера равен "Q"
      Serial.print(" первый слайдер ");
      number_str = numbers; // преобразовываем массив с цифрами в строку     
      frequency = number_str.toInt(); // преобразовываем строку в int
      
      Serial.print("частота: ");
      Serial.println(frequency);      
      ledcWriteTone(magnet, frequency);   // включаем с частотой переменной frequency
      ledcWriteTone(lighting, frequency); // включаем с частотой переменной frequency
    }
    
    if (symbol == 'W') { // если символ перед значением слайдера равен "W"
      Serial.print(" второй слайдер ");
      number_str = numbers; // преобразовываем массив с цифрами в строку     
      magnet_brightness = number_str.toInt(); // преобразовываем строку в int
      
      Serial.print("мощность магнита: ");
      Serial.println(magnet_brightness);      
      ledcWrite(magnet, magnet_brightness); // включаем магнит с мощностью переменной magnet_brightness
    }

    if (symbol == 'E') { // если символ перед значением слайдера равен "R"
      Serial.print(" четвёртый слайдер ");
      number_str = numbers; // преобразовываем массив с цифрами в строку 
      lighting_brightness = number_str.toInt(); // преобразовываем строку в int
      
      Serial.print("яркость подсветки: ");
      Serial.println(lighting_brightness);
      ledcWrite(lighting, lighting_brightness); // включаем подсветку с яркостью переменной lighting_brightness      
    }    
  }
}
