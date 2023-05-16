/**
 * @mainpage Counter_Lib Dokümantasyonu
 *
 * @section intro_sec Giriş
 *
 * Counter_Lib, Timer Interrupt'larını kullanarak kolayca sayaçlar oluşturmamıza ve bu sayaçları konteol
 * etmemize olanak sağlayan bir kütüphanedir. Bu dokümantasyon, projenin işlevlerini, bileşenlerini
 * ve kullanımını ayrıntılı bir şekilde açıklamaktadır.
 *
 * @section author_sec Ahmet Talha Ardıç ve 16/05/2023
 *
 * Bu dokümantasyon, Ahmet Talha Ardıç tarafından hazırlanmıştır.
 *
 * - Başlık: Counter_Lib Dokümantasyonu
 * - Yazar: Ahmet Talha Ardıç
 * - Oluşturma Tarihi: 15 Mayıs 2023
 * - Son Güncelleme Tarihi: 16 Mayıs 2023
 *
 * @section install_sec Kurulum
 *
 * Counter_Lib'i kullanmak için aşağıdaki adımları izleyin:
 * - Adım 1: Counter_Lib kaynak kodunu indirin veya kopyalayın.
 * - Adım 2: İndirilen veya kopyalanan klasörü projenizin ana dizinine getirin.
 * - Adım 3: Proje ayarlarınızdan Counter_Lib dosyasının yolunu "Include Paths"e ekleyin.
 * - Adım 4: Artık proje derlenmeye hazır.
 *
 * @section usage_sec Kullanım
 *
 * MyProject'i kullanırken aşağıdaki adımları takip edin:
 * - Adım 1: "counter.h" başlık dosyasını dahil edin.
 * - Adım 2: Artık ilgili işlevlere erişim sağlayabilirsiniz.
 */


#ifndef COUNTER_H_
#define COUNTER_H_

/**
 * @brief Sayac yapisini temsil eden Counter_t turu.
 *
 * Counter_t yapisi, bir sayacin durumunu, ozelliklerini ve iliskili bilgilerini iceren veri yapisini tanimlar.
 * - use: Sayacin kullanılabilir olup olmadığını belirten bir bayrak
 * - enable: Sayacin etkin olup olmadigini belirten bir bayrak
 * - name: Sayac ismini temsil eden karakter dizisi (en fazla 16 karakter)
 * - count: Sayac degeri
 * - target: Sayac hedef degeri
 * - flag: Sayac icin belirtilmis olan bayrak (flag) degişkeninin adresini tutan isaretci
 * - callback_function: Sayac icin belirtilmis olan geri çagirma islevinin (callback function) adresini tutan isaretci
 */
typedef struct {
    uint8_t use; //!< Sayaç kullanım durumu.
    uint8_t enable; //!< Sayaç etkin durumu.
    char name[16]; //!< Sayaç ismi.
    uint16_t count; //!< Sayaç değeri.
    uint16_t target; //!< Sayaç hedef değeri.
    uint8_t* flag; //!< Sayaç bayrağı (flag) değişkeni.
    void (*callback_function)(void); //!< Sayaç geri çağırma işlevi (callback function).
} Counter_t;


Counter_t* counter_add(char* name, uint8_t inital_enable, uint16_t target, uint8_t* flag, void (*callback)(void));
void counter_delete(Counter_t* counter);
void counter_start(Counter_t* counter);
void counter_stop(Counter_t* counter);
int16_t counter_get_count(Counter_t* counter);
char* counter_get_name(Counter_t* counter);

void counter_call(void);


#endif /* COUNTER_H_ */
