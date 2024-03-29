#include "my_str_t.h"


static size_t my_strlen(const char *str) {
    if(!str)
    {
        return -1;
    }
    size_t index = 0;
    while (str[index++] != '\0');
    return index - 1;
}

static int my_str_insert_length(my_str_t* str, const char* from, size_t pos, size_t length)
{
    if(!str)
    {
        return -1;
    }

    if(str->size_m + length > str->capacity_m)
    {
        if(my_str_reserve(str, (str->size_m + length)*2) == -2)
        {
            return -2;
        }
    }

    memmove(str->data + pos + length, str->data + pos, sizeof(char)*(str->size_m + length - pos));
    memcpy(str->data + pos, from, sizeof(char)*length);
    str->size_m += length;
    return 0;

}

//!===========================================================================
//! Створення та знищення стрічки.
//!===========================================================================

//! Створити стрічку із буфером вказаного розміру. Пам'ять виділяється динамічно.
//! Варто виділяти buf_size+1 для спрощення роботи my_str_get_cstr().
//! УВАГА! Ця функція -- аналог конструктора інших мов.
//! Всі решта функцій можуть вважати, що їм не буде передано жодної змінної,
//! до якої не застосували рівно один раз цю функцію.
//! Тобто, до того, як викликати будь-яку іншу функцію, до змінної типу
//! my_str_t слід рівно один раз за час її існування викликати  my_str_create().
//! (З біллю в серці згадуються закони Мерфі -- кожне твердження
//! обов'язково зрозуміють неправильно. Розказавши 20-30 раз, вирішив
//! записати. Тепер очікую збільшення кількості скарг на довжину опису...)
//! Після використання -- викличте my_str_free().
//! Зауважте -- за такого підходу, повторний виклик my_str_create до тієї ж
//! змінної, без виклику my_str_free, вестиме до memory leak.
//! Повертає 0, якщо все ОК, від'ємні числа, якщо сталася помилка.
int my_str_create(my_str_t *str, size_t buf_size) {
    if (!str) {
        return -1;
    }
    str->data = (char*)malloc(sizeof(char) * (buf_size + 1));
    if (!str->data)
        return -2;
    str->capacity_m = buf_size;
    str->size_m = 0;

    return 0;
}

//! Звільняє пам'ять, знищуючи стрічку.
//! Аналог деструктора інших мов.
void my_str_free(my_str_t *str) {
    free(str->data);
    str->data = 0;
    str->capacity_m = 0;
    str->size_m = 0;
}

//! Створити стрічку із буфером вказаного розміру із переданої С-стрічки.
//! Якщо розмір, buf_size == 0, виділяє блок, рівний розміру С-стрічки, якщо
//! менший за її розмір -- вважати помилкою, не змінювати стрічку.
//! Пам'ять виділяється динамічно. Не слід викликати my_str_create(),
//! більше підійде my_str_resize() або my_str_reserve().
//! Коди завершення:
//! 0 -- якщо все ОК, -1 -- недостатній розмір буфера, -2 -- не вдалося виділити пам'ять
int my_str_from_cstr(my_str_t *str, const char *cstr, size_t buf_size) {
    size_t str_len = my_strlen(cstr);
    if (buf_size == 0)
        buf_size = str_len;

    if (buf_size < str_len) {
        return -1;
    }
    str->capacity_m = buf_size;
    str->size_m = str_len;
    str->data = (char *) malloc(sizeof(char) * (buf_size + 1));
    if (!str->data)
        return -2;
    memcpy(str->data, cstr, sizeof(char)*str->size_m);
    return 0;
}


//!============================================================================
//! Інформація про стрічку
//!============================================================================

//! Повертає розмір стрічки.
//! Для нульового вказівника -- 0.
size_t my_str_size(const my_str_t *str) {
    return !str ? 0 : str->size_m;
}

//! Повертає розмір буфера.
//! Для нульового вказівника -- 0.
size_t my_str_capacity(const my_str_t *str) {
    return !str ? 0 : str->capacity_m;
}

//! Повертає булеве значення, чи стрічка порожня:
int my_str_empty(const my_str_t *str) {
    return my_str_size(str) == 0;
}

//!===========================================================================
//! Доступ до символів стрічки
//!===========================================================================

//! Повертає символ у вказаній позиції, або -1, якщо вихід за межі стрічки,
//! включаючи переданий нульовий вказівник.
//! Тому, власне, int а не char
int my_str_getc(const my_str_t *str, size_t index) {
    if (!str || index >= str->size_m) {
        return -1;
    }

    return str->data[index];
}

//! Записує символ у вказану позиції (заміняючи той, що там був),
//! Повертає 0, якщо позиція в межах стрічки,
//! Поветає -1, не змінюючи її вмісту, якщо ні.
int my_str_putc(my_str_t *str, size_t index, char c) {
    if (!str || index >= str->size_m) {
        return -1;
    }
    str->data[index] = c;
    return 0;
}

//! Повернути вказівник на С-стрічку, еквівалентну str.
//! Вважатимемо, що змінювати цю С-стрічку заборонено.
//! Вважатимемо, що як тільки my_str_t змінено, цей вказівник
//! може стати некоректним.
//! Якщо в буфері було зарезервовано на байт більше за макс. розмір, можна
//! просто додати нульовий символ в кінці та повернути вказівник data.
const char *my_str_get_cstr(my_str_t *str) {
    if (!str || !(str->data))
        return NULL;
    str->data[str->size_m] = '\0';
    return str->data;
}

//!===========================================================================
//! Модифікації стрічки, що змінюють її розмір і можуть викликати реалокацію.
//!===========================================================================
//! Якщо буфер недостатній -- ці функції збільшують його,
//! викликом my_str_reserve().
//! Розумним є буфер кожного разу збільшувати в 1.8-2 рази.
//! ==========================================================================

//! Додає символ в кінець.
//! Повертає 0, якщо успішно,
//! -1 -- якщо передано нульовий вказівник,
//! -2 -- помилка виділення додаткової пам'яті.
int my_str_pushback(my_str_t *str, char c){
    if (!str)
        return -1;

    int status_code = 0;

    if (++str->size_m > str->capacity_m)
        status_code = my_str_reserve(str, str->size_m * 2);
    if (status_code == -2)
        return -2;
    *(str->data + str->size_m - 1) = c;
    return 0;
}

//! Викидає символ з кінця.
//! Повертає його, якщо успішно,
//! -1 -- якщо передано нульовий вказівник,
//! -2 -- якщо стрічка порожня.
int my_str_popback(my_str_t *str){
    if (!str)
        return -1;
    if (my_str_empty(str))
        return -2;

    --str->size_m;

    return 0;
}

//! Копіює стрічку. Якщо reserve == true,
//! то із тим же розміром буферу, що й вихідна,
//! інакше -- із буфером мінімального достатнього розміру.
//! (Старий вміст стрічки перед тим звільняє, за потреби).
//! Повертає 0, якщо успішно, різні від'ємні числа для діагностики
//! проблеми некоректних аргументів.
int my_str_copy(const my_str_t *from, my_str_t *to, int reserve){
    if (!from || !to)
        return -1;
    int status_code = 0;
    size_t new_capacity = reserve ? from->capacity_m : from->size_m;
    my_str_free(to);
    status_code = my_str_create(to, new_capacity);
    if (status_code)
        return -2;
    memcpy(to->data, from->data, sizeof(char)*from->size_m);
    to->size_m = from->size_m;

    return 0;
}

//! Очищає стрічку -- робить її порожньою. Складність має бути О(1).
//! Уточнення (чомусь ця ф-ція викликала багато непорозумінь):
//! стрічка продовжує існувати, буфер той самий, того ж розміру, що був,
//! лише містить 0 символів -- єдине, що вона робить, це size_m = 0.
void my_str_clear(my_str_t *str){
    if(!str)
        return;
    str->size_m = 0;
}

//! Вставити символ у стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert_c(my_str_t *str, char c, size_t pos){
    if(!str)
    {
        return -1;
    }

    if(++str->size_m > str->capacity_m)
    {
        if(my_str_reserve(str, (str->size_m)*2) == -2)
        {
            return -2;
        }
    }

    memmove(str->data + pos + 1, str->data + pos, sizeof(char)*(str->size_m - pos - 1));
    str->data[pos] = c;
    return 0;
}

//! Вставити стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert(my_str_t *str, const my_str_t *from, size_t pos)
{
    if(!str || !from)
        return -1;

    return my_str_insert_length(str, from->data, pos, from->size_m);
}

//! Вставити C-стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert_cstr(my_str_t *str, const char *from, size_t pos)
{
    if(!str)
        return -1;

    return my_str_insert_length(str, from, pos, my_strlen(from));
}

//! Додати стрічку в кінець.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_append(my_str_t *str, const my_str_t *from)
{
    if(!str)
        return -1;

    return my_str_insert(str, from, str->size_m);
}

//! Додати С-стрічку в кінець.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_append_cstr(my_str_t *str, const char *from)
{
    if(!str)
        return -1;

    return my_str_insert_cstr(str, from, str->size_m);
}

//! Скопіювати підстрічку, із beg включно, по end не включно ([beg, end)).
//! Якщо end за межами початкової стрічки -- це не помилка, копіювати всі
//! символи до кінця. beg має бути в її межах -- якщо beg>size, це помилка.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_substr(const my_str_t *from, my_str_t *to, size_t beg, size_t end)
{
    if(!from || !to || beg > from->size_m)
    {
        return -1;
    }

    if(end > from->size_m)
    {
        end = from->size_m;
    }

    if(end - beg > to->capacity_m)
    {

        if(my_str_reserve(to, end - beg) == -2)
        {
            return -2;
        }
    }
    to->size_m = end - beg;
    memcpy(to->data, from->data + beg, sizeof(char)*to->size_m);


    return 0;
}

//! C-string варіант my_str_substr().
//! Вважати, що в цільовій С-стрічці достатньо місц.
int my_str_substr_cstr(const my_str_t *from, char *to, size_t beg, size_t end)
{
    if(!from || !to || beg > from->size_m)
    {
        return -1;
    }

    if(end > from->size_m)
    {
        end = from->size_m;
    }

    memcpy(to, from->data + beg, sizeof(char)*(end - beg));
    to[end-beg] = '\0';
    return 0;
}

//!===========================================================================
//! Маніпуляції розміром стрічки
//!===========================================================================

//! Збільшує буфер стрічки, із збереженням вмісту,
//! якщо новий розмір більший за попередній,
//! не робить нічого, якщо менший або рівний.
//! (Як показує практика, це -- корисний підхід).
//! Для збільшення виділяє новий буфер, копіює вміст
//! стрічки (size_m символів -- немає сенсу копіювати
//! решту буфера) із старого буфера та звільняє його.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_reserve(my_str_t *str, size_t buf_size) {
    if (str->data == 0)
        return -1;
    if (buf_size <= str->capacity_m)
        return 0;

    char *new_data = (char *) malloc(sizeof(char) * (buf_size + 1));
    if (!new_data)
        return -2;
    memcpy(new_data, str->data, sizeof(char)*(str->size_m));
    str->data = new_data;
    str->capacity_m = buf_size;
    return 0;

}

//! Робить буфер розміром, рівний необхідному:
//! так, щоб capacity_m == size_t. Єдиний "офіційний"
//! спосіб зменшити фактичний розмір буфера.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_shrink_to_fit(my_str_t *str){
    if (!str)
        return -1;

    char* new_data = (char*)malloc(sizeof(char)*(str->size_m + 1));
    str->capacity_m = str->size_m;
    memcpy(new_data, str->data, sizeof(char)*str->size_m);
    return 0;
}

//! Якщо new_size менший за поточний розмір -- просто
//! відкидає зайві символи (зменшуючи size_m). Якщо
//! більший -- збільшує фактичний розмір стрічки,
//! встановлюючи нові символи рівними sym.
//! За потреби, збільшує буфер.
//! Сподіваюся, різниця між розміром буфера та фактичним
//! розміром стрічки зрозуміла?
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_resize(my_str_t *str, size_t new_size, char sym){
    if (!str->data)
        return -1;
    if (new_size <= str->size_m){
        str->size_m = new_size;
        return 0;
    }
    int code_status = my_str_reserve(str, new_size);
    if (code_status == -2)
        return -2;

    for (size_t i = str->size_m; i < new_size; i++)
        *(str->data + i) = sym;
    str->size_m = new_size;
    return 0;
}

//!===========================================================================
//! Функції пошуку та порівняння
//!===========================================================================

//! Знайти першу підстрічку в стрічці, повернути номер її
//! початку або (size_t)(-1), якщо не знайдено. from -- місце, з якого починати шукати.
//! Якщо більше за розмір -- вважати, що не знайдено.
size_t my_str_find(const my_str_t *str, const my_str_t *tofind, size_t from){
    if (from > str->size_m)
        return (size_t)(-1);
    for (size_t i = from; i < str->size_m; i++){
        for (size_t j = 0; j < tofind->size_m; j++)
            if (*(str->data + i + j) != *(tofind->data + j)){
                //If one of chars in substring is not equal
                //to char of tofind go to next iteration
                goto label;
            }
        return i;

        label:;
    }
    return (size_t)(-1);
}

static int my_str_cmp_extra(const my_str_t* str1, const char* cstr2, size_t size)
{
    for(int i = 0; i < str1->size_m && i < size; ++i){
        if (*(str1->data + i) < *(cstr2 + i))
            return -1;
        if (*(str1->data + i) > *(cstr2 + i))
            return 1;
    }
    if (str1->size_m < size)
        return -1;
    if (str1->size_m > size)
        return 1;

    return 0;
}

//! Порівняти стрічки, повернути 0, якщо рівні (за вмістом!)
//! -1 (або інше від'ємне значення), якщо перша менша,
//! 1 (або інше додатне значення) -- якщо друга.
//! Поведінка має бути такою ж, як в strcmp.
int my_str_cmp(const my_str_t *str1, const my_str_t *str2){
    return my_str_cmp_extra(str1, str2->data, str2->size_m);
}

//! Порівняти стрічку із С-стрічкою, повернути 0, якщо рівні (за вмістом!)
//! -1 (або інше від'ємне значення), якщо перша менша,
//! 1 (або інше додатне значення) -- якщо друга.
//! Поведінка має бути такою ж, як в strcmp.
int my_str_cmp_cstr(const my_str_t *str1, const char *cstr2){
    size_t size = my_strlen(cstr2);
    return my_str_cmp_extra(str1, cstr2, size);
}

//! Знайти перший символ в стрічці, повернути його номер
//! або (size_t)(-1), якщо не знайдено. from -- місце, з якого починати шукати.
//! Якщо більше за розмір -- вважати, що не знайдено.
size_t my_str_find_c(const my_str_t *str, char tofind, size_t from){
    if (from >= str->size_m)
        return (size_t)(-1);
    for (size_t i = from; i < str->size_m; ++i)
        if (*(str->data + i) == tofind)
            return i;
    return (size_t)(-1);
}

//! Знайти символ в стрічці, для якого передана
//! функція повернула true, повернути його номер
//! або (size_t)(-1), якщо не знайдено:
size_t my_str_find_if(const my_str_t *str, int (*predicat)(int)){
    if(!str)
    {
        return (size_t)(-1);
    }
    for (size_t i = 0; i < str->size_m; ++i)
        if (predicat(*(str->data + i)))
            return i;
    return (size_t)(-1);
}

//!===========================================================================
//! Ввід-вивід
//!===========================================================================

//! Прочитати стрічку із файлу. Читає цілий файл.
//! Не давайте читанню вийти за межі буфера! За потреби --
//! збільшуйте буфер.
//! Рекомендую скористатися fgets().
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_read_file(my_str_t *str, FILE *file){
    if (!file || !str){
        return -1;
    }
    size_t capacity = my_str_capacity(str);
    my_str_free(str);
    my_str_create(str, capacity);
    int c;
    while((c = fgetc(file)) != EOF) {
        my_str_pushback(str, (char)c);
    }
    fclose(file);
    return 0;
}

//! Аналог my_str_read_file, із stdin.
int my_str_read(my_str_t *str){

    return my_str_read_file(str, stdin);
}

//! Записати стрічку в файл:
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_write_file(const my_str_t *str, FILE *file){
    if (!file || !str)
        return -1;
    *(str->data + str->size_m) = '\0';
    int status_code = fputs(str->data, file);
    if (status_code < 0)
        return status_code;
    fclose(file);
    return 0;
}

//! Записати стрічку на консоль:
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_write(const my_str_t *str){
    return my_str_write_file(str, stdout);
}

//! На відміну від my_str_read_file(), яка читає до кінця файлу,
//! читає по вказаний delimiter, за потреби
//! збільшує стрічку.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_read_file_delim(my_str_t *str, FILE *file, char delimiter){
    if (!file || !str){
        return -1;
    }

    size_t capacity = my_str_capacity(str);
    my_str_free(str);
    my_str_create(str, capacity);
    int c;
    while((c = fgetc(file)) != EOF) {
        if(c == delimiter)
            break;
        my_str_pushback(str, (char)c);
    }
    fclose(file);
    return 0;
}