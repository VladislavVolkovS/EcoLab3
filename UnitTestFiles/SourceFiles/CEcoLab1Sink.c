/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1Sink
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1Sink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"
#include <windows.h>


/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_QueryInterface(/* in */ struct IEcoLab1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    if ( IsEqualUGUID(riid, &IID_IEcoLab1Events ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }

    *ppv = 0;

    return -1;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_AddRef(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    pCMe->m_cRef++;
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_Release(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1Sink((IEcoLab1Events*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

void SetColor(HANDLE* handle, const char* color) {
	if (color == "Default") {
		SetConsoleTextAttribute(*handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	if (color == "White") {
		SetConsoleTextAttribute(*handle, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	if (color == "Red") {
		SetConsoleTextAttribute(*handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
	}
	if (color == "Green") {
		SetConsoleTextAttribute(*handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	if (color == "Blue") {
		SetConsoleTextAttribute(*handle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	if (color == "Yellow") {
		SetConsoleTextAttribute(*handle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	if (color == "YellowBack") {
		SetConsoleTextAttribute(*handle, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN);
	}
	if (color == "GreenBack") {
		SetConsoleTextAttribute(*handle, BACKGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	if (color == "RedBack") {
		SetConsoleTextAttribute(*handle, BACKGROUND_INTENSITY | BACKGROUND_RED);
	}
	if (color == "WhiteBack") {
		SetConsoleTextAttribute(*handle, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	}
}

void PrintRootsInt(const void* roots_data, size_t n_elems, HANDLE* handle) {
    int* result = (int*) roots_data;
	size_t i;
	SetColor(handle, "Green");
	printf("Roots of the polynomial\n");
    for (i = 0; i < n_elems; ++i) {
		SetColor(handle, "GreenBack");
		printf("%d", *(result + i));
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf(" ");
		}
		SetColor(handle, "GreenBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintPolyInt(const void* coefs_data, size_t n_elems, HANDLE* handle) {
    int* result = (int*) coefs_data;
	size_t i;
	SetColor(handle, "Green");
	printf("Polynomial:\n");
	SetColor(handle, "GreenBack");
    for (i = 0; i < n_elems; ++i) {
		if (*(result + i) == 0) {
			continue;
		}
		if (n_elems - i - 1 > 1) {
			printf("%d*x^%d", *(result + i), n_elems - i - 1);
		}
		else if (n_elems - i - 1 == 1) {
			printf("%d*x", *(result + i));
		}
		else {
			printf("%d", *(result + i));
		}
		if (i != n_elems - 1) {
			if (result[i + 1] > 0) {
				printf("+");
			}
		}
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintRootsIntIter(const void* roots_data, size_t n_elems, HANDLE* handle, size_t ind) {
    int* result = (int*) roots_data;
	size_t i;
	SetColor(handle, "White");
	printf("Roots of the polynomial\n");
    for (i = 0; i < n_elems; ++i) {
		int* current = result+i;
		SetColor(handle, "WhiteBack");
		if (i == ind) {
			SetColor(handle, "RedBack");
		}
		printf("%d", *(result + i));
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf(" ");
		}
		SetColor(handle, "WhiteBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintCoefsInt(const void* coefs_data, size_t n_elems, HANDLE* handle, size_t ind) {
    int* result = (int*) coefs_data;
	size_t i;
	SetColor(handle, "Yellow");
	printf("Coefficients:\n");
    for (i = 0; i < n_elems; ++i) {
		int* current = result+i;
		SetColor(handle, "YellowBack");
		if (i == ind) {
			SetColor(handle, "RedBack");
		}
		printf("%d", *(result + i));
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf("  ");
		}
		SetColor(handle, "YellowBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintRootsDouble(const void* roots_data, size_t n_elems, HANDLE* handle) {
    double* result = (double*) roots_data;
	size_t i;
	SetColor(handle, "Green");
	printf("Roots of the polynomial\n");
    for (i = 0; i < n_elems; ++i) {
		SetColor(handle, "GreenBack");
		printf("%.2f", *(result + i));
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf(" ");
		}
		SetColor(handle, "GreenBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintPolyDouble(const void* coefs_data, size_t n_elems, HANDLE* handle) {
    double* result = (double*) coefs_data;
	size_t i;
	SetColor(handle, "Green");
	printf("Polynomial:\n");
	SetColor(handle, "GreenBack");
    for (i = 0; i < n_elems; ++i) {
		if (n_elems - i - 1 > 1) {
			printf("%.2f*x^%d", *(result + i), n_elems - i - 1);
		}
		else if (n_elems - i - 1 == 1) {
			printf("%.2f*x", *(result + i));
		}
		else {
			if (result[i] != 0) {
				printf("%.2f", *(result + i));
			}
		}
		if (i != n_elems - 1) {
			if (result[i + 1] > 0) {
				printf("+");
			}
		}
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintRootsDoubleIter(const void* roots_data, size_t n_elems, HANDLE* handle, size_t ind) {
    double* result = (double*) roots_data;
	size_t i;
	SetColor(handle, "White");
	printf("Roots of the polynomial\n");
	SetColor(handle, "WhiteBack");
    for (i = 0; i < n_elems; ++i) {
		if (i == ind) {
			SetColor(handle, "RedBack");
		}
		printf("%.2f", *(result + i));
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf(" ");
		}
		SetColor(handle, "WhiteBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintCoefsDouble(const void* coefs_data, size_t n_elems, HANDLE* handle, size_t ind) {
    double* result = (double*) coefs_data;
	size_t i;
	SetColor(handle, "Yellow");
	printf("Coefficients:\n");
	SetColor(handle, "YellowBack");
    for (i = 0; i < n_elems; ++i) {
		if (i == ind) {
			SetColor(handle, "RedBack");
		}
		printf("%.2f", *(result + i));
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf("  ");
		}
		SetColor(handle, "YellowBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}

typedef struct ComplexNumberTest {
    double real, imag;
} ComplexTest;

void PrintRootsComplex(const void* roots_data, size_t n_elems, HANDLE* handle) {
    ComplexTest* result = (ComplexTest*) roots_data;
	size_t i;
	SetColor(handle, "Green");
	printf("Roots of the polynomial\n");
    for (i = 0; i < n_elems; ++i) {
		ComplexTest* current = result+i;
		SetColor(handle, "GreenBack");
		if (current->imag >= 1e-6) {
		printf("%.2f+%.2fi", current->real, current->imag);
		} else if (current->imag <= -1e-6) {
			printf("%.2f%.2fi", current->real, current->imag);
		} else {
			printf("%.2f", current->real);
		}
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf(" ");
		}
		SetColor(handle, "GreenBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintPolyComplex(const void* coefs_data, size_t n_elems, HANDLE* handle) {
    ComplexTest* result = (ComplexTest*) coefs_data;
	size_t i;
	SetColor(handle, "Green");
	printf("Polynomial:\n");
	SetColor(handle, "GreenBack");
    for (i = 0; i < n_elems; ++i) {
		ComplexTest* current = result+i;
		if (n_elems - i - 1 > 1) {
			if (current->imag >= 1e-6) {
				printf("(%.2f+%.2fi)*x^%d", current->real, current->imag, n_elems - i - 1);
			} else if (current->imag <= -1e-6) {
				printf("(%.2f%.2fi)*x^%d", current->real, current->imag, n_elems - i - 1);
			} else {
				if (current->real != 0) {
					printf("%.2f*x^%d", current->real, n_elems - i - 1);
				}
			}
		}
		else if (n_elems - i - 1 == 1) {
			if (current->imag >= 1e-6) {
				printf("(%.2f+%.2fi)*x", current->real, current->imag);
			} else if (current->imag <= -1e-6) {
				printf("(%.2f%.2fi)*x", current->real, current->imag);
			} else {
				if (current->real != 0) {
					printf("%.2f*x", current->real);
				}
			}
		}
		else {
			if (current->imag >= 1e-6) {
				printf("%.2f+%.2fi", current->real, current->imag);
			} else if (current->imag <= -1e-6) {
				printf("%.2f%.2fi", current->real, current->imag);
			} else {
				if (current->real != 0) {
					printf("%.2f", current->real);
				}
			}
		}
		if (i != n_elems - 1) {
			if (current->real >= 1e-6) {
				printf("+");
			} else if (current->real == 0) {
				if (current->imag >= 1e-6) {
					printf("+");
				}
			}
			
		}
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintRootsComplexIter(const void* roots_data, size_t n_elems, HANDLE* handle, size_t ind) {
    ComplexTest* result = (ComplexTest*) roots_data;
	size_t i;
	SetColor(handle, "White");
	printf("Roots of the polynomial\n");
    for (i = 0; i < n_elems; ++i) {
		ComplexTest* current = result+i;
		SetColor(handle, "WhiteBack");
		if (i == ind) {
			SetColor(handle, "RedBack");
		}
		if (current->imag >= 1e-6) {
		printf("%.2f+%.2fi", current->real, current->imag);
		} else if (current->imag <= -1e-6) {
			printf("%.2f%.2fi", current->real, current->imag);
		} else {
			printf("%.2f", current->real);
		}
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf(" ");
		}
		SetColor(handle, "WhiteBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}

void PrintCoefsComplex(const void* coefs_data, size_t n_elems, HANDLE* handle, size_t ind) {
    ComplexTest* result = (ComplexTest*) coefs_data;
	size_t i;
	SetColor(handle, "Yellow");
	printf("Coefficients:\n");
    for (i = 0; i < n_elems; ++i) {
		ComplexTest* current = result+i;
		SetColor(handle, "YellowBack");
		if (i == ind) {
			SetColor(handle, "RedBack");
		}
		if (current->imag >= 1e-6) {
			printf("%.2f+%.2fi", current->real, current->imag);
		} else if (current->imag <= -1e-6) {
			printf("%.2f%.2fi", current->real, current->imag);
		} else {
				printf("%.2f", current->real);
		}
		SetColor(handle, "Default");
		if (i != n_elems - 1) {
			printf("  ");
		}
		SetColor(handle, "YellowBack");
    }
	SetColor(handle, "Default");
    printf("\n");
}


/*
 *
 * <сводка>
 *   Функция ShowRoots
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 *   Показывает корни уравнения
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_ShowRoots(/* in */ struct IEcoLab1Events* me, /* in */ const void* coefs_data, size_t coefs_count, const char* name_type) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (name_type == "INT") {
		PrintRootsInt(coefs_data, coefs_count, &handle);
	}
	else if (name_type == "DOUBLE") {
		PrintRootsDouble(coefs_data, coefs_count, &handle);
	}
	else if (name_type == "COMPLEX") {
		PrintRootsComplex(coefs_data, coefs_count, &handle);
	}

    if (me == 0 ) {
        return -1;
    }


    return 0;
}

/*
 *
 * <сводка>
 *   Функция ShowPoly
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 *   Показывает текущий полином
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_ShowPoly(/* in */ struct IEcoLab1Events* me, /* in */ const void* coefs_data, size_t coefs_count, const char* name_type) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (me == 0 ) {
        return -1;
    }

	if (name_type == "INT") {
		PrintPolyInt(coefs_data, coefs_count, &handle);
	}
	else if (name_type == "DOUBLE") {
		PrintPolyDouble(coefs_data, coefs_count, &handle);
	}
	else if (name_type == "COMPLEX") {
		PrintPolyComplex(coefs_data, coefs_count, &handle);
	}

    return 0;
}

/*
 *
 * <сводка>
 *   Функция ShowPoly
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 *   Показывает текущие коэффициенты при умножении на корень
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_ShowState(/* in */ struct IEcoLab1Events* me, /* in */ const void* roots_data, const void* coefs_data, size_t coefs_count, const char* name_type, size_t root_ind, size_t coef_ind) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (me == 0 ) {
        return -1;
    }

	if (name_type == "INT") {
		SetColor(&handle, "Blue");
		printf("Testing Int:\n");
		SetColor(&handle, "Default");
		PrintRootsIntIter(roots_data, coefs_count - 1, &handle, root_ind);
		PrintCoefsInt(coefs_data, coefs_count, &handle, coef_ind);
	}
	else if (name_type == "DOUBLE") {
		SetColor(&handle, "Blue");
		printf("Testing Double:\n");
		SetColor(&handle, "Default");
		PrintRootsDoubleIter(roots_data, coefs_count - 1, &handle, root_ind);
		PrintCoefsDouble(coefs_data, coefs_count, &handle, coef_ind);
	}
	else if (name_type == "COMPLEX") {
		SetColor(&handle, "Blue");
		printf("Testing Complex:\n");
		SetColor(&handle, "Default");
		PrintRootsComplexIter(roots_data, coefs_count - 1, &handle, root_ind);
		PrintCoefsComplex(coefs_data, coefs_count, &handle, coef_ind);
	}

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Advise
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Advise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint* pCP = 0;
    int16_t result = 0;

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);

    if (result == 0 && pCPC != 0) {
        result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
        pCPC->pVTbl->Release(pCPC);
        pCPC = 0;
        if (result == 0 && pCP != 0) {

            result = pCP->pVTbl->Advise(pCP, (IEcoUnknown*)me, &me->m_cCookie);
            pCP->pVTbl->Release(pCP);
            pCP = 0;
        }
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция Unadvise
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Unadvise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint * pCP = 0;
    int16_t result = 0;

    if (me->m_cCookie) {
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);
        if (result == 0) {
            result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
            pCPC->pVTbl->Release(pCPC);
            pCPC = 0;
            if (result == 0) {
                result = pCP->pVTbl->Unadvise(pCP, me->m_cCookie);
                pCP->pVTbl->Release(pCP);
                pCP = 0;
            }
        }
    }
    return result;
}

/* Create Virtual Table IEcoLab1VTblEvents */
IEcoLab1VTblEvents g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents = {
    CEcoLab1Sink_QueryInterface,
    CEcoLab1Sink_AddRef,
    CEcoLab1Sink_Release,
	CEcoLab1Sink_ShowRoots,
    CEcoLab1Sink_ShowPoly,
	CEcoLab1Sink_ShowState
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1Sink(/* in */ IEcoMemoryAllocator1* pIMem, /* out */ IEcoLab1Events** ppIEcoLab1Events) {
    int16_t result = -1;
    CEcoLab1Sink* pCMe = 0;

    /* Проверка указателей */
    if (ppIEcoLab1Events == 0 || pIMem == 0 ) {
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1Sink*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1Sink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;
    pCMe->m_pIMem->pVTbl->AddRef(pCMe->m_pIMem);

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoP2PEvents */
    pCMe->m_pVTblIEcoLab1Events = &g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents;

    *ppIEcoLab1Events = (IEcoLab1Events*)pCMe;

    return 0;
};

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1Sink(IEcoLab1Events* pIEcoLab1Events) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)pIEcoLab1Events;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1Events != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
};
