#pragma once

#include "Sensors/Sensors.h"
#include "Behaviors/SearchBehaviors/ISearchBehavior.h"
#include "Motors/IMotorController.h"

class FocalizedSearch : public ISearchBehavior {
public:

    enum lastDirection {
        NONE,
        RIGHT,
        CENTER,
        LEFT,
    };

    /**
    * @brief Loop de busqueda focalizada.
    *
    * Función que implementa un comportamiento de búsqueda focalizada.
    *
    * El robot avanza en linea recta o gira en una dirección basándose en las lecturas de los sensores.
    *
    * Al momento de ser llamada, la función activa un state_hold, liberandolo luego de un timeout dictaminado por la variable de <b>entorno SEEK_TURN_TIMEOUT</b>
    *
    * @param data Struct de tipo SensorData con las lecturas de los sensores.
    *
    */
    Status execute(SensorData data) override;

    explicit FocalizedSearch(IMotorController* motorController);
private:
    IMotorController* m_motorController; // Contiene un pointer a la capa de abstracción de los motores
};
