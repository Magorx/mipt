#include "redactor/plugin_std/plugin_std.hpp"
#include "utils.h"

#include "kctf_widget_library.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = PUPPY::EXTENSION;

const char *PNAME    = "Widget Library Extension";
const char *PVERSION = "69";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "The most Awasomest WidGetS ever in my humble factory";

// ============================================================================ Resources

SuperWidgetFactory *r_super_factory = nullptr;

// ============================================================================ General

#include "plugin_interface.h"
 
// ============================================================================ Logic

class MyRadioButton : public RadioButton {
    bool on;
    PUPPY::RenderTarget *one;
    PUPPY::RenderTarget *two;

    const PUPPY::AppInterface *APPI;

    HandlerType handler;

public:
    MyRadioButton(const PUPPY::AppInterface *app_interface, const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr, PUPPY::RGBA first = {200, 100, 100}, PUPPY::RGBA second = {100, 200, 100});

    virtual ~MyRadioButton();

    virtual void on_mouse_press(const PUPPY::Event::MousePress &event) override;

    virtual bool get_on() const override;
    virtual void set_on(bool flag) override;

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }
};

MyRadioButton::MyRadioButton(const PUPPY::AppInterface *app_interface, const PUPPY::WBody &body, PUPPY::Widget *parent, PUPPY::RGBA first, PUPPY::RGBA second) :
RadioButton(body, parent),
on(false),
one(nullptr),
two(nullptr),
APPI(app_interface)
{
    one = APPI->factory.target->spawn({(size_t) body.size.x, (size_t) body.size.y}, second);
    two = APPI->factory.target->spawn({(size_t) body.size.x, (size_t) body.size.y}, first);

    set_on(on);
}

MyRadioButton::~MyRadioButton() {
    delete one;
    delete two;
}

void MyRadioButton::on_mouse_press(const PUPPY::Event::MousePress &event) {
    if (!is_inside(event.position)) {
        return;
    }

    set_on(!on);
    if (handler) {
        handler();
    }
}

bool MyRadioButton::get_on() const {
    return on;
}

void MyRadioButton::set_on(bool flag) {
    on = flag;
    if (on) {
        texture = one;
    } else {
        texture = two;
    }
}

struct MySuperWidgetFactory : public SuperWidgetFactory {
    virtual RadioButton *radio_button(const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr, PUPPY::RGBA first = {200, 100, 100}, PUPPY::RGBA second = {100, 200, 100}) override {
        return new MyRadioButton(APPI, body, parent, first, second);
    }
};

PUPPY::Status MyPluginInterface::init(const PUPPY::AppInterface *app_interface, const std::filesystem::path&) {
    srand(time(NULL));

    APPI = app_interface;

    r_super_factory = new MySuperWidgetFactory;

    APPI->ext_register_as("KCTFSuperWidget");
    PINTERFACE.exts.set("SuperWidgetFactory", r_super_factory);

    APPI->log("[plugin](%s) inited", PINFO.name);
    return PUPPY::OK;
}

PUPPY::Status MyPluginInterface::deinit() {
    delete r_super_factory;

    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return PUPPY::OK;
}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const PUPPY::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double /*dt*/) const {}

void MyPluginInterface::tool_on_press(const PUPPY::Vec2f &/*pos*/) const {}

void MyPluginInterface::tool_on_move(const PUPPY::Vec2f &/*from*/, const PUPPY::Vec2f &/*to*/) const {}

void MyPluginInterface::tool_on_release(const PUPPY::Vec2f &/*pos*/) const {}

void MyPluginInterface::effect_apply() const {}

inline unsigned long long read_next_long_long(const char **buffer) {
    const char *c = *buffer;
    while (c && (*c == ' ' || *c == '\n')) ++c;
 
    unsigned long long l = 0;
    while (*c >= '0' && *c <= '9') {
        l = l * 10 + *c - '0';
        ++c;
    }
 
    *buffer = c;
    return l;
}

unsigned long long read(const char *text) {
    unsigned long long wanted_size = read_next_long_long(&text);
    return wanted_size;
}

void MyPluginInterface::show_settings() const {}


typedef void (*generate_triangle_type)(const PUPPY::Vec2f &pos, float radius, PUPPY::Vec2f &p1, PUPPY::Vec2f &p2, PUPPY::Vec2f &p3);

void MyPluginInterface::draw(const PUPPY::Vec2f &/*pos*/) const {}
