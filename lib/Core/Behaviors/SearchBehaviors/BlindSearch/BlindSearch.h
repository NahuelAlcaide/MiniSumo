#pragma once

#include "Behaviors/SearchBehaviors/ISearchBehavior.h"
#include "Motors/IMotorController.h"

class blindSearch : public ISearchBehavior {
public:
    /**
    * @brief Loop de busqueda focalizada.
    *
    * Función que implementa un comportamiento de búsqueda ciega.
    *
    * El robot avanza en una especie de zig-zag, cambiando de dirección de giro cada cierto tiempo.
    *
    * @param data Struct de tipo SensorData con las lecturas de los sensores.
    *
    */
    Status execute(SensorData data) override;

    explicit blindSearch(IMotorController* motorController);
private:
    IMotorController* m_motorController; // Contiene un pointer a la capa de abstracción de los motores
};
