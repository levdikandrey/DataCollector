#ifndef APROTOCOL_H
#define APROTOCOL_H

#include <cstdint>

namespace  AProtocol
{
    /**
     * @brief The command enum - Команды протокола
     */
    enum class command
    {
        analysisAVP = 1,        // Анализ одного АВП
        analysisManyAVP = 2     // Анализ нескольких АВП
    };
    /**
     * @brief The header struct - Заголовок сообщений
     */
#pragma pack(push, 1)
    struct header
    {
        uint8_t command;        // Номер команды
        uint16_t length;        // Длина команды. Содержит длинну команды без заголовка
    };
    /**
     * @brief The commandAnalysis struct - Команда. Анализ одного АВП
     */
    struct commandAnalysis : header
    {
        uint64_t idAVP;         // ID АВП в БД АВП
    };
    /**
     * @brief The commandAnalysisManyAPV struct - Команда. Анализ нескольких АВП
     */
    struct commandAnalysisManyAPV : header
    {
        uint16_t coountAVP;     // Количество АВП для анализа
        const char* listAVP;    // Список идентификаторов АВП (coountAVP * 8)
    };
#pragma pack(pop)

};

#endif // APROTOCOL_H
