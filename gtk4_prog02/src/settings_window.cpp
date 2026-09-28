#include "settings_window.h"
#include "main_window.h"

SettingsWindow::SettingsWindow(GtkWindow *parent, IMainWindow *main_window) :
m_main_window(main_window)
{
    m_builder = gtk_builder_new_from_resource(
        "/com/example/gtk4demo/ui/settings_window.ui");
    m_widget = GTK_WIDGET(gtk_builder_get_object(m_builder, "settings_window"));
    gtk_window_set_transient_for(GTK_WINDOW(m_widget), parent);

    m_sw_dark  = GTK_SWITCH(gtk_builder_get_object(m_builder, "switch_dark_theme"));
    m_sw_notif = GTK_SWITCH(gtk_builder_get_object(m_builder, "switch_notifications"));
    m_sw_save  = GTK_SWITCH(gtk_builder_get_object(m_builder, "switch_autosave"));
    m_en_test = GTK_ENTRY(gtk_builder_get_object(m_builder, "edit_test"));

    g_signal_connect(m_sw_dark,  "notify::active", G_CALLBACK(on_sw_dark_notify),  this);
    g_signal_connect(m_sw_notif, "notify::active", G_CALLBACK(on_sw_notif_notify), this);
    g_signal_connect(m_sw_save,  "notify::active", G_CALLBACK(on_sw_save_notify),  this);

    GtkButton *btn_apply  = GTK_BUTTON(gtk_builder_get_object(m_builder, "btn_apply"));
    GtkButton *btn_cancel = GTK_BUTTON(gtk_builder_get_object(m_builder, "btn_cancel"));
    g_signal_connect(btn_apply,  "clicked", G_CALLBACK(on_btn_apply),  this);
    g_signal_connect(btn_cancel, "clicked", G_CALLBACK(on_btn_cancel), this);
}

SettingsWindow::~SettingsWindow()
{
    if (m_builder) g_object_unref(m_builder);
}

void SettingsWindow::present()
{
    sync_switches_to_state();
    gtk_window_present(GTK_WINDOW(m_widget));
}

void SettingsWindow::setTestEntry(const std::string &test_entry)
{
    if (m_en_test == nullptr) return;
    gtk_editable_set_text(GTK_EDITABLE(m_en_test), test_entry.c_str());
}

void SettingsWindow::sync_switches_to_state()
{
    m_syncing = true;
    gtk_switch_set_active(m_sw_dark,  m_dark_theme);
    gtk_switch_set_active(m_sw_notif, m_notifications);
    gtk_switch_set_active(m_sw_save,  m_autosave);
    m_syncing = false;
}

void SettingsWindow::on_switch_changed(GtkSwitch *sw, const char *name)
{
    if (m_syncing) return;
    g_print("[Settings] %s → %s (pending)\n",
            name, gtk_switch_get_active(sw) ? "on" : "off");
}

void SettingsWindow::on_apply()
{
    m_dark_theme    = gtk_switch_get_active(m_sw_dark);
    m_notifications = gtk_switch_get_active(m_sw_notif);
    m_autosave      = gtk_switch_get_active(m_sw_save);
    const char* txt = gtk_editable_get_text(GTK_EDITABLE(m_en_test));
    m_test_entry = std::string(txt);

    g_print("[Settings] Applied — dark:%d  notifications:%d  autosave:%d, test:%s\n",
            m_dark_theme, m_notifications, m_autosave, m_test_entry.c_str());
    if (m_main_window != nullptr) {
        m_main_window->setParams(m_test_entry);
    }
    else {
        g_print("Interface to main was not installed\n");
    }

    gtk_window_close(GTK_WINDOW(m_widget));
}

void SettingsWindow::on_cancel()
{
    g_print("[Settings] Cancelled\n");
    gtk_window_close(GTK_WINDOW(m_widget));
}

// ── Static signal trampolines ─────────────────────────────────────────────────

void SettingsWindow::on_sw_dark_notify(GObject *obj, GParamSpec *, gpointer self)
{
    static_cast<SettingsWindow *>(self)->on_switch_changed(GTK_SWITCH(obj), "dark-theme");
}

void SettingsWindow::on_sw_notif_notify(GObject *obj, GParamSpec *, gpointer self)
{
    static_cast<SettingsWindow *>(self)->on_switch_changed(GTK_SWITCH(obj), "notifications");
}

void SettingsWindow::on_sw_save_notify(GObject *obj, GParamSpec *, gpointer self)
{
    static_cast<SettingsWindow *>(self)->on_switch_changed(GTK_SWITCH(obj), "autosave");
}

void SettingsWindow::on_btn_apply(GtkButton *, gpointer self)
{
    static_cast<SettingsWindow *>(self)->on_apply();
}

void SettingsWindow::on_btn_cancel(GtkButton *, gpointer self)
{
    static_cast<SettingsWindow *>(self)->on_cancel();
}