/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"

/*Testing headers*/
#include "EcoLab1TestInt.h"
#include "EcoLab1TestFloat.h"
#include "EcoLab1TestDouble.h"
#include "EcoLab1TestCmplx.h"
#include "EcoLab2TestOperations.h"

void TestPoly(IEcoMemoryAllocator1 *pIMem, IEcoLab1* lab1) {
    // 5 roots
    WriteToFileIntTestResults(pIMem, lab1, 5);
    WriteToFileFloatTestResults(pIMem, lab1, 5);
    WriteToFileDoubleTestResults(pIMem, lab1, 5);
    WriteToFileComplexTestResults(pIMem, lab1, 5);
}

void TimeTestPoly(IEcoMemoryAllocator1 *pIMem, IEcoLab1* lab1) {
	size_t i;
    for (i = 0; i <= 10000; i += 1000) {
        WriteTimeToFileInt(pIMem, lab1, i);
        WriteTimeToFileFloat(pIMem, lab1, i);
        WriteTimeToFileDouble(pIMem, lab1, i);
        WriteTimeToFileComplex(pIMem, lab1, i);
    }
}


/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* name = 0;
    char_t* copyName = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;
	/* Указатели на интерфейсы */
    //IEcoCalculatorX* pIEcoCalculatorX = 0;
    //IEcoCalculatorY* pIEcoCalculatorY = 0;
	//int16_t x = 20;
	//int16_t y = 5;
	//int16_t foo_res;
	///* Генерируем данные для тестирования операций */
	//Data testing_data;
	//GetRandomData(&testing_data);

	/* Указатель на интерфейс контейнера точек подключения */
    IEcoConnectionPointContainer* pICPC = 0;
    /* Указатель на интерфейс точки подключения */
    IEcoConnectionPoint* pICP = 0;
    /* Указатель на обратный интерфейс */
    IEcoLab1Events* pIEcoLab1Sink = 0;
    IEcoUnknown* pISinkUnk = 0;
    uint32_t cAdvise = 0;
	size_t n_roots = 3;
	void* roots_ptr;
	void* coefs;


    /* Проверка и создание системного интрефейса */
	printf("Create interface...");
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }
	printf("OK\n");

    /* Получение интерфейса для работы с интерфейсной шиной */
	printf("Getting bus interface...");
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
	printf("OK\n");
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
	printf("Getting memorymanager interface...");
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }
	printf("OK\n");

	printf("Getting test interface...");
    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }
	printf("OK\n");

	/* Проверка поддержки подключений обратного интерфейса */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void **)&pICPC);
    if (result != 0 || pICPC == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

	/* Запрос на получения интерфейса точки подключения */
    result = pICPC->pVTbl->FindConnectionPoint(pICPC, &IID_IEcoLab1Events, &pICP);
    if (result != 0 || pICP == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }
    /* Освобождение интерфейса */
    pICPC->pVTbl->Release(pICPC);

	/* Создание экземпляра обратного интерфейса */
    result = createCEcoLab1Sink(pIMem, (IEcoLab1Events**)&pIEcoLab1Sink);

	if (pIEcoLab1Sink != 0) {
        result = pIEcoLab1Sink->pVTbl->QueryInterface(pIEcoLab1Sink, &IID_IEcoUnknown,(void **)&pISinkUnk);
        if (result != 0 || pISinkUnk == 0) {
            /* Освобождение интерфейсов в случае ошибки */
            goto Release;
        }
        /* Подключение */
        result = pICP->pVTbl->Advise(pICP, pISinkUnk, &cAdvise);
        /* Проверка */
        if (result == 0 && cAdvise == 1) {
            /* Сюда можно добавить код */
        }
        /* Освобождение интерфейса */
        pISinkUnk->pVTbl->Release(pISinkUnk);
    }

	printf("\nTesting Poly:\n");

	roots_ptr = CreateRootsIntArray(pIMem, n_roots);
	coefs = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(int));
	pIEcoLab1->pVTbl->poly(pIEcoLab1, roots_ptr, n_roots, sizeof(int), MultInt, SubInt, InitIntOnes, "INT", coefs);
	pIMem->pVTbl->Free(pIMem, coefs);
	DeleteIntArray(pIMem, roots_ptr);

	roots_ptr = CreateRootsDoubleArray(pIMem, n_roots);
	coefs = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(double));
	pIEcoLab1->pVTbl->poly(pIEcoLab1, roots_ptr, n_roots, sizeof(double), MultDouble, SubDouble, InitDoubleOnes, "DOUBLE", coefs);
	pIMem->pVTbl->Free(pIMem, coefs);
	DeleteDoubleArray(pIMem, roots_ptr);

	roots_ptr = CreateRootsComplexArray(pIMem, n_roots);
	coefs = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(Complex));
	pIEcoLab1->pVTbl->poly(pIEcoLab1, roots_ptr, n_roots, sizeof(Complex), MultComplex, SubComplex, InitComplexOnes, "COMPLEX", coefs);
	pIMem->pVTbl->Free(pIMem, coefs);
	DeleteComplexArray(pIMem, roots_ptr);

	if (pIEcoLab1Sink != 0) {
        /* Отключение */
        result = pICP->pVTbl->Unadvise(pICP, cAdvise);
        pIEcoLab1Sink->pVTbl->Release(pIEcoLab1Sink);
        pICP->pVTbl->Release(pICP);
    }


	//printf("\nTesting interfaces:\n\n");

	//printf("Query IEcoCalculatorX from Lab1...");
 //   result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
 //   if (result != 0) {
 //       goto Release;
 //   }
	//printf("OK\n");
 //   pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

	//printf("Query IEcoCalculatorY from Lab1...");
 //   result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
 //   if (result != 0) {
 //       goto Release;
 //   }
	//printf("OK\n");
 //   pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);

	//printf("Query IEcoCalculatorX from IEcoCalculatorY...");
	//result = pIEcoCalculatorY->pVTbl->QueryInterface(pIEcoCalculatorY, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
	//if (result != 0) {
 //       goto Release;
 //   }
	//printf("OK\n");
 //   pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

	//printf("Query IEcoCalculatorY from IEcoCalculatorX...");
	//result = pIEcoCalculatorX->pVTbl->QueryInterface(pIEcoCalculatorX, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
	//if (result != 0) {
 //       goto Release;
 //   }
	//printf("OK\n");
 //   pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);


	//printf("\nTesting operations:\n\n");

	///* запрос интерфейса IEcoCalculatorX через IEcoLab1 */
	//printf("Query interface IEcoCalculatorX via IEcoLab1...");
 //   result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
 //   if (result != 0 || pIEcoCalculatorX == 0) {
 //       goto Release;
 //   }
	//printf("OK\n");

	//printf("-------------------\n");
	//printf("Addition test\nInput: %d + %d\nExpected: %d\n", testing_data.x, testing_data.y, testing_data.add_result);
	//if (CheckResult(pIEcoCalculatorX->pVTbl->Addition(pIEcoCalculatorX, testing_data.x, testing_data.y), testing_data.add_result)) {
	//	printf("\nCorrect!\n");
	//} else {
	//	printf("\nIncorrect!\n");
	//	printf("Returned: %d\n", pIEcoCalculatorX->pVTbl->Addition(pIEcoCalculatorX, testing_data.x, testing_data.y));
	//}

	//printf("-------------------\n");
	//printf("Subtraction test\nInput: %d - %d\nExpected: %d\n", testing_data.x, testing_data.y, testing_data.sub_result);
	//if (CheckResult(pIEcoCalculatorX->pVTbl->Subtraction(pIEcoCalculatorX, testing_data.x, testing_data.y), testing_data.sub_result)) {
	//	printf("\nCorrect!\n");
	//} else {
	//	printf("\nIncorrect!\n");
	//	printf("Returned: %d\n", pIEcoCalculatorX->pVTbl->Subtraction(pIEcoCalculatorX, testing_data.x, testing_data.y));
	//}
 //   pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

	// /* запрос интерфейса IEcoCalculatorY через IEcoLab1 */
	//printf("\n\nQuery interface IEcoCalculatorY via IEcoLab1...");
 //   result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
 //   if (result != 0 || pIEcoCalculatorY == 0) {
 //       goto Release;
 //   }
	//printf("OK\n");

	//printf("-------------------\n");
	//printf("Multiplication test\nInput: %d * %d\nExpected: %d\n", testing_data.x, testing_data.y, testing_data.mul_result);
	//if (CheckResult(pIEcoCalculatorY->pVTbl->Multiplication(pIEcoCalculatorY, testing_data.x, testing_data.y), testing_data.mul_result)) {
	//	printf("\nCorrect!\n");
	//} else {
	//	printf("\nIncorrect!\n");
	//	printf("Returned: %d\n", pIEcoCalculatorY->pVTbl->Multiplication(pIEcoCalculatorY, testing_data.x, testing_data.y));
	//}

	//printf("-------------------\n");
	//printf("Division test\nInput: %d / %d\nExpected: %d\n", testing_data.x, testing_data.y, testing_data.div_result);
	//if (CheckResult(pIEcoCalculatorY->pVTbl->Division(pIEcoCalculatorY, testing_data.x, testing_data.y), testing_data.div_result)) {
	//	printf("\nCorrect!\n");
	//} else {
	//	printf("\nIncorrect!\n");
	//	printf("Returned: %d\n", pIEcoCalculatorY->pVTbl->Division(pIEcoCalculatorY, testing_data.x, testing_data.y));
	//}
 //   pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);


	/*PrintIntTestResults(pIMem, pIEcoLab1, 5);*/

	/* Запись в файл */
    //TestPoly(pIMem, pIEcoLab1);
    //TimeTestPoly(pIMem, pIEcoLab1);




    /* Освлбождение блока памяти */
    pIMem->pVTbl->Free(pIMem, name);

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

