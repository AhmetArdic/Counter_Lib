#include <stdint.h>
#include <string.h>

#include "counter.h"


#define MAX_COUNTER 8 //!< Maksimum sayac sayisi
static Counter_t counter_list[MAX_COUNTER] = {0x0}; //!< Sayaclari temsil eden Counter_t turunde bir dizi
static uint8_t total_counter = 0; //!< Toplam sayac sayisi


/**
 * @brief Fonksiyon, yeni bir sayac olusturur ve global sayac listesine eklenmesini saglar.
 *
 * Bu fonksiyon, verilen parametrelerle yeni bir sayac oluşturur ve global sayac listesine sayaci ekler.
 * Eğer toplam sayac sayisi MAX_COUNTER'a ulasirsa, MAX_COUNTER degerine esitlenir ve yeni sayac eklemeden NULL
 * dondurulur. Eğer toplam sayac sayisi negatif bir degere duserse, 0'a esitlenir ve ekleme islemine devam edilir.
 * Olusturulan sayacin yapisal isaretcisi dondurulur. Baslangicta etkinlestirme bayragi (inital_enable) 1 ise sayac
 * baslatilir, 0 ise durdurulur.
 *
 * @param name Sayac adı
 * @param inital_enable Başlangıcta etkinlestirilip etkinlestirilmeyecegi bilgisi (0: devre disi, 1: etkin)
 * @param target Hedef deger
 * @param flag Sayac hedefe ulastiginda kaldirilacak bayragin isaretcisi
 * @param callback Sayac hedefe ulaştigina çagrilacak geri çagirma islevi (callback function)
 * @return Counter_t* Olusturulan yeni sayacin isaretcisi (NULL, eger sayac ekleme islemi basarisiz ise)
 */
Counter_t* counter_add(char* name, uint8_t inital_enable, uint16_t target, uint8_t* flag, void (*callback)(void)){
	if(total_counter > MAX_COUNTER){
		total_counter = MAX_COUNTER;
		return NULL;
	}

	if(total_counter < 0){
		total_counter = 0;
	}

	Counter_t* temp_counter = &counter_list[total_counter];

	memset(temp_counter, 0x0, sizeof(Counter_t));

	temp_counter->use = 1;
	strcpy(temp_counter->name, name);
	temp_counter->target = target;
	temp_counter->flag = flag;
	temp_counter->callback_function = callback;

	if(inital_enable) counter_start(temp_counter);
	else counter_stop(temp_counter);

	total_counter++;

	return temp_counter;
}


/**
 * @brief Fonksiyon, belirtilen sayaci siler.
 *
 * Bu fonksiyon, belirtilen sayaci siler. Eger sayacin isaretcisi NULL ise,
 * hicbir islem yapılmaz ve fonksiyon sonlanir. Aksi halde, `counter_list` dizisindeki
 * sayaclari kontrol eder. Kullanımda olan bir sayacın isaretcisi verilen sayacin isaretcisiyle
 * eslesirse, sayacin tum alanları sifirlanir (`(Counter_t){0x00}` ile), toplam sayacin
 * sayisi bir azaltilir ve fonksiyon sonlanir.
 *
 * @param counter Silinecek sayacin işaretçisi
 */
void counter_delete(Counter_t* counter){

	if(counter == NULL){
		return;
	}

	for(int i=0 ; i<MAX_COUNTER ; ++i){
		Counter_t *temp_counter = &counter_list[i];
		if(temp_counter->use){
			if(temp_counter == counter){
				*temp_counter = (Counter_t){0x00};
				total_counter--;
				return;
			}
		}
		temp_counter = NULL;
	}
}


/**
 * @brief Bir sayacin baslatilmasini saglayan fonksiyon.
 *
 * Bu fonksiyon, bir Counter_t isaretcisi alir ve isaretcinin gosterdigi sayaci baslatir.
 * Eger counter isaretcisi NULL ise fonksiyon hemen sonlanir.
 * Sayac baslatildiginda "enable" alani 1 olarak ayarlanir.
 *
 * @param counter Counter_t turunde bir isaretci.
 */
void counter_start(Counter_t* counter){

	if(counter == NULL){
		return;
	}

	counter->enable = 1;
}


/**
 * @brief Bir sayacin durdurulmasini saglayan fonksiyon.
 *
 * Bu fonksiyon, bir Counter_t isaretcisi alir ve isaretcinin gosterdigi sayacı durdurur.
 * Eger counter isaretcisi NULL ise fonksiyon hemen sonlanir.
 * Sayac durduruldugunda "enable" alanı 0 olarak ayarlanir ve "count" alanı sifirlanir.
 *
 * @param counter Counter_t turunde bir isaretci.
 */
void counter_stop(Counter_t* counter){

	if(counter == NULL){
		return;
	}

	counter->enable = 0;
	counter->count = 0;
}


/**
 * @brief Counter'in geçerli sayac degerini donduren bir fonksiyon.
 *
 * Bu fonksiyon, bir Counter_t isaretcisi alir ve isaretcinin gısterdigi sayactan
 * gecerli sayac degerini dondurur. Gecerli sayac degeri, Counter_t yapisi icindeki
 * "count" alaninda saklanır.
 *
 * @param counter Counter_t turunde bir isaretci.
 * @return Gecerli sayac degeri. Eger counter isaretcisi NULL ise -1 dondurur.
 */
int16_t counter_get_count(Counter_t* counter){

	if(counter == NULL){
		return -1;
	}

	for(int i=0 ; i<MAX_COUNTER ; ++i){
		Counter_t *temp_counter = &counter_list[i];
		if(temp_counter->use){
			if(temp_counter == counter){
				return temp_counter->count;
			}
		}
		temp_counter = NULL;
	}
	return -1;
}


/**
 * @brief Bir sayacin ismini donduren fonksiyon.
 *
 * Bu fonksiyon, bir Counter_t isaretcisi alir ve isaretcinin gosterdigi sayacin ismini dondurur.
 * Eger counter isaretcisi NULL ise NULL dondurur.
 *
 * @param counter Counter_t türünde bir isaretci.
 * @return Sayac ismini temsil eden bir karakter dizisi.
 *         Eğer counter isaretsisi NULL ise NULL dondurur.
 */
char* counter_get_name(Counter_t* counter){
	if(counter == NULL){
		return NULL;
	}

	return counter->name;
}


/**
 * @brief Sayacları isleyen fonksiyon.
 *
 * Bu fonksiyon, timer interrupt icerisinde cagrilir ve counter_list dizisindeki sayacları periyodik olarak isler.
 * Her bir sayac icin asagidaki islemleri gerceklestirir:
 * - Sayac kullaniliyorsa ve etkinse, sayaç degerini bir artirir.
 * - Sayac hedef degeri asildiginda, sayac degerini sifirlar.
 * - Sayac icin belirtilmis olan geri çagırma islevini (callback function) çagirir (eger varsa).
 * - Sayac icin belirtilmic olan bayrak (flag) degiskenini 1 olarak ayarlar (eger varsa).
 *
 * @note Bu fonksiyon, MAX_COUNTER sabitinde belirtilen maksimum sayaç sayisini kullanir ve counter_list dizisindeki sayaclari isler.
 * @note Bu fonksiyonun verimli sekilde calismasi icin void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) icerisinde
 * cagrilmalidir.
 */
void counter_call(void){
	for(int i=0 ; i<MAX_COUNTER ; ++i){
		Counter_t *temp_counter = &counter_list[i];
		if(temp_counter->use){
			if(temp_counter->enable){
				temp_counter->count++;

				if(temp_counter->count >= temp_counter->target){
					temp_counter->count = 0;

					if(temp_counter->callback_function != NULL) temp_counter->callback_function();
					if(temp_counter->flag != NULL) *temp_counter->flag = 1;
				}
			}
		}
		temp_counter = NULL;
	}
}


