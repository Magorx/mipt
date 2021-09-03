float parabosina(float x) {
    return x * x * sin(x) / 5;
}

float func(float x) {
    return x * sin(5 / x);
}

float param_x(float t) {
    return 3 * cos(cos(7.32 * round(t))) * 1.2 * (1 + cos(16.6 * t));
}

float param_y(float t) {
    float tmp = sin(16.6 * t);
    return 3 * tmp * tmp * sin(7.32 * t);
}

float param_spiral_x(float t) {
    return 0.75 * exp(t) * cos(5 * t);
}

float param_spiral_y(float t) {
    return 1 * exp(t) * sin(5 * t);
}

sf::Color param_color(float t) {
    return sf::Color(fabs(255 * sin(t)), ((int)(200 * t)) % 255, 200);
}
