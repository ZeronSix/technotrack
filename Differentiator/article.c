#include "article.h"
#include <assert.h>
#include <stdlib.h>

/*!
 * Intro format.
 */
static const char INTRO_FORMAT[] = "В современной математике одно из самых перспективных направлений - " \
                                  "это %s %s %s %s. При этом не обойтись без вычисления производных. " \
                                  "Так, например, %s %s %s определяется следующим образом.\n";

static const size_t WORD1_COUNT = 4;
static const size_t WORD2_COUNT = 4;
static const size_t WORD3_COUNT = 4;
static const size_t WORD4_COUNT = 4;
static const size_t WORD5_COUNT = 4;
static const size_t WORD6_COUNT = 4;
static const size_t WORD7_COUNT = 4;

static const char *WORDS1[] = 
{
    "квантовая",
    "детерминирующая",
    "релятивистская",
    "асинхронная"
};

static const char *WORDS2[] = 
{
    "флуктуация",
    "вибрация",
    "прецессия",
    "постуляция"
};

static const char *WORDS3[] = 
{
    "излучающих",
    "флюоресцентных",
    "преломляющих",
    "поглощающих"
};

static const char *WORDS4[] = 
{
    "тетраболоидов",
    "преобразований",
    "компиляций",
    "сортировок"
};

static const char *WORDS5[] = 
{
    "гироскопический",
    "дифференцирующий",
    "межпространственный",
    "тормозящий"
};

static const char *WORDS6[] = 
{
    "градиент",
    "интегратор",
    "оператор",
    "мюонизатор"
};

static const char *WORDS7[] = 
{
    "Лобачевского",
    "стерилизации",
    "графа",
    "кривой"
};


/*!
 * Transitions
 */
static const char TRANSITION_FORMAT[] = "%s\n";

static const size_t TRANSITION_COUNT = 6;
static const char *TRANSITIONS[] = 
{ 
    "Очевидно, что:", 
    "Вывод можно оставить без доказательства.",
    "Оставим доказательство вывода читателю на размышление в свободное время.",
    "Ясно, что:",
    "Вдумчивый читатель легко догадается, что:",
    "Не требует объяснения, что:"
};

/*!
 * Outro format.
 */
static const char OUT_FORMAT[] = "Такие дела. $\\blacksquare$\n";

void PrintIntro(FILE *file)
{
    assert(file);
    fprintf(file, INTRO_FORMAT, WORDS1[(size_t)rand() % WORD1_COUNT],
                                WORDS2[(size_t)rand() % WORD2_COUNT],
                                WORDS3[(size_t)rand() % WORD3_COUNT],
                                WORDS4[(size_t)rand() % WORD4_COUNT],
                                WORDS5[(size_t)rand() % WORD5_COUNT],
                                WORDS6[(size_t)rand() % WORD6_COUNT],
                                WORDS7[(size_t)rand() % WORD7_COUNT]);
}

void PrintTransition(FILE *file)
{
    assert(file);
    fprintf(file, TRANSITION_FORMAT, TRANSITIONS[(size_t)rand() % TRANSITION_COUNT]);
}

void PrintOutro(FILE *file)
{
    assert(file);
    fprintf(file, OUT_FORMAT);
}
