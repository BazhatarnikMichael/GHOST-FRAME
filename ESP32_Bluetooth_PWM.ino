#include "BluetoothSerial.h" // подключаем библиотеку для работы с bluetooth 4.2
BluetoothSerial SerialBT; // создаём экземпляр SerialBT

int magnet = 0;   // создаём нулевой канал для управления магнитом по ШИМ
int lighting = 2; // создаём первый канал для управления подсветкой по ШИМ
const int resolution = 8; // задаём разрешение ШИМ сигнала от 1 до 16:  7 бит - 127; 8 бит - 255;

char numbers[] = {'-', '-', '-'}; // массив для хранения значения слайдера
char symbol = '-';                // переменная для хранения символа перед значением слайдера

String number_str;       // переменная для хранения значения слайдера
int magnet_frequency;    // переменная для хранения частоты магнита
int lighting_frequency;  // переменная для хранения частоты светодиодной ленты
int magnet_brightness;   // переменная для хранения мощности магнита
int lighting_brightness; // переменная для хранения яркости светодиода

void setup() {
  pinMode(22, OUTPUT); // настраиваем 22 пин как выход
  pinMode(23, OUTPUT); // настраиваем 23 пин как выход
  
  ledcSetup (magnet, 0, resolution); // настраиваем канал для управления магнитом по ШИМ
  ledcSetup (lighting, 0, resolution);  // настраиваем канал для управления подсветкой по ШИМ
    
  ledcAttachPin (22, magnet); // подключаем магнит к каналу "magnet"
  ledcAttachPin (23, lighting);  // подключаем подсветку к каналу "lighting"
  
  SerialBT.begin("Ghost_frame"); // запускаем bluetooth на ESP32
}

void loop() {
  if (SerialBT.available()) { // если нам что-то пришло, то    
    symbol = SerialBT.read(); // записываем символ перед значением слайдера в переменную symbol    
    numbers[0] = SerialBT.read(); // записываем нулевую цифру в нулевой элемент массива    
    numbers[1] = SerialBT.read(); // записываем первую цифру в первый элемент массива    
    numbers[2] = SerialBT.read(); // записываем вторую цифру в второй элемент массива
    number_str = numbers; // преобразовываем массив с цифрами в строку 

    switch (symbol)
    {
      case 'Q': 
      magnet_frequency = number_str.toInt(); // преобразовываем строку в int
      ledcWriteTone(magnet, magnet_frequency);   // включаем с частотой переменной magnet_frequency
      break;
      case 'W': 
      lighting_frequency = number_str.toInt(); // преобразовываем строку в int
      ledcWriteTone(lighting, lighting_frequency); // включаем с частотой переменной lighting_frequency
      break;
      case 'E':
      magnet_brightness = number_str.toInt();  // преобразовываем строку в int
      ledcWrite(magnet, magnet_brightness); // включаем магнит с мощностью переменной magnet_brightness 
      break;
      case 'R':
      lighting_brightness = number_str.toInt(); // преобразовываем строку в int      
      ledcWrite(lighting, lighting_brightness); // включаем подсветку с яркостью переменной lighting_brightness  
      break;
    }         
  }
}
