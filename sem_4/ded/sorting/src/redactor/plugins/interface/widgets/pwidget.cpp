#include "pwidget.h"
#include "utils/logger.h"
#include "engine/Resources.h"
#include "engine/moga_engine.h"


FWD_ALL_FOR_CLASS_(WidgetView)


WidgetView::WidgetView(const ViewBody &body, PUPPY::Widget *widget) :
v_Highlighter(body, nullptr, false),
widget(widget)
{
    INIT_FWD_TO_WIDGET(WidgetView);
    set_focuseable(true);
}

WidgetView::~WidgetView() {
    if (to_delete_widget) {
        delete widget;
    }
}


PluginWidget::PluginWidget(const ViewBody &body, PUPPY::Widget *parent, bool to_spawn_widget) :
Widget(),
view(to_spawn_widget ? new WidgetView(body, this) : nullptr),
body(to_wbody(body)), parent(parent), texture(nullptr)
{
    if (parent) {
        parent->add_child(this);
    }
}

PluginWidget::PluginWidget(Widget *widget) :
Widget(),
view(nullptr),
body(widget->get_body()), parent(widget->get_parent()), texture(widget->get_texture())
{
    ViewBody body = {
        {widget->get_body().position.x, widget->get_body().position.y},
        {widget->get_body().size.x, widget->get_body().size.y}
    };

    view = new WidgetView(body, widget);
}

PluginWidget::~PluginWidget() {
    view->to_delete = true;
}

bool PluginWidget::is_active() {
    return view->is_active();
}

bool PluginWidget::is_inside(const PUPPY::Vec2f &pos) {
    return view->is_inside({pos.x, pos.y});
}


bool PluginWidget::add_child(PUPPY::Widget *child) {
    if (!child || !view) return false;

    PluginWidget *app_widget = dynamic_cast<PluginWidget*>(child);
    if (!app_widget) {
        // app_widget = new PluginWidget(child);
        ViewBody body = {
            {child->get_body().position.x, child->get_body().position.y},
            {child->get_body().size.x, child->get_body().size.y}
        };
        auto new_view = new WidgetView(body, child);
        view->add_subview(new_view);
        return true;
    }

    return add_child(app_widget);
}

bool PluginWidget::add_child(PluginWidget *child) {
    if (!child || !view || !child->get_view()) return false;

    view->add_subview(child->get_view());
    child->get_view()->focus();
    return true;
}

bool PluginWidget::delete_child(PluginWidget */*child*/) {
    return false;
}

bool PluginWidget::delete_child(Widget */*child*/) {
    logger.error("PluginWidget", "delete_child is not implemented");
    return false;
}


bool PluginWidget::delete_from_parent() {
    logger.error("PluginWidget", "delete_from_parent is not implemented");
    return false;
}

void PluginWidget::hide() {
    view->set_active(false);
}

void PluginWidget::show() {
    view->set_active(true);
}

void PluginWidget::focus() {
    view->focus();
}

void PluginWidget::set_caption(const char *text, size_t font_size, const PUPPY::Vec2f */*pos*/) {
    view->add_label(text, font_size, App.font.color.basic_header, 0, true);
}

void PluginWidget::set_base_color(const PUPPY::RGBA &color) {
    view->set_appearence(App.add_appr(new AppearenceColor(color.ui32)));
}
