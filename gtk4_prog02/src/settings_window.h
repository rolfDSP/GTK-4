#pragma once
#include <string>
#include <gtk/gtk.h>
class IMainWindow;  // forward declaration — full definition in settings_window.cpp

class SettingsWindow {
public:
    explicit SettingsWindow(GtkWindow *parent, IMainWindow *main_window = nullptr);
    ~SettingsWindow();

    // Show the window; switches are reset to the last applied state.
    void present();

    // Committed (applied) settings, readable by the rest of the app.
    bool dark_theme()    const { return m_dark_theme; }
    bool notifications() const { return m_notifications; }
    bool autosave()      const { return m_autosave; }

    void setTestEntry(const std::string &test_entry);

private:
    GtkWidget  *m_widget   = nullptr;
    GtkBuilder *m_builder  = nullptr;

    GtkSwitch  *m_sw_dark  = nullptr;
    GtkSwitch  *m_sw_notif = nullptr;
    GtkSwitch  *m_sw_save  = nullptr;
    GtkEntry  *m_en_test  = nullptr;

    // Committed state — updated only when the user clicks Apply.
    bool m_dark_theme    = false;
    bool m_notifications = true;
    bool m_autosave      = true;
    std::string m_test_entry = "";

    // Suppresses log noise from programmatic switch resets.
    bool m_syncing = false;

    void sync_switches_to_state();
    void on_switch_changed(GtkSwitch *sw, const char *name);
    void on_apply();
    void on_cancel();

    static void on_sw_dark_notify (GObject *, GParamSpec *, gpointer self);
    static void on_sw_notif_notify(GObject *, GParamSpec *, gpointer self);
    static void on_sw_save_notify (GObject *, GParamSpec *, gpointer self);
    static void on_btn_apply      (GtkButton *, gpointer self);
    static void on_btn_cancel     (GtkButton *, gpointer self);

    IMainWindow *m_main_window = nullptr;
};