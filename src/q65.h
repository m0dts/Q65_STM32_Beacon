/*
 * q65.h
 *
 *  Created on: Dec 18, 2025
 *      Author: rob
 */

#ifndef INC_Q65_H_
#define INC_Q65_H_


extern uint8_t symbolSequence[];
void gen_q65_symbols(char *msg);

typedef enum {
    // 15-second submodes (0.15s symbol length)
    Q65_15A,
    Q65_15B,
    Q65_15C,

    // 30-second submodes (0.30s symbol length)
    Q65_30A,
    Q65_30B,
    Q65_30C,
    Q65_30D,

    // 60-second submodes (0.60s symbol length)
    Q65_60A,
    Q65_60B,
    Q65_60C,
    Q65_60D,
    Q65_60E,

    // 120-second submodes (1.33s symbol length)
    Q65_120A,
    Q65_120B,
    Q65_120C,
    Q65_120D,
    Q65_120E,

    // 300-second submodes (3.46s symbol length)
    Q65_300A,
    Q65_300B,
    Q65_300C,
    Q65_300D,
    Q65_300E,

    Q65_DEFAULT // Fallback or invalid mode
} SubMode;




typedef struct {
    double symbol_length_s; // Symbol length in seconds (s)
    double tone_spacing_hz; // Tone spacing in Hertz (Hz)
    double cycle_s;	// period for restart of message
} SubmodeParameters;




SubmodeParameters get_submode_parameters(SubMode mode) {
    SubmodeParameters params;

    switch (mode) {
        // --- Q65-15 submodes (0.15 s) ---
        case Q65_15A:
            params.symbol_length_s = 0.15;
            params.tone_spacing_hz = 6.67;
            params.cycle_s = 30;
            break;
        case Q65_15B:
            params.symbol_length_s = 0.15;
            params.tone_spacing_hz = 13.33;
            params.cycle_s = 30;
            break;
        case Q65_15C:
            params.symbol_length_s = 0.15;
            params.tone_spacing_hz = 26.67;
            params.cycle_s = 30;
            break;

        // --- Q65-30 submodes (0.30 s) ---
        case Q65_30A:
            params.symbol_length_s = 0.30;
            params.tone_spacing_hz = 3.33;
            params.cycle_s = 60;
            break;
        case Q65_30B:
            params.symbol_length_s = 0.30;
            params.tone_spacing_hz = 6.67;
            params.cycle_s = 60;
            break;
        case Q65_30C:
            params.symbol_length_s = 0.30;
            params.tone_spacing_hz = 13.33;
            params.cycle_s = 60;
            break;
        case Q65_30D:
            params.symbol_length_s = 0.30;
            params.tone_spacing_hz = 26.67;
            params.cycle_s = 60;
            break;

        // --- Q65-60 submodes (0.60 s) ---
        case Q65_60A:
            params.symbol_length_s = 0.60;
            params.tone_spacing_hz = 1.67;
            params.cycle_s = 120;
            break;
        case Q65_60B:
            params.symbol_length_s = 0.60;
            params.tone_spacing_hz = 3.33;
            params.cycle_s = 120;
            break;
        case Q65_60C:
            params.symbol_length_s = 0.60;
            params.tone_spacing_hz = 6.67;
            params.cycle_s = 120;
            break;
        case Q65_60D:
            params.symbol_length_s = 0.60;
            params.tone_spacing_hz = 13.33;
            params.cycle_s = 120;
            break;
        case Q65_60E:
            params.symbol_length_s = 0.60;
            params.tone_spacing_hz = 26.67;
            params.cycle_s = 120;
            break;

        // --- Q65-120 submodes (1.33 s) ---
        case Q65_120A:
            params.symbol_length_s = 1.33333;
            params.tone_spacing_hz = 0.75;
            params.cycle_s = 240;
            break;
        case Q65_120B:
            params.symbol_length_s = 1.33333;
            params.tone_spacing_hz = 1.50;
            params.cycle_s = 240;
            break;
        case Q65_120C:
            params.symbol_length_s = 1.33333;
            params.tone_spacing_hz = 3.00;
            params.cycle_s = 240;
            break;
        case Q65_120D:
            params.symbol_length_s = 1.33333;
            params.tone_spacing_hz = 6.00;
            params.cycle_s = 240;
            break;
        case Q65_120E:
            params.symbol_length_s = 1.33333;
            params.tone_spacing_hz = 12.0;
            params.cycle_s = 240;
            break;

        // --- Q65-300 submodes (3.46 s) ---
        case Q65_300A:
            params.symbol_length_s = 3.4565;
            params.tone_spacing_hz = 0.292;
            params.cycle_s = 600;
            break;
        case Q65_300B:
            params.symbol_length_s = 3.4565;
            params.tone_spacing_hz = 0.546;
            params.cycle_s = 600;
            break;
        case Q65_300C:
            params.symbol_length_s = 3.4565;
            params.tone_spacing_hz = 1.16;
            params.cycle_s = 600;
            break;
        case Q65_300D:
            params.symbol_length_s = 3.4565;
            params.tone_spacing_hz = 2.31;
            params.cycle_s = 600;
            break;
        case Q65_300E:
            params.symbol_length_s = 3.4565;
            params.tone_spacing_hz = 4.63;
            params.cycle_s = 600;
            break;

        case Q65_DEFAULT:
        default:
            params.symbol_length_s = 3.4565;
            params.tone_spacing_hz = 6.67;
            params.cycle_s = 30;
            break;
    }

    return params;
}





#endif /* INC_Q65_H_ */
