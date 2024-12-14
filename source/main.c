#include <stdio.h>
#include <curl/curl.h>

size_t write_data( char *ptr, size_t size, size_t nmemb, FILE* data)
{
  // ptr - данные из буффера.
  // size - размер принимамого блока данных.
  // nmemb - общее ко-во блоков данных.
  // data - указатель на наш откртый файл, куда будут записываться данные из буффера вместо обычного
  // вывода в консоль.

  return fwrite(ptr, size, nmemb, data);
}

int main(void)
{
  // создаем объект курла (инцилизируем его)
  CURL *handle = curl_easy_init();

  // файл записи
  const char body[] = "body.ts";

  // отскрываем наш файл на запись
  FILE *fp = fopen(body, "w");
  if(!fp) return -1; // проверка открытия

  if(handle)
  {
    // добавляем через опции адрес на который будет отправлен гет запрос
    curl_easy_setopt(handle, CURLOPT_URL, "https://stream1.inoriginal.net/series/cross-2024/s1/e1/480p.mp4/seg-1-v1-a1.ts");
    
    // это настройка для отключение вывода тела ответа/файла
    // curl_easy_setopt(handle, CURLOPT_NOBODY, 1);

    // передаем указатель на файл записи
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
    // передаем функцию записи из буффера в файл
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);

    // отправляем запрос
    CURLcode res;
    res = curl_easy_perform(handle);
    int http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code); // get http status code

    // заканчиваем работу с курлом
    curl_easy_cleanup(handle);
    
    if(http_code != 200)
    {
      printf("Error %d\n", res);
      
      return -1;
    }
    
    printf("Done\n");

  }

  // закрываем файл
  fclose(fp);

  return 0;
}