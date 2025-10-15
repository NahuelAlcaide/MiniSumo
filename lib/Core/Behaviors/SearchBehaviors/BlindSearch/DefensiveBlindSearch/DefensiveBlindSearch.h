#pragma once
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"

class DefensiveBlindSearch : public BlindSearch {
public:
    /**
    * @brief Loop de busqueda ciega defensiva.
    *
    * Función que implementa un comportamiento de búsqueda ciega.
    *
    * El robot mantiene su posición realizando rotaciones de lado a lado.
    *
    * @param data Struct de tipo SensorData con las lecturas de los sensores.
    *
    */
    Status execute(SensorData data) override;
    explicit DefensiveBlindSearch(IMotorController* motorController);
};