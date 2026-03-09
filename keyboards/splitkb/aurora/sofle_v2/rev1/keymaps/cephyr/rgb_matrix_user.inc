RGB_MATRIX_EFFECT(cycling_starlight)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static uint8_t phase_offsets[RGB_MATRIX_LED_COUNT];

static void cycling_starlight_init(effect_params_t* params) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        phase_offsets[i] = random8();
    }
}

hsv_t cycling_starlight_run(hsv_t hsv, uint8_t i, uint8_t time) {
    hsv.v = scale8(abs8(sin8((time * 2 + phase_offsets[i]) / 2) - 128) * 2, hsv.v);
    uint8_t wave = (g_led_config.point[i].y - time);
    if (wave > 127) {
        wave = 255 - wave;
    }
    hsv.h += scale8(wave, 196);
    return hsv;
}

static bool cycling_starlight(effect_params_t* params) {
    if (params->init) cycling_starlight_init(params);
    return effect_runner_i(params, &cycling_starlight_run);
}

#endif
