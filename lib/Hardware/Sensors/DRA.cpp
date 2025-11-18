#include <Arduino.h>
#include <avr/io.h>
#include "Config.h"
#include "DRA.h"

// TODO: Checkear y corregir comentarios de Gemini

// --- Configuración del Prescaler del ADC ---
//
// Clock ATmega328p = 16 MHz
// Clock ADC = 16 MHz / 32 = 500 KHz
// Una conversión toma 13 ciclos del ADC.
// Tiempo de Conversión = 13 * (1 / 500,000 MHz) = 26 microsegundos
//
// ADPS2=1, ADPS1=0, ADPS0=1 -> 101 -> Prescaler 32
#define ADC_PRESCALER_BITS ((1 << ADPS2) | (1 << ADPS0))

// --- Checks de Tiempos (Compile-time) ---

// Este cálculo determina el último momento en el que se puede
// iniciar la conversión del ADC para que termine (con margen)
// antes de que se cumpla el tiempo del puslo del LED.
constexpr uint32_t LATEST_POSSIBLE_ADC_START_US =
    (LED_PULSE_WIDTH > (ADC_CONV_US + ADC_MARGIN_US))
    ? (LED_PULSE_WIDTH - (ADC_CONV_US + ADC_MARGIN_US))
    : 0u;

static_assert(LED_PULSE_WIDTH >= (ADC_CONV_US + ADC_MARGIN_US),
              "LED_PULSE_WIDTH es muy corto para la conversión ADC + margen.");

static_assert(SAMPLE_DELAY_US < LATEST_POSSIBLE_ADC_START_US,
    "SAMPLE_DELAY_ON_US es muy largo. La conversión ADC no va a terminar antes de que el LED se apague.");

// Calcula el tiempo restante de espera después de la muestra para asegurar que el LED esté encendido exactamente LED_PULSE_WIDTH.
constexpr uint32_t REMAINING_ON = LED_PULSE_WIDTH - (SAMPLE_DELAY_US + ADC_CONV_US);

// --- Helpers: Acceso rápido a pines ---
// Obtiene la dirección del registro para escrituras digitales rápidas
static inline volatile uint8_t *portOutputRegForPin(uint8_t pin) {
    uint8_t port = digitalPinToPort(pin);
    return port ? portOutputRegister(port) : nullptr;
}
// Obtiene la máscara de bit para escrituras digitales rápidas
static inline uint8_t bitMaskForPin(uint8_t pin) {
    return digitalPinToBitMask(pin);
}
/**
 * @brief Convierte un pin analógico de Arduino (A0-A5) a su
 * canal ADC (0-5).
 */
static inline uint8_t analogPinToAdcChannel(uint8_t analogPin) {
    return static_cast<uint8_t>(analogPin - A0);
}

// --- Funciones de Ayuda del ADC ---

/**
 * @brief Lee un canal específico del ADC en modo bloqueante.
 * @note Esta función es "segura" y re-establece el prescaler en cada
 * llamada, para evitar conflictos con analogRead().
 * @param ch Canal ADC (0-7)
 * @return Resultado ADC de 10 bits
 */
static inline uint16_t adcReadChannel_blocking(uint8_t ch) {
    // 1. Seleccionar canal
    //    REFS0 se asume configurado en setup() (usando AVcc)
    //    Limpiamos los bits MUX (MUX0-MUX3) y asignamos el nuevo canal.
    ch &= 0x07; // Asegurarse que el canal es 0-7
    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);

    // 2. Iniciar conversión (Modo "Seguro")
    //    Establecemos TODOS los bits necesarios en cada llamada.
    //    Esto (re)habilita el ADC (ADEN), inicia la conversión (ADSC),
    //    y, lo más importante, **fuerza el prescaler a 16** (ADC_PRESCALER_BITS),
    //    sobrescribiendo cualquier valor que analogRead() haya podido dejar.
    ADCSRA = (1 << ADEN) | (1 << ADSC) | ADC_PRESCALER_BITS;

    // 3. Esperar a que la conversión se complete
    //    El hardware limpia el bit ADSC automáticamente al terminar.
    while (ADCSRA & (1 << ADSC));

    // 4. Leer el resultado de 10 bits
    //    Leer 'ADC' (el macro) lee ADCL y ADCH en el orden correcto.
    return ADC;
}

// --- Funciones Públicas ---

/**
 * @brief Inicializa el ADC para conversiones manuales rápidas.
 */
void draSetup() {
    // 1. Configurar Registro de Mux del ADC (ADMUX)
    //    (1 << REFS0): Usar AVcc como voltaje de referencia.
    //    ADLAR = 0 (default): Resultado de 10 bits ajustado a la derecha.
    ADMUX = (1 << REFS0);

    // 2. Configurar Registro de Control A (ADCSRA)
    //    (1 << ADEN): Habilitar el ADC.
    //    ADC_PRESCALER_BITS: Establecer el prescaler (definido arriba).
    ADCSRA = (1 << ADEN) | ADC_PRESCALER_BITS;

    // 3. Deshabilitar Buffers de Entrada Digital (DIDR0)
    //    Optimización para ahorrar energía y reducir ruido.
    //    Desconecta la lógica digital de los pines usados como analógicos.
    DIDR0 |= (1 << analogPinToAdcChannel(RIGHT_SENSOR_PIN)) |
             (1 << analogPinToAdcChannel(CENTER_SENSOR_PIN)) |
             (1 << analogPinToAdcChannel(LEFT_SENSOR_PIN));

    // 4. Dejar que el ADC se estabilice (opcional pero recomendado)
    delay(2);
}

/**
 * @brief Realiza una medición de pulso IR diferencial.
 * @return Valor "limpio" (Muestra ON - Muestra OFF)
 */
int pulse(uint8_t sensorAnalogPin, uint8_t ledDigitalPin) {
    uint8_t adcChan = analogPinToAdcChannel(sensorAnalogPin);
    volatile uint8_t *portOut = portOutputRegForPin(ledDigitalPin);
    uint8_t mask = bitMaskForPin(ledDigitalPin);

    // 1. Esperar un tiempo de seguridad para evitar crosstalk entre sensores
    delayMicroseconds(AMBIENT_SAMPLE_DELAY_US);

    // 2. Tomar muestra 'OFF' (Ambiente)
    uint16_t sampleOff = adcReadChannel_blocking(adcChan);

    // 3. Encender LED
    if (portOut) *portOut |= mask; // Inicio del pulso

    // 4. Esperar el tiempo de retardo antes de la muestra
    //    Este delay ocurre *dentro* del período en que el LED está encendido.
    delayMicroseconds(SAMPLE_DELAY_US);

    // 5. Iniciar la conversión 'ON' (mientras el LED sigue ON)
    //    La función se bloqueará hasta que la conversión termine.
    uint16_t sampleOn = adcReadChannel_blocking(adcChan);

    // 6. Esperar el resto del ancho de pulso
    //    Esto asegura que el LED esté encendido por *exactamente*
    //    LED_PULSE_WIDTH microsegundos.
    delayMicroseconds(REMAINING_ON);

    // 7. Apagar LED
    if (portOut) *portOut &= ~mask; // Fin del pulso

    // 8. Calcular valor diferencial
    int denoised = static_cast<int>(sampleOn) - static_cast<int>(sampleOff);
    if (denoised < 0) denoised = 0; // Prevenir lecturas negativas
    return denoised;
}